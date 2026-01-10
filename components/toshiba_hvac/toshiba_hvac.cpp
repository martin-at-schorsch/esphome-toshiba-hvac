// esphome/components/toshiba_hvac/toshiba_hvac.cpp

#include "toshiba_hvac.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace toshiba_hvac {

const char *const ToshibaHVACClimate::TAG = "toshiba_hvac";

using namespace climate;

// Zeitabstand für das Polling in Millisekunden
const uint32_t POLLING_INTERVAL_MS = 5000;

// ----- Protokoll-Konstanten (basiert auf ormsport/ToshibaCarrierHvac) -----
// Befehls-Header
const uint8_t HEADER_1 = 0x20;
const uint8_t HEADER_2 = 0x00;
const uint8_t HEADER_3 = 0x00;
const uint8_t HEADER_4 = 0x00;

// Befehls-Typen
const uint8_t CMD_TYPE_STATUS_REQUEST = 0x08;
const uint8_t CMD_TYPE_SET_STATE = 0x09;

// Nachrichten-Nutzlast für Statusabfrage
const uint8_t STATUS_REQUEST_PAYLOAD[] = {0x01, 0x00, 0x19};

// -------------------------------------------------------------------------

void ToshibaHVACClimate::setup() {
    ESP_LOGCONFIG(TAG, "Toshiba HVAC wird eingerichtet...");
    this->mode = CLIMATE_MODE_OFF;
    this->target_temperature = 22.0f;
    this->fan_mode = CLIMATE_FAN_AUTO;
    
    // Initialen Zustand für den Vergleich speichern.
    this->last_sent_mode_ = this->mode;
    this->last_sent_target_temperature_ = this->target_temperature;
    
    if (this->fan_mode.has_value()) {
        this->last_sent_fan_mode_ = *this->fan_mode;
    }
    
    this->last_poll_ = millis();
}

void ToshibaHVACClimate::dump_config() {
    LOG_CLIMATE("", "Toshiba HVAC", this);
}

void ToshibaHVACClimate::loop() {
    // Regelmäßig den Status von der Klimaanlage abfragen
    if (millis() - this->last_poll_ > POLLING_INTERVAL_MS) {
        this->last_poll_ = millis();
        ESP_LOGD(TAG, "Sende Status-Polling-Anfrage...");
        
        std::vector<uint8_t> poll_command = {HEADER_1, HEADER_2, HEADER_3, HEADER_4,
                                             CMD_TYPE_STATUS_REQUEST, 
                                             STATUS_REQUEST_PAYLOAD[0], STATUS_REQUEST_PAYLOAD[1], STATUS_REQUEST_PAYLOAD[2]};
        // Checksumme hinzufügen (falls für Poll-Befehl erforderlich) und senden.
        // In vielen Implementierungen hat der Poll-Befehl eine feste Checksumme.
        // Laut `ormsport` ist die Checksumme für Poll `0x19`.
        this->send_hvac_command(poll_command);
    }
    
    // Eingehende Daten von der Klimaanlage lesen
    while (this->uart_->available()) {
        // Hier Logik zum Lesen und Verarbeiten der Antwort implementieren
    }
}

ClimateTraits ToshibaHVACClimate::traits() {
    auto traits = ClimateTraits();
    traits.set_supports_current_temperature(false); // Anpassen, falls ein Sensor ausgelesen wird
    traits.set_visual_min_temperature(17.0f);
    traits.set_visual_max_temperature(30.0f);
    traits.set_visual_temperature_step(1.0f);

    traits.set_supported_modes({
        CLIMATE_MODE_OFF, CLIMATE_MODE_COOL, CLIMATE_MODE_HEAT,
        CLIMATE_MODE_DRY, CLIMATE_MODE_FAN_ONLY, CLIMATE_MODE_AUTO
    });

    traits.set_supported_fan_modes({
        CLIMATE_FAN_AUTO, CLIMATE_FAN_LOW, CLIMATE_FAN_MEDIUM, CLIMATE_FAN_HIGH
    });

    return traits;
}

void ToshibaHVACClimate::control(const ClimateCall &call) {
    bool state_changed = false;
    if (call.get_mode().has_value()) {
        this->mode = *call.get_mode();
        state_changed = true;
    }
    if (call.get_target_temperature().has_value()) {
        this->target_temperature = *call.get_target_temperature();
        state_changed = true;
    }
    if (call.get_fan_mode().has_value()) {
        this->fan_mode = *call.get_fan_mode();
        state_changed = true;
    }

    if (state_changed) {
        this->set_hvac_state();
    }

    this->publish_state();
}

uint8_t ToshibaHVACClimate::calculate_checksum(const std::vector<uint8_t> &command) {
    uint8_t checksum = 0;
    // Die Checksumme ist ein einfaches XOR über alle Bytes des Befehls.
    for (uint8_t byte : command) {
        checksum ^= byte;
    }
    return checksum;
}

