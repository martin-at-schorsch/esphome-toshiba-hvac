#include "toshiba_hvac.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace toshiba_hvac {

void ToshibaHVACClimate::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Toshiba HVAC...");
  this->mode = climate::CLIMATE_MODE_OFF;
  this->target_temperature = 22.0;
  this->fan_mode = climate::CLIMATE_FAN_AUTO;
  this->swing_mode = climate::CLIMATE_SWING_OFF;
  if (this->outdoor_temperature_sensor_ != nullptr) {
      this->outdoor_temperature_sensor_->publish_state(NAN);
  }
}

// Helper Class Callbacks
void ToshibaPowerSelect::control(const std::string &value) {
    this->publish_state(value);
    if (this->parent_ != nullptr)
        this->parent_->set_power_sel(value);
}

void ToshibaPureSwitch::write_state(bool state) {
    this->publish_state(state);
    if (this->parent_ != nullptr)
        this->parent_->set_pure(state);
}

void ToshibaQueryButton::press_action() {
    if (this->parent_ != nullptr)
        this->parent_->on_query_button_press();
}

void ToshibaHVACClimate::set_power_sel(const std::string &value) {
    if (!connected_) return;
    uint8_t psel_val = 100; // Default
    if (value == "75%") psel_val = 75;
    else if (value == "50%") psel_val = 50;

    std::vector<uint8_t> data = {FUNC_PSEL, psel_val};
    create_packet(std::vector<uint8_t>(PACKET_HEADER, PACKET_HEADER+3), PACKET_TYPE_COMMAND, data);
}

void ToshibaHVACClimate::set_pure(bool state) {
    if (!connected_) return;
    // 16=OFF, 24=ON
    uint8_t pure_val = state ? 24 : 16;
    std::vector<uint8_t> data = {FUNC_PURE, pure_val};
    create_packet(std::vector<uint8_t>(PACKET_HEADER, PACKET_HEADER+3), PACKET_TYPE_COMMAND, data);
}
//...
// In control:
// create_packet(std::vector<uint8_t>(PACKET_HEADER, PACKET_HEADER+3), PACKET_TYPE_COMMAND, data);

void ToshibaHVACClimate::on_query_button_press() {
    if (this->connected_) {
        this->query_all();
    }
}

void ToshibaHVACClimate::loop() {
  uint32_t now = millis();
  
  if (!connected_) {
    send_handshake();
    now = millis();
  }

  // First Run initialization
  if (first_run_) {
    last_receive_ = now;
    last_send_wake_ = now;
    first_run_ = false;
  }

  // Periodic polling
  if ((now - last_receive_ >= START_DELAY) && !init_done_ && connected_) {
    query_all();
    init_done_ = true;
  }

  // Idle timeout (watchdog)
  if ((now - last_receive_ >= IDLE_TIMEOUT) && !send_wake_ && init_done_) {
    ESP_LOGD(TAG, "Idle timeout reached, querying temperature...");
    send_wake_ = true;
    query_temperature();
    last_send_wake_ = now;
  }

  // Connection timeout
  if (connected_ && send_wake_ && (now >= last_send_wake_) && (now - last_send_wake_ >= CONNECTION_TIMEOUT)) {
    ESP_LOGW(TAG, "Connection timeout (diff=%u, timeout=%u, wake=%d), restarting handshake...", 
             (now - last_send_wake_), CONNECTION_TIMEOUT, send_wake_);
    first_run_ = true;
    handshake_active_ = false;
    ready_ = false;
    connected_ = false;
    send_wake_ = false;
    init_done_ = false;
    last_receive_ = now;
    last_send_wake_ = now;
  } else if (connected_ && send_wake_ && (now < last_send_wake_)) {
      last_send_wake_ = now;
  }

  // Read incoming data
  while (available()) {
    if (rx_buffer_.empty()) {
        rx_start_time_ = millis();
    }
    uint8_t c;
    read_byte(&c);
    rx_buffer_.push_back(c);
  }
  
  now = millis();

  // Process received buffer
  if (!rx_buffer_.empty()) {
     // Check for timeout or full buffer to process
     if (millis() - rx_start_time_ > RX_READ_TIMEOUT || rx_buffer_.size() > 250) {
        
        ESP_LOGV(TAG, "RX Buffer Content: %s", format_hex_pretty(rx_buffer_).c_str());
        
        // Try to find known headers
        size_t processed = 0;
        
        // Simple parser loop - this is a simplified version of chunkSplit from reference
        // We look for valid packets within the buffer.
        for (size_t i = 0; i < rx_buffer_.size() - 2; i++) {
            // Check for potential headers: 02 00 00 (Handshake), 02 00 02 (Confirm), 02 00 03 (Packet)
            if (rx_buffer_[i] == 0x02 && rx_buffer_[i+1] == 0x00) {
                 if (rx_buffer_[i+2] == 0x00 || rx_buffer_[i+2] == 0x02 || rx_buffer_[i+2] == 0x03) {
                     // Potential packet start
                     // Check if we have enough length for size byte (at index + 6)
                     if (i + 6 < rx_buffer_.size()) {
                         uint8_t packet_len = rx_buffer_[i+6] + 8; // Data len + header(6) + size(1) + unknown(1)
                         // Check valid packet length
                         if (packet_len >= 8 && packet_len <= 30 && (i + packet_len <= rx_buffer_.size())) {
                             std::vector<uint8_t> packet(rx_buffer_.begin() + i, rx_buffer_.begin() + i + packet_len);
                             if (read_packet(packet)) {
                                 processed = i + packet_len;
                                 i += packet_len - 1; // Skip forward
                                 last_receive_ = millis();
                                 send_wake_ = false;
                                 if (!connected_) send_wake_ = true;
                             }
                         }
                     }
                 }
            }
        }
        
        rx_buffer_.clear();
     }
  }
}

