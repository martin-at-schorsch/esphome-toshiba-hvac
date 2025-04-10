// toshiba_hvac.h
#ifndef ESPHOME_COMPONENTS_TOSHIBA_HVAC_H_
#define ESPHOME_COMPONENTS_TOSHIBA_HVAC_H_

#include "esphome.h"

#include <vector>
#include <array>

// Toshiba HVAC Konstanten und Enums (wie im vorherigen YAML)
namespace ToshibaHVAC {
    // Befehle
    extern const std::vector<uint8_t> POLL_COMMAND;
    extern const std::vector<uint8_t> SET_MODE_TEMP_COMMAND_PREFIX;
    extern const std::vector<uint8_t> FAN_SPEED_COMMAND_PREFIX;

    // Modi
    enum class OperationMode {
        AUTO = 0x00,
        COOL = 0x01,
        HEAT = 0x02,
        DRY = 0x03,
        FAN_ONLY = 0x04,
        OFF = 0x05
    };

      // Fan Geschwindigkeiten
    enum class FanSpeed {
        AUTO = 0x00,
        LOW = 0x01,
        MEDIUM = 0x02,
        HIGH = 0x03,
        SILENT = 0x06
    };

    // Hilfsfunktionen
    uint8_t calculateChecksum(const std::vector<uint8_t>& data);
    std::vector<uint8_t> addChecksum(const std::vector<uint8_t>& data);
    
    //Konvertiert den Enum Wert in den uint8_t Wert
    uint8_t operationModeToByte(OperationMode mode);
    uint8_t fanSpeedToByte(FanSpeed speed);

} // namespace ToshibaHVAC

// Klasse für die Toshiba HVAC Komponente
class ToshibaHVACClimate : public Component, public Climate {
public:
    ToshibaHVACClimate(UARTComponent *uart) : uart_(uart) {}

    void setup() override;
    void loop() override;

    // Von der Climate Klasse geerbte Methoden
    void set_target_temperature(float temperature) override;
    void set_mode(ClimateMode mode) override;
    void set_fan_mode(FanMode mode) override;
    
    float get_target_temperature() override;
    ClimateMode get_mode() override;
    FanMode get_fan_mode() override;
    
    
    
private:
    UARTComponent *uart_;

    // Aktueller Zustand
    float current_temperature_ = 22.0f;  // Standardwert
    ToshibaHVAC::OperationMode current_mode_ = ToshibaHVAC::OperationMode::AUTO;
    ToshibaHVAC::FanSpeed current_fan_speed_ = ToshibaHVAC::FanSpeed::AUTO;
    bool hvac_is_on_ = true;

    // Senden und Empfangen von Daten
    void sendHVACCommand(const std::vector<uint8_t>& command);
    std::vector<uint8_t> receiveHVACResponse();
    void processHVACResponse(const std::vector<uint8_t>& data);

    // Helfermethoden zum Setzen von Werten
     void setHVACMode(ToshibaHVAC::OperationMode mode);
     void setHVACTemperature(int temp);
     void setHVACFanSpeed(ToshibaHVAC::FanSpeed speed);
     
    // Konvertiert ESPHome ClimateMode und FanMode zu ToshibaHVAC Enums.
    ToshibaHVAC::OperationMode esphomeModeToToshibaMode(ClimateMode mode);
    ClimateMode toshibaModeToEsphomeMode(ToshibaHVAC::OperationMode mode);
    
    ToshibaHVAC::FanSpeed esphomeFanModeToToshibaFanSpeed(FanMode mode);
    FanMode toshibaFanSpeedToEsphomeFanMode(ToshibaHVAC::FanSpeed speed);
    
};

#endif // ESPHOME_COMPONENTS_TOSHIBA_HVAC_H_
