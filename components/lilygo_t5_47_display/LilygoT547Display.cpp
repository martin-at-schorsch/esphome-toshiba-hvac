#include "LilygoT547Display.h"
#include "esphome/core/log.h"
#include <esp_task_wdt.h>

#define WAVEFORM EPD_BUILTIN_WAVEFORM

namespace esphome {
namespace lilygo_t5_47_display {

static const char *const TAG = "lilygo_t5_47_display";

float LilygoT547Display::get_setup_priority() const { return esphome::setup_priority::LATE; }

void LilygoT547Display::set_clear_screen(bool clear) { this->clear_ = clear; }
void LilygoT547Display::set_power_off_delay_enabled(bool power_off_delay_enabled) {
  this->power_off_delay_enabled_ = power_off_delay_enabled;
}
void LilygoT547Display::set_landscape(bool landscape) { this->landscape_ = landscape; }

void LilygoT547Display::set_temperature(uint32_t temperature) { this->temperature_ = temperature; }
void LilygoT547Display::set_full_update_every(uint32_t interval) { this->full_update_every_ = interval; }

int LilygoT547Display::get_width_internal() { return 960; }

int LilygoT547Display::get_height_internal() { return 540; }

void LilygoT547Display::setup() {
  esp_task_wdt_deinit();
  esp_task_wdt_config_t twdt_config = {
      .timeout_ms = 30000,
      .idle_core_mask = 3, // Watch both cores
      .trigger_panic = true,
  };
  esp_task_wdt_init(&twdt_config); // Extend WDT to 30s
  ESP_LOGD(TAG, "Initializing EPD...");

  if (heap_caps_get_free_size(MALLOC_CAP_SPIRAM) < 100000) {
      ESP_LOGE(TAG, "Not enough PSRAM for EPD! Free: %d", heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
  }
  epd_init(&epd_board_lilygo_t5_47, &ED047TC1, EPD_OPTIONS_DEFAULT);
  hl = epd_hl_init(WAVEFORM);
  
  if (hl.front_fb == NULL || hl.back_fb == NULL) {
      ESP_LOGE(TAG, "epd_hl_init failed! PSRAM allocation likely failed.");
      return; 
  }
  if (landscape_) {
    EpdRotation orientation = EPD_ROT_LANDSCAPE;
    epd_set_rotation(orientation);
  } else {
    EpdRotation orientation = EPD_ROT_PORTRAIT;
    epd_set_rotation(orientation);
  }
}

void LilygoT547Display::update() {
  fb = epd_hl_get_framebuffer(&hl);
  
  bool full_update = false;
  if (this->full_update_every_ != 0) {
      static uint32_t frame_count = 0;
      frame_count++;
      if (frame_count >= this->full_update_every_) {
          full_update = true;
          frame_count = 0;
      }
  }

  if (this->init_clear_executed_ == false && this->clear_ == true) {
    full_update = true;
    this->init_clear_executed_ = true;
  }
  
  if (full_update) {
      epd_poweron();
      epd_fullclear(&hl, this->temperature_);
      epd_poweroff();
  } else {
      epd_hl_set_all_white(&hl);
  }
  
  ESP_LOGI(TAG, "Update: Width=%d, Height=%d", this->get_width(), this->get_height());
  
  this->do_update_();
  LilygoT547Display::flush_screen_changes();
}

void LilygoT547Display::clear() {
  epd_hl_set_all_white(&hl);
}

void LilygoT547Display::flush_screen_changes() {
  // Check if FB has content
  if (fb != NULL) {
      bool occupied = false;
      for (int i = 0; i < 960 * 540 / 2; i++) { // Check first half
          if (fb[i] != 0xFF) { // Assuming 0xFF is White (Empty). Any non-0xFF is content.
             occupied = true;
             break;
          }
      }
      ESP_LOGD(TAG, "Framebuffer status before flush: %s. First byte: 0x%02X", occupied ? "Has Content" : "Empty/Uniform (All White)", fb[0]);
  } else {
      ESP_LOGE(TAG, "Framebuffer is NULL!");
  }

  epd_poweron();
  delay(50); // Ensure PMIC ready
  err = epd_hl_update_screen(&hl, MODE_GL16, this->temperature_);
  ESP_LOGI(TAG, "Update finished. Err: %d", err);
  if (this->power_off_delay_enabled_ == true) {
    delay(700);
  }
  epd_poweroff();
}

void LilygoT547Display::set_all_white() { epd_hl_set_all_white(&hl); }
void LilygoT547Display::poweron() { epd_poweron(); }
void LilygoT547Display::poweroff() { epd_poweroff(); }

void LilygoT547Display::on_shutdown() {
  ESP_LOGI(TAG, "Shutting down Lilygo T5-4.7 screen");
  epd_poweroff();
  epd_deinit();
}

void HOT LilygoT547Display::draw_absolute_pixel_internal(int x, int y, Color color) {
  if (color.red == 255 && color.green == 255 && color.blue == 255) {
    epd_draw_pixel(x, y, 255, fb);
  } else if (color.red == 0 && color.green == 0 && color.blue == 0) {
    epd_draw_pixel(x, y, 0, fb);
  } else {
    int col = (0.2126 * color.red) + (0.7152 * color.green) + (0.0722 * color.blue);
    epd_draw_pixel(x, y, col, fb);
  }
}

}  // namespace lilygo_t5_47_display
}  // namespace esphome