bool ToshibaHVACClimate::read_packet(const std::vector<uint8_t> &data) {
    if (data.size() < 4) return false;
    uint8_t packet_type = data[3];

    if (packet_type == PACKET_TYPE_FEEDBACK) {
        if (data.size() < 12) return false;
        uint8_t data_len = data[11];
        if (data.size() < 12 + data_len) return false;
        std::vector<uint8_t> payload(data.begin() + 12, data.begin() + 12 + data_len);
        return process_data(payload);
    } else if (packet_type == PACKET_TYPE_REPLY) {
         if (data.size() < 14) return false;
         uint8_t data_len = data[13];
         if (data.size() < 14 + data_len) return false;
         std::vector<uint8_t> payload(data.begin() + 14, data.begin() + 14 + data_len);
         return process_data(payload);
    } else if (packet_type == PACKET_TYPE_SYN_ACK) {
        ESP_LOGD(TAG, "Received SYN/ACK");
        handshake_active_ = true;
        return true;
    } else if (packet_type == PACKET_TYPE_ACK) {
        ESP_LOGD(TAG, "Received ACK. Connected!");
        ready_ = false;
        handshake_active_ = false;
        connected_ = true;
        return true;
    } else if (data.size() == 14) {
        // Echo of our command? Or "Received query packet, ignored" from ref
        ESP_LOGV(TAG, "Received Echo/Query packet (len 14), ignoring.");
        return true; 
    } else if (packet_type == 0x81 || packet_type == 0x7B || packet_type == 0x91) {
        // Ignored known unknown packets to reduce noise
        return true;
    }
    
    ESP_LOGD(TAG, "Unknown packet type: 0x%02X", packet_type);
    return false;
}

