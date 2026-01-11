#pragma once

#include "esphome/core/component.h"
#include "esphome/components/climate/climate.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/select/select.h"
#include "esphome/components/switch/switch.h"
#include "esphome/components/button/button.h"

namespace esphome {
namespace toshiba_hvac {

static const char *const TAG = "toshiba_hvac";

// Constants from ToshibaCarrierHvac.h
static const uint8_t HANDSHAKE_SYN_PACKET_1[] = {2, 255, 255, 0, 0, 0, 0, 2};
static const uint8_t HANDSHAKE_SYN_PACKET_2[] = {2, 255, 255, 1, 0, 0, 1, 2, 254};
static const uint8_t HANDSHAKE_SYN_PACKET_3[] = {2, 0, 0, 0, 0, 0, 2, 2, 2, 250};
static const uint8_t HANDSHAKE_SYN_PACKET_4[] = {2, 0, 1, 129, 1, 0, 2, 0, 0, 123};
static const uint8_t HANDSHAKE_SYN_PACKET_5[] = {2, 0, 1, 2, 0, 0, 2, 0, 0, 254};
static const uint8_t HANDSHAKE_SYN_PACKET_6[] = {2, 0, 2, 0, 0, 0, 0, 254};

static const uint8_t HANDSHAKE_ACK_PACKET_1[] = {2, 0, 2, 1, 0, 0, 2, 0, 0, 251};
static const uint8_t HANDSHAKE_ACK_PACKET_2[] = {2, 0, 2, 2, 0, 0, 2, 0, 0, 250};

static const uint8_t HANDSHAKE_HEADER[] = {2, 0, 0};
static const uint8_t CONFIRM_HEADER[] = {2, 0, 2};
static const uint8_t PACKET_HEADER[] = {2, 0, 3};

// Packet Types
static const uint8_t PACKET_TYPE_COMMAND = 16;
static const uint8_t PACKET_TYPE_FEEDBACK = 17;
static const uint8_t PACKET_TYPE_SYN_ACK = 128;
static const uint8_t PACKET_TYPE_ACK = 130;
static const uint8_t PACKET_TYPE_REPLY = 144;

// Function Bytes
static const uint8_t FUNC_ROOMTEMP = 187; // or 190 based on code
static const uint8_t FUNC_OUTSIDETEMP = 190;
static const uint8_t FUNC_STATE = 128;
static const uint8_t FUNC_MODE = 176;
static const uint8_t FUNC_SETPOINT = 179;
static const uint8_t FUNC_FANMODE = 160;
static const uint8_t FUNC_SWING = 163;
static const uint8_t FUNC_OP = 247;
static const uint8_t FUNC_FN_GROUP_1 = 248;
static const uint8_t FUNC_PSEL = 135;
static const uint8_t FUNC_PURE = 199;

// New Helper Classes
class ToshibaHVACClimate;

class ToshibaPowerSelect : public select::Select, public Component {
 public:
  void set_parent(ToshibaHVACClimate *parent) { parent_ = parent; }
  void control(const std::string &value) override;
 protected:
  ToshibaHVACClimate *parent_{nullptr};
};

class ToshibaPureSwitch : public switch_::Switch, public Component {
 public:
  void set_parent(ToshibaHVACClimate *parent) { parent_ = parent; }
  void write_state(bool state) override;
 protected:
  ToshibaHVACClimate *parent_{nullptr};
};

class ToshibaQueryButton : public button::Button, public Component {
 public:
  void set_parent(ToshibaHVACClimate *parent) { parent_ = parent; }
  void press_action() override;
 protected:
  ToshibaHVACClimate *parent_{nullptr};
};

class ToshibaHVACClimate : public climate::Climate, public uart::UARTDevice, public Component {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;

  climate::ClimateTraits traits() override;
  void control(const climate::ClimateCall &call) override;
  
  void set_outdoor_temperature_sensor(sensor::Sensor *sensor) { this->outdoor_temperature_sensor_ = sensor; }
  void set_power_select(ToshibaPowerSelect *sel) { this->power_select_ = sel; }
  void set_pure_switch(ToshibaPureSwitch *sw) { this->pure_switch_ = sw; }
  void set_query_button(ToshibaQueryButton *btn) { this->query_button_ = btn; }
  
  // Public methods to be called by helper classes
  void set_power_sel(const std::string &value);
  void set_pure(bool state);
  void on_query_button_press();

 protected:
  sensor::Sensor *outdoor_temperature_sensor_{nullptr};
  ToshibaPowerSelect *power_select_{nullptr};
  ToshibaPureSwitch *pure_switch_{nullptr};
  ToshibaQueryButton *query_button_{nullptr};
  
  void send_packet(const std::vector<uint8_t> &data);
  void create_packet(const std::vector<uint8_t> &header, uint8_t packet_type, const std::vector<uint8_t> &data);
  void send_handshake();
  void query_all();
  void query_temperature();
  
  bool read_packet(const std::vector<uint8_t> &data);
  bool process_data(const std::vector<uint8_t> &data);
  uint8_t calculate_checksum(uint16_t base_key, const std::vector<uint8_t> &data);
  int8_t correction_temperature(uint8_t val);

  bool first_run_{true};
  bool connected_{false};
  bool handshake_active_{false};
  bool ready_{false};
  bool init_done_{false};
  bool send_wake_{false};

  uint32_t last_receive_{0};
  uint32_t last_send_wake_{0};
  uint32_t last_sync_settings_{0};
  
  // Buffers
  std::vector<uint8_t> rx_buffer_;
  uint32_t rx_start_time_{0};
  
  // Limits
  static const uint32_t RX_READ_TIMEOUT = 300; // Increased to 300ms
  static const uint32_t COMMAND_DELAY = 200;   // Increased to 200ms
  static const uint32_t CONNECTION_TIMEOUT = 2 * 60 * 1000; // 2 minutes
  static const uint32_t IDLE_TIMEOUT = 1 * 60 * 1000; // 1 minute
  static const uint32_t START_DELAY = 2 * 1000; // 2 seconds

  // Current State Tracking
  climate::ClimateMode mode_{climate::CLIMATE_MODE_OFF};
  uint8_t setpoint_{22};
  climate::ClimateFanMode fan_mode_{climate::CLIMATE_FAN_AUTO};
  climate::ClimateSwingMode swing_mode_{climate::CLIMATE_SWING_OFF};
  
  // Helpers for conversions
  uint8_t esphome_mode_to_toshiba(climate::ClimateMode mode);
  
  uint8_t esphome_fan_mode_to_toshiba(climate::ClimateFanMode fan_mode);

  uint8_t esphome_swing_to_toshiba(climate::ClimateSwingMode swing);
};

}  // namespace toshiba_hvac
}  // namespace esphome