void ToshibaHVACClimate::set_hvac_state() {
    // Sicherstellen, dass alle Werte vorhanden sind
    if (!this->fan_mode.has_value()) return;

    ESP_LOGD(TAG, "Bereite Befehl vor: Mode=%s, Temp=%.1f°C, Fan=%s", 
             climate_mode_to_string(this->mode), this->target_temperature, 
             climate_fan_mode_to_string(*this->fan_mode));

    // ---- Befehl gemäß ormsport/ToshibaCarrierHvac Protokoll zusammenbauen ----
    // Byte 0-3: Header
    // Byte 4:   Befehlstyp (0x09 für Setzen)
    // Byte 5:   Power & Modus
    // Byte 6:   Temperatur
    // Byte 7:   Lüftermodus & Schwingen (Swing)
    // Byte 8:   Immer 0x00
    // Byte 9:   Checksumme
    // -------------------------------------------------------------------------
    
    std::vector<uint8_t> command(9); // 9 Bytes für den Befehl + 1 für Checksumme
    command[0] = HEADER_1;
    command[1] = HEADER_2;
    command[2] = HEADER_3;
    command[3] = HEADER_4;
    command[4] = CMD_TYPE_SET_STATE;

    // Byte 5: Power & Modus
    // Power ON ist 0x01, OFF ist 0x00. Der Modus wird in die höheren Bits kodiert.
    uint8_t power_mode_byte = 0x00;
    if (this->mode == CLIMATE_MODE_OFF) {
        power_mode_byte = 0x00; // Power OFF
    } else {
        power_mode_byte = 0x01; // Power ON
        power_mode_byte |= (esphome_mode_to_toshiba(this->mode)) << 1;
    }
    command[5] = power_mode_byte;

    // Byte 6: Temperatur
    // Temperatur wird als `(gewünschte_temp - 17) * 2` kodiert.
    uint8_t temp_byte = (static_cast<uint8_t>(this->target_temperature) - 17) * 2;
    command[6] = temp_byte;

    // Byte 7: Lüftermodus
    // Die unteren 3 Bits sind für den Lüftermodus. Swing wird hier nicht gesteuert.
    uint8_t fan_byte = (esphome_fan_mode_to_toshiba(*this->fan_mode));
    command[7] = fan_byte;
    
    // Byte 8 ist immer 0x00
    command[8] = 0x00;

    // Byte 9: Checksumme
    uint8_t checksum = calculate_checksum(command);
    command.push_back(checksum); // Füge die Checksumme als 10. Byte hinzu

    send_hvac_command(command);

    // Neuen Zustand als "gesendet" markieren
    this->last_sent_mode_ = this->mode;
    this->last_sent_target_temperature_ = this->target_temperature;
    this->last_sent_fan_mode_ = *this->fan_mode;
}

void ToshibaHVACClimate::send_hvac_command(const std::vector<uint8_t> &command) {
    if (this->uart_ == nullptr) {
        ESP_LOGE(TAG, "UART nicht konfiguriert!");
        return;
    }
    ESP_LOGD(TAG, "Sende Befehl: %s", format_hex_pretty(command).c_str());
    this->uart_->write_array(command);
    this->uart_->flush(); // Sicherstellen, dass der Befehl gesendet wird
}

// Konvertierungs-Helfer (Enums müssen mit dem Protokoll übereinstimmen)
uint8_t ToshibaHVACClimate::esphome_mode_to_toshiba(ClimateMode mode) {
    switch (mode) {
        case CLIMATE_MODE_AUTO:     return 0b000; // 0
        case CLIMATE_MODE_COOL:     return 0b001; // 1
        case CLIMATE_MODE_HEAT:     return 0b010; // 2
        case CLIMATE_MODE_DRY:      return 0b011; // 3
        case CLIMATE_MODE_FAN_ONLY: return 0b100; // 4
        case CLIMATE_MODE_OFF:      return 0b101; // 5
        //case CLIMATE_MODE_HEAT_COOL:return 0b110; // 6
        //case CLIMATE_MODE_DRY_COOL: return 0b111; // 7
        default:                    return 0b000; // Fallback auf Auto
    }
}

uint8_t ToshibaHVACClimate::esphome_fan_mode_to_toshiba(ClimateFanMode fan_mode) {
    switch (fan_mode) {
        case CLIMATE_FAN_AUTO:   return 0b000; // 0
        case CLIMATE_FAN_LOW:    return 0b001; // 1
        case CLIMATE_FAN_MEDIUM: return 0b010; // 2
        case CLIMATE_FAN_HIGH:   return 0b100; // 4 (Laut Repo ist "Medium-High" 3, "High" ist 4)
        //case CLIMATE_FAN_SILENT: return 0b101; // 5
        //case CLIMATE_FAN_MAX:    return 0b110; // 6
        //case CLIMATE_FAN_ECO:    return 0b111; // 7
        default:                 return 0b000;
    }
}

}  // namespace toshiba_hvac
}  // namespace esphome