bool ToshibaHVACClimate::process_data(const std::vector<uint8_t> &data) {
    if (data.empty()) return false;
    uint8_t func = data[0];

    // Single Byte Data
    if (data.size() == 2) {
        if (func == FUNC_ROOMTEMP) {
            int8_t temp = correction_temperature(data[1]);
            ESP_LOGD(TAG, "Received Room Temp: %d", temp);
            this->current_temperature = (float)temp;
            ESP_LOGD(TAG, "Updating state. Current temp is now: %.1f", this->current_temperature);
            this->publish_state();
            return true;
        } else if (func == FUNC_OUTSIDETEMP) {
            int8_t temp = correction_temperature(data[1]);
            ESP_LOGD(TAG, "Received Outside Temp: %d", temp);
            if (this->outdoor_temperature_sensor_ != nullptr) {
                if (temp == 127) {
                     // 127 means CDU off/idle.
                     // We publish NAN to indicate unavailable/idle state.
                     if (!std::isnan(this->outdoor_temperature_sensor_->state)) {
                        this->outdoor_temperature_sensor_->publish_state(NAN);
                     }
                } else {
                     this->outdoor_temperature_sensor_->publish_state(temp);
                }
            }
            return true;
        } else if (func == FUNC_SETPOINT) {
             uint8_t sp = data[1];
             ESP_LOGD(TAG, "Received Setpoint: %d", sp);
             this->target_temperature = sp;
             this->publish_state();
             return true;
        } else if (func == FUNC_MODE) {
             // 65=Auto, 66=Cool, 67=Heat, 68=Dry, 69=FanOnly
             ESP_LOGD(TAG, "Received Mode Byte: %d", data[1]);
             // Add logic if needed
             return true;
        } else if (func == FUNC_STATE) {
             // 48=OFF, 49=ON
             ESP_LOGD(TAG, "Received State Byte: %d", data[1]);
             bool is_on = (data[1] == 49); // or 0x31
             if (!is_on) {
                 this->mode = climate::CLIMATE_MODE_OFF;
                 this->publish_state();
             }
             return true;
        } else if (func == FUNC_SWING) {
             ESP_LOGD(TAG, "Received Swing Byte: %d", data[1]);
             return true;
        } else if (func == FUNC_PSEL) {
             // 50=50%, 75=75%, 100=100%
             ESP_LOGD(TAG, "Received PSEL Byte: %d", data[1]);
             if (this->power_select_ != nullptr) {
                 std::string psel = "100%";
                 if (data[1] == 75) psel = "75%";
                 else if (data[1] == 50) psel = "50%";
                 this->power_select_->publish_state(psel);
             }
             return true;
        } else if (func == FUNC_PURE) {
             // 16=OFF, 24=ON
             ESP_LOGD(TAG, "Received PURE Byte: %d", data[1]);
             if (this->pure_switch_ != nullptr) {
                 this->pure_switch_->publish_state(data[1] == 24);
             }
             return true;
        } else if (func == FUNC_OP) {
             ESP_LOGD(TAG, "Received OP Byte: %d", data[1]);
             // 0=Normal, 1=HighPower, 2=Silent1, 3=Eco, 4=8C, 16/32/48=Fireplace (not impl)
             climate::ClimatePreset p = climate::CLIMATE_PRESET_NONE;
             switch (data[1]) {
                 case 1: p = climate::CLIMATE_PRESET_BOOST; break;
                 case 2: p = climate::CLIMATE_PRESET_SLEEP; break;
                 case 3: p = climate::CLIMATE_PRESET_ECO; break;
                 case 4: p = climate::CLIMATE_PRESET_AWAY; break;
             }
             this->preset = p;
             this->publish_state();
             return true;
        }
    }
    
    // Group Data (FN_GROUP_1)
    if (data.size() >= 5 && func == FUNC_FN_GROUP_1) {
        // [FUNC, MODE, SETPOINT, FAN, OP]
        this->target_temperature = data[2];
        ESP_LOGD(TAG, "Received Group1 Data: Setpoint=%d", data[2]);
        this->publish_state();
        return true;
    }

    if (func == 0x80 || func == 0x88 || func == 0x87 || func == 0x90 || func == 0x94 || func == 0xC7 || func == 0xDF) {
        // Suppress known unknown functions
        return true;
    }

    ESP_LOGW(TAG, "Received process_data with unknown FUNC: 0x%02X (Dec: %d), DataLen: %d, Data: %s", 
             func, func, data.size(), format_hex_pretty(data).c_str());
    return false;
}

