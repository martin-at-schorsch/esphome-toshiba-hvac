#pragma once

#include "esphome/core/component.h"
#include "esphome/components/climate/climate.h" 
#include "esphome/components/uart/uart.h"
#include <vector>

namespace esphome {
namespace toshiba_hvac {

// Interne Enums für die Logik deiner Komponente
enum class OperationMode { AUTO = 0x00, COOL = 0x01, HEAT = 0x02, DRY = 0x03, FAN_ONLY = 0x04, OFF = 0x05 };
enum class FanSpeed { AUTO = 0x00, FAN_LOW = 0x01, FAN_MEDIUM = 0x02, FAN_HIGH = 0x03, FAN_SILENT = 0x04 };

class ToshibaHVACClimate : public climate::Climate, public Component {
public:
    // TAG für das Logging
    static const char *const TAG;

    // Setter für die UART-Komponente
    void set_uart(uart::UARTComponent *uart) { this->uart_ = uart; }

    // Überschriebene Methoden von Component
    void setup() override;
    void loop() override;
    void dump_config() override;

    // Überschriebene Methoden von Climate
    climate::ClimateTraits traits() override; // KORREKTUR: 'get_traits' zu 'traits' geändert
    void control(const climate::ClimateCall &call) override;

protected:
    // Hilfsmethoden zur Ansteuerung der Klimaanlage
    void send_hvac_command(const std::vector<uint8_t> &command);
    void set_hvac_state();

    // Konvertierungs-Helfer
    uint8_t esphome_mode_to_toshiba(climate::ClimateMode mode);
    uint8_t esphome_fan_mode_to_toshiba(climate::ClimateFanMode fan_mode);

    uart::UARTComponent *uart_{nullptr};

    // Interner Zustand, um redundante Befehle zu vermeiden
    climate::ClimateMode last_sent_mode_{climate::CLIMATE_MODE_OFF};
    float last_sent_target_temperature_{22.0f};
    climate::ClimateFanMode last_sent_fan_mode_{climate::CLIMATE_FAN_AUTO};
    
    uint32_t last_poll_{0};
    uint8_t calculate_checksum(const std::vector<uint8_t> &command);
    uint8_t power_mode_byte;
    uint8_t temp_byte;
    uint8_t fan_byte;
    uint8_t swing_byte;
    uint8_t checksum;
    std::vector<uint8_t> command;




};

}  // namespace toshiba_hvac
}  // namespace esphome
