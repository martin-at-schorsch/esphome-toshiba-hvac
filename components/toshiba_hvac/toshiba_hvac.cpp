// toshiba_hvac.cpp
#include "toshiba_hvac.h"
#include "esphome/core/log.h"

namespace esphome {
    namespace toshiba_hvac {

        static const char *TAG = "toshiba_hvac";

        // Toshiba HVAC Konstanten (Definitionen)
        const std::vector<uint8_t> POLL_COMMAND = {0x20, 0x00, 0x00, 0x00, 0x08, 0x01, 0x00, 0x19};
        const std::vector<uint8_t> SET_MODE_TEMP_COMMAND_PREFIX = {0x20, 0x00, 0x00, 0x00, 0x11, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //Basisteil zum Setzen von Modus und Temperatur
        const std::vector<uint8_t> FAN_SPEED_COMMAND_PREFIX = {0x20, 0x00, 0x00, 0x00, 0x09, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //Basis zum setzen der Fan Speed

        // Hilfsfunktionen (Definitionen)
        uint8_t ToshibaHVAC::calculateChecksum(const std::vector<uint8_t>& data) {
            uint8_t checksum = 0;
            for (uint8_t byte : data) {
                checksum += byte;
            }
            return checksum;
        }

        std::vector<uint8_t> ToshibaHVAC::addChecksum(const std::vector<uint8_t>& data) {
            std::vector<uint8_t> commandWithChecksum = data;
            uint8_t checksum = calculateChecksum(data);
            commandWithChecksum.push_back(checksum);
            return commandWithChecksum;
        }
        
        //Konvertiert den Enum Wert in den uint8_t Wert
        uint8_t ToshibaHVAC::operationModeToByte(OperationMode mode){
            switch(mode){
                case OperationMode::AUTO: return 0x00;
                case OperationMode::COOL: return 0x01;
                case OperationMode::HEAT: return 0x02;
                case OperationMode::DRY: return 0x03;
                case OperationMode::FAN_ONLY: return 0x04;
                case OperationMode::OFF: return 0x05;
                default: return 0x00;
            }
        }
        
        uint8_t ToshibaHVAC::fanSpeedToByte(FanSpeed speed){
             switch(speed){
                case FanSpeed::AUTO: return 0x00;
                case FanSpeed::LOW: return 0x01;
                case FanSpeed::MEDIUM: return 0x02;
                case FanSpeed::HIGH: return 0x03;
                case FanSpeed::SILENT: return 0x06;
                default: return 0x00;
            }
        }

        // ToshibaHVACClimate Klassenmethoden

        void ToshibaHVACClimate::setup() {
            // Initialisierung der UART-Kommunikation erfolgt bereits durch ESPHome
            ESP_LOGCONFIG(TAG, "Setting up Toshiba HVAC communication");
        }

        void ToshibaHVACClimate::loop() {
            // Regelmäßig Status abfragen
            sendHVACCommand(ToshibaHVAC::POLL_COMMAND);
            std::vector<uint8_t> response = receiveHVACResponse();
            processHVACResponse(response);
            delay(5000); // Wartezeit bis zur nächsten Abfrage
        }

        void ToshibaHVACClimate::sendHVACCommand(const std::vector<uint8_t>& command) {
            std::vector<uint8_t> fullCommand = ToshibaHVAC::addChecksum(command);
            for (uint8_t byte : fullCommand) {
                uart_->write(byte);
            }
            // Wartezeit für Antwort
            delay(100);
        }

        std::vector<uint8_t> ToshibaHVACClimate::receiveHVACResponse() {
            std::vector<uint8_t> response;
            while (uart_->available()) {
                response.push_back(uart_->read());
            }
            return response;
        }

        void ToshibaHVACClimate::processHVACResponse(const std::vector<uint8_t>& data) {
           if (data.empty()) return;

          ESP_LOGD(TAG, "Received data: %s", format_hex(data).c_str());

           // Beispiel: Statusantwort verarbeiten (sehr vereinfacht)
           // Annahme:  Byte 0: Startbyte, Byte 1: Befehl, Daten folgen, Letztes Byte: Checksumme
           if (data.size() > 3 && data[0] == 0x24) { // Beispiel Startbyte
                //Hier die Logik zum Verarbeiten der Antwort einfügen
                // Temperatur extrahieren
                if(data[1] == 0x21){
                    int temperature = data[5]; //zb Byte 5 enthält die Temp
                    ESP_LOGD(TAG,"Temp: %i", temperature);
                    current_temperature_ = temperature;
                    //Man muss hier den Wert noch an ESPHome übermitteln.
                    this->publish_state();
                }
           }
        }

        void ToshibaHVACClimate::set_target_temperature(float temperature) {
            ESP_LOGD(TAG, "Setting target temperature to: %f", temperature);
            // Hier die Logik zum Umwandeln der Temperatur in das Toshiba-Format
            int temp_toshiba = static_cast<int>(temperature); // Beispiel: float zu int
            setHVACTemperature(temp_toshiba);
            this->publish_state();
        }

        void ToshibaHVACClimate::set_mode(ClimateMode mode) {
             ESP_LOGD(TAG, "Setting mode to: %i", mode);
             hvac_is_on_ = (mode != CLIMATE_MODE_OFF);
             ToshibaHVAC::OperationMode toshibaMode = esphomeModeToToshibaMode(mode);
             setHVACMode(toshibaMode);
             this->publish_state();
        }

       void ToshibaHVACClimate::set_fan_mode(FanMode mode){
            ESP_LOGD(TAG, "Setting fan mode: %i", mode);
            ToshibaHVAC::FanSpeed toshibaFanSpeed = esphomeFanModeToToshibaFanSpeed(mode);
            setHVACFanSpeed(toshibaFanSpeed);
            this->publish_state();
       }
       
        float ToshibaHVACClimate::get_target_temperature() override{
            return current_temperature_;
        }
        
        ClimateMode ToshibaHVACClimate::get_mode() override{
            return toshibaModeToEsphomeMode(current_mode_);
        }
        
        FanMode ToshibaHVACClimate::get_fan_mode() override{
            return toshibaFanSpeedToEsphomeFanMode(current_fan_speed_);
        }

        void ToshibaHVACClimate::setHVACMode(ToshibaHVAC::OperationMode mode) {
            current_mode_ = mode;
            std::vector<uint8_t> command = ToshibaHVAC::SET_MODE_TEMP_COMMAND_PREFIX;
            command[7] = ToshibaHVAC::operationModeToByte(mode); // Mode Byte
            sendHVACCommand(command);
        }

        void ToshibaHVACClimate::setHVACTemperature(int temp) {
            current_temperature_ = temp;
            std::vector<uint8_t> command = ToshibaHVAC::SET_MODE_TEMP_COMMAND_PREFIX;
            command[9] = static_cast<uint8_t>(temp);  // Temperatur Byte
            sendHVACCommand(command);
        }
        
        void ToshibaHVACClimate::setHVACFanSpeed(ToshibaHVAC::FanSpeed speed){
            current_fan_speed_ = speed;
            std::vector<uint8_t> command = ToshibaHVAC::FAN_SPEED_COMMAND_PREFIX;
            command[6] = ToshibaHVAC::fanSpeedToByte(speed);
            sendHVACCommand(command);
        }

        ToshibaHVAC::OperationMode ToshibaHVACClimate::esphomeModeToToshibaMode(ClimateMode mode) {
            switch (mode) {
                case CLIMATE_MODE_OFF: return ToshibaHVAC::OperationMode::OFF;
                case CLIMATE_MODE_HEAT: return ToshibaHVAC::OperationMode::HEAT;
                case CLIMATE_MODE_COOL: return ToshibaHVAC::OperationMode::COOL;
                case CLIMATE_MODE_AUTO: return ToshibaHVAC::OperationMode::AUTO;
                case CLIMATE_MODE_DRY: return ToshibaHVAC::OperationMode::DRY;
                case CLIMATE_MODE_FAN_ONLY: return ToshibaHVAC::OperationMode::FAN_ONLY;
                default: return ToshibaHVAC::OperationMode::AUTO;
            }
        }
        
        ClimateMode ToshibaHVACClimate::toshibaModeToEsphomeMode(ToshibaHVAC::OperationMode mode){
             switch (mode) {
                case ToshibaHVAC::OperationMode::OFF: return CLIMATE_MODE_OFF;
                case ToshibaHVAC::OperationMode::HEAT: return CLIMATE_MODE_HEAT;
                case ToshibaHVAC::OperationMode::COOL: return CLIMATE_MODE_COOL;
                case ToshibaHVAC::OperationMode::AUTO: return CLIMATE_MODE_AUTO;
                case ToshibaHVAC::OperationMode::DRY: return CLIMATE_MODE_DRY;
                case ToshibaHVAC::OperationMode::FAN_ONLY: return CLIMATE_MODE_FAN_ONLY;
                default: return CLIMATE_MODE_AUTO;
            }
        }
        
        ToshibaHVAC::FanSpeed ToshibaHVACClimate::esphomeFanModeToToshibaFanSpeed(FanMode mode) {
            switch (mode) {
              case FAN_MODE_OFF:
              case FAN_MODE_AUTO:
                return ToshibaHVAC::FanSpeed::AUTO;
              case FAN_MODE_LOW:
                return ToshibaHVAC::FanSpeed::LOW;
              case FAN_MODE_MEDIUM:
                return ToshibaHVAC::FanSpeed::MEDIUM;
              case FAN_MODE_HIGH:
                return ToshibaHVAC::FanSpeed::HIGH;
              default:
                return ToshibaHVAC::FanSpeed::AUTO;
            }
        }
        
        FanMode ToshibaHVACClimate::toshibaFanSpeedToEsphomeFanMode(ToshibaHVAC::FanSpeed speed){
             switch (speed) {
                case ToshibaHVAC::FanSpeed::AUTO: return FAN_MODE_AUTO;
                case ToshibaHVAC::FanSpeed::LOW: return FAN_MODE_LOW;
                case ToshibaHVAC::FanSpeed::MEDIUM: return FAN_MODE_MEDIUM;
                case ToshibaHVAC::FanSpeed::HIGH: return FAN_MODE_HIGH;
                case ToshibaHVAC::FanSpeed::SILENT: return FAN_MODE_LOW; //Silent als Low zurückgeben
                default: return FAN_MODE_AUTO;
            }
        }

    } // namespace toshiba_hvac
} // namespace esphome

// Deklaration der Komponente für ESPHome
namespace esphome {
    namespace components {
        void toshiba_hvac::ToshibaHVACClimate::setup() {
            // Initialisierung der UART-Kommunikation
            ESP_LOGCONFIG(TAG, "Setting up Toshiba HVAC communication");
        }
        
        climate::Climate *new_climate(UARTComponent *config) {
            return new toshiba_hvac::ToshibaHVACClimate(config);
        }
        
    }  // namespace components
}  // namespace esphome