void ToshibaHVACClimate::send_handshake() {
  uint32_t now = millis();
  static uint32_t last_handshake_attempt = 0;

  // Retry handshake every 5 seconds if not connected
  if (now - last_handshake_attempt < 5000 && !handshake_active_) return;

  if (!connected_) {
     if (!handshake_active_) {
        last_handshake_attempt = now;
        ESP_LOGD(TAG, "Sending Handshake SYN Sequence...");
        
        // Send all 6 SYN packets with strict timing
        std::vector<uint8_t> packets[] = {
            std::vector<uint8_t>(HANDSHAKE_SYN_PACKET_1, HANDSHAKE_SYN_PACKET_1 + sizeof(HANDSHAKE_SYN_PACKET_1)),
            std::vector<uint8_t>(HANDSHAKE_SYN_PACKET_2, HANDSHAKE_SYN_PACKET_2 + sizeof(HANDSHAKE_SYN_PACKET_2)),
            std::vector<uint8_t>(HANDSHAKE_SYN_PACKET_3, HANDSHAKE_SYN_PACKET_3 + sizeof(HANDSHAKE_SYN_PACKET_3)),
            std::vector<uint8_t>(HANDSHAKE_SYN_PACKET_4, HANDSHAKE_SYN_PACKET_4 + sizeof(HANDSHAKE_SYN_PACKET_4)),
            std::vector<uint8_t>(HANDSHAKE_SYN_PACKET_5, HANDSHAKE_SYN_PACKET_5 + sizeof(HANDSHAKE_SYN_PACKET_5)),
            std::vector<uint8_t>(HANDSHAKE_SYN_PACKET_6, HANDSHAKE_SYN_PACKET_6 + sizeof(HANDSHAKE_SYN_PACKET_6))
        };
        
        for (const auto& packet : packets) {
            send_packet(packet);
            delay(200); // Blocking delay to ensure strict timing
        }
        ESP_LOGD(TAG, "Handshake SYN Sequence Sent.");
     }
  } 
  
  if (handshake_active_ && !connected_ && !ready_) {
      ESP_LOGD(TAG, "Sending Handshake ACK Sequence...");
      // Received SYN/ACK, sending ACK packets
      std::vector<uint8_t> packet1(HANDSHAKE_ACK_PACKET_1, HANDSHAKE_ACK_PACKET_1 + sizeof(HANDSHAKE_ACK_PACKET_1));
      send_packet(packet1);
      delay(200);
      
      std::vector<uint8_t> packet2(HANDSHAKE_ACK_PACKET_2, HANDSHAKE_ACK_PACKET_2 + sizeof(HANDSHAKE_ACK_PACKET_2));
      send_packet(packet2);
      
      handshake_active_ = false; 
      ready_ = true; 
      send_wake_ = true; 
      last_send_wake_ = now; 
      ESP_LOGD(TAG, "Handshake ACK Sequence Sent. Waiting for final confirmation.");
  }
}

void ToshibaHVACClimate::query_all() {
    ESP_LOGD(TAG, "Querying all data...");
    uint8_t fns[] = {128, 135, 144, 148, 163, 187, 190, 199, 222, 223, 248};
    for (uint8_t fn : fns) {
        std::vector<uint8_t> data = {fn};
        create_packet(std::vector<uint8_t>(PACKET_HEADER, PACKET_HEADER+3), PACKET_TYPE_COMMAND, data);
        delay(COMMAND_DELAY); // blocking delay to space commands
    }
}

void ToshibaHVACClimate::query_temperature() {
     ESP_LOGD(TAG, "Querying temperatures...");
     uint8_t fns[] = {FUNC_ROOMTEMP, FUNC_OUTSIDETEMP};
     for (uint8_t fn : fns) {
        std::vector<uint8_t> data = {fn};
        create_packet(std::vector<uint8_t>(PACKET_HEADER, PACKET_HEADER+3), PACKET_TYPE_COMMAND, data);
        delay(COMMAND_DELAY);
     }
}

void ToshibaHVACClimate::control(const climate::ClimateCall &call) {
    if (!connected_) {
         ESP_LOGW(TAG, "Not connected, cannot send control command.");
         return;
    }

    if (call.get_mode().has_value()) {
        auto mode_val = *call.get_mode();
        this->mode = mode_val;
        uint8_t protocol_mode = esphome_mode_to_toshiba(mode_val);
        // Special mapping: OFF is handled by STATE command, but MODE command also needed usually
        // If mode is OFF, we normally send STATE=OFF. But here we send STATE command specifically.
        
        if (mode_val == climate::CLIMATE_MODE_OFF) {
             std::vector<uint8_t> data = {FUNC_STATE, 49}; // 49=OFF, 48=ON
             create_packet(std::vector<uint8_t>(PACKET_HEADER, PACKET_HEADER+3), PACKET_TYPE_COMMAND, data);
        } else {
             // Ensure ON
             if (this->mode == climate::CLIMATE_MODE_OFF) { // Attempting to turn ON
                 // Send ON first? Or does setting MODE turn it on? Ref sends STATE=ON
                 std::vector<uint8_t> data = {FUNC_STATE, 48}; // ON
                 create_packet(std::vector<uint8_t>(PACKET_HEADER, PACKET_HEADER+3), PACKET_TYPE_COMMAND, data);
                 delay(200);
             }
             std::vector<uint8_t> data = {FUNC_MODE, protocol_mode};
             create_packet(std::vector<uint8_t>(PACKET_HEADER, PACKET_HEADER+3), PACKET_TYPE_COMMAND, data);
        }
    }
    
    if (call.get_target_temperature().has_value()) {
        this->target_temperature = *call.get_target_temperature();
        uint8_t temp = (uint8_t)this->target_temperature;
        std::vector<uint8_t> data = {FUNC_SETPOINT, temp};
        create_packet(std::vector<uint8_t>(PACKET_HEADER, PACKET_HEADER+3), PACKET_TYPE_COMMAND, data);
    }
    
    if (call.get_fan_mode().has_value()) {
        this->fan_mode = *call.get_fan_mode();
        uint8_t fan = esphome_fan_mode_to_toshiba(this->fan_mode.value());
        std::vector<uint8_t> data = {FUNC_FANMODE, fan};
        create_packet(std::vector<uint8_t>(PACKET_HEADER, PACKET_HEADER+3), PACKET_TYPE_COMMAND, data);
    }
    
    if (call.get_swing_mode().has_value()) {
        this->swing_mode = *call.get_swing_mode();
        uint8_t swing = esphome_swing_to_toshiba(this->swing_mode);
        std::vector<uint8_t> data = {FUNC_SWING, swing};
        create_packet(std::vector<uint8_t>(PACKET_HEADER, PACKET_HEADER+3), PACKET_TYPE_COMMAND, data);
    }

    if (call.get_preset().has_value()) {
        this->preset = *call.get_preset();
        uint8_t op_val = 0; // Normal
        if (this->preset.has_value()) {
            switch (this->preset.value()) {
                case climate::CLIMATE_PRESET_BOOST: op_val = 1; break; // High Power
                case climate::CLIMATE_PRESET_SLEEP: op_val = 2; break; // Silent 1
                case climate::CLIMATE_PRESET_ECO:   op_val = 3; break; // Eco
                case climate::CLIMATE_PRESET_AWAY:  op_val = 4; break; // 8C
                default: op_val = 0; break;
            }
        }
        std::vector<uint8_t> data = {FUNC_OP, op_val};
        create_packet(std::vector<uint8_t>(PACKET_HEADER, PACKET_HEADER+3), PACKET_TYPE_COMMAND, data);
    }
    
    // Publish state happens automatically? No, we should update and publish or wait for feedback
    // Typically wait for feedback, but optimistic update is UI friendly.
    // However, since we receive feedback constantly, better wait for ACK/Feedback
}

void ToshibaHVACClimate::create_packet(const std::vector<uint8_t> &header, uint8_t packet_type, const std::vector<uint8_t> &data) {
    if (packet_type == PACKET_TYPE_COMMAND) {
        // Structure: Header(3) + Type(1) + 00 00 + Size(1) + 01 30 01 + 0x00(filler?) + DataLen(1) + Data(N)
        // Ref: packet[6] = size; packet[7]=1; packet[8]=48; packet[9]=1; packet[11]=dataLen 
        // Note: Ref uses specific fixed bytes for unknown fields
        
        std::vector<uint8_t> packet = header;
        packet.push_back(packet_type);
        packet.push_back(0x00);
        packet.push_back(0x00);
        
        uint8_t packet_size = 12 + data.size() + 1 - 8; // Size field logic from Ref: sizeof(packet)-8
        // Total size = 12 (base) + dataLen + 1 (checksum)
        // Ref: packet[6] = sizeof(packet) - 8. 
        // Base is 12 bytes: head(3)+type(1)+pad(2)+size(1)+unk(3)+pad(1)+datalen(1)
        
        packet.push_back(packet_size); // Index 6
        
        packet.push_back(1);  // Index 7
        packet.push_back(48); // Index 8 '0'
        packet.push_back(1);  // Index 9
        packet.push_back(0);  // Index 10 (Implicit pad?) Ref doesn't set it explicitly but memset 0
        
        packet.push_back(data.size()); // Index 11
        
        packet.insert(packet.end(), data.begin(), data.end());
        
        packet.push_back(calculate_checksum(438, data)); // Checksum Key 438 for Commands
        
        send_packet(packet);
    }
}

void ToshibaHVACClimate::send_packet(const std::vector<uint8_t> &data) {
    write_array(data);
    last_send_wake_ = millis();
    send_wake_ = true;
    ESP_LOGV(TAG, "Sent: %s", format_hex_pretty(data).c_str());
}

uint8_t ToshibaHVACClimate::calculate_checksum(uint16_t base_key, const std::vector<uint8_t> &data) {
    uint16_t key = base_key - (data.size() * 2);
    int16_t result = key;
    for (uint8_t b : data) {
        result -= b;
    }
    if (result > 255) return result - 256;
    else if (result < 0) return result + 256;
    return (uint8_t)result;
}

int8_t ToshibaHVACClimate::correction_temperature(uint8_t val) {
    if (val > 127) return ((256 - val) * (-1));
    return val;
}

// Mappers
uint8_t ToshibaHVACClimate::esphome_mode_to_toshiba(climate::ClimateMode mode) {
    // 65=Auto, 66=Cool, 67=Heat, 68=Dry, 69=FanOnly
    switch(mode) {
        case climate::CLIMATE_MODE_AUTO: return 65;
        case climate::CLIMATE_MODE_COOL: return 66;
        case climate::CLIMATE_MODE_HEAT: return 67;
        case climate::CLIMATE_MODE_DRY: return 68;
        case climate::CLIMATE_MODE_FAN_ONLY: return 69;
        default: return 65;
    }
}

uint8_t ToshibaHVACClimate::esphome_fan_mode_to_toshiba(climate::ClimateFanMode fan_mode) {
    // 49=Quiet, 50-54=Lvl1-5, 65=Auto
    switch(fan_mode) {
        case climate::CLIMATE_FAN_QUIET: return 49;
        case climate::CLIMATE_FAN_LOW: return 50;
        case climate::CLIMATE_FAN_MEDIUM: return 52;
        case climate::CLIMATE_FAN_HIGH: return 54;
        case climate::CLIMATE_FAN_AUTO: return 65;
        default: return 65;
    }
}

void ToshibaHVACClimate::dump_config() {
    LOG_CLIMATE("", "Toshiba HVAC", this);
}

climate::ClimateTraits ToshibaHVACClimate::traits() {
    auto traits = climate::ClimateTraits();
    // traits.set_supports_current_temperature(true);
    traits.set_visual_min_temperature(17);
    traits.set_visual_max_temperature(30);
    traits.set_visual_temperature_step(1);
    
    traits.set_supported_modes({
        climate::CLIMATE_MODE_OFF,
        climate::CLIMATE_MODE_AUTO,
        climate::CLIMATE_MODE_COOL,
        climate::CLIMATE_MODE_HEAT,
        climate::CLIMATE_MODE_DRY,
        climate::CLIMATE_MODE_FAN_ONLY
    });
    
    traits.set_supported_fan_modes({
        climate::CLIMATE_FAN_AUTO,
        climate::CLIMATE_FAN_QUIET,
        climate::CLIMATE_FAN_LOW,
        climate::CLIMATE_FAN_MEDIUM,
        climate::CLIMATE_FAN_HIGH
    });
    
    traits.set_supported_swing_modes({
        climate::CLIMATE_SWING_OFF,
        climate::CLIMATE_SWING_VERTICAL
    });

    traits.set_supported_presets({
        climate::CLIMATE_PRESET_NONE,
        climate::CLIMATE_PRESET_ECO,
        climate::CLIMATE_PRESET_BOOST,
        climate::CLIMATE_PRESET_SLEEP,
        climate::CLIMATE_PRESET_AWAY
    });
    
    return traits;
}

uint8_t ToshibaHVACClimate::esphome_swing_to_toshiba(climate::ClimateSwingMode swing) {
    if (swing == climate::CLIMATE_SWING_VERTICAL) return 65; // V_SWING
    if (swing == climate::CLIMATE_SWING_HORIZONTAL) return 66; // H_SWING
    if (swing == climate::CLIMATE_SWING_BOTH) return 67; // HV_SWING
    return 49; // FIX
}

}  // namespace toshiba_hvac
}  // namespace esphome
