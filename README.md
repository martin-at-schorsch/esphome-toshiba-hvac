# ESPHome Custom Components Collection

This repository houses custom components for ESPHome.

## Included Components

1.  **Toshiba HVAC**: Control Toshiba air conditioners via UART.
2.  **LilyGo T5-4.7 Display**: Driver for the LilyGo T5-4.7 E-Paper Display (ESP-IDF 5 compatible).

---

# 1. Toshiba HVAC Component

## Description

This is an ESPHome component for controlling Toshiba HVAC (Heating, Ventilation, and Air Conditioning) systems. It allows you to integrate your Toshiba air conditioner into your ESPHome setup, enabling control via Home Assistant or other ESPHome-compatible systems.

## Features

* Control Toshiba HVAC systems
* Integration with ESPHome and Home Assistant
* Set modes (Heat, Cool, Auto, etc.)
* Set target temperature
* Control fan speed

## Prerequisites

* ESPHome installed
* An ESP8266 or ESP32 board
* A Toshiba HVAC system
* Connection between the ESP board and the Toshiba HVAC system

## Installation

1.  **Clone Repository:**
    ```bash
    git clone https://github.com/martin-at-schorsch/esphome-toshiba-hvac
    ```

2.  **Configure External Component:**
    Add the external component to your ESPHome configuration:
    ```yaml
    external_components:
      - source:
          type: git
          url: https://github.com/martin-at-schorsch/esphome-toshiba-hvac
        components:
          - toshiba_hvac
    ```

3.  **Configure Climate Component:**
    Configure the climate component in your yaml:
    ```yaml
    climate:
      - platform: toshiba_hvac
        name: "Toshiba HVAC"
        uart_id: hvac_uart
        supported_modes:
          - "off"
          - "heat"
          - "cool"
          - "auto"
          - "dry"
          - "fan_only"
        supported_fan_modes:
          - "auto"
          - "low"
          - "medium"
          - "high"
          - "silent"
        default_temperature: 22 °C
        visual:
          temperature_step: 1 °C
    ```

## Configuration Variables

* `name`: The name of the HVAC device in ESPHome.
* `uart_id`: The ID of the UART bus used.
* `supported_modes`: List of modes supported by your unit.
* `supported_fan_modes`: List of fan modes supported by your unit.
* `default_temperature`: The default target temperature.
* `visual`: Settings for temperature representation in the UI.

---

# 2. LilyGo T5-4.7 Display Component

## Description
This component enables the use of the LilyGo T5-4.7 E-Paper Display with ESPHome. It is based on the [epdiy](https://github.com/vroland/epdiy) library with ESP-IDF 5.x support added by (https://github.com/Frogy76/epdiy) and includes specific adjustments and fixes for compatibility with **ESP-IDF 5.x** (specifically ADC calibration) which are currently missing in the standard repository and Frogy76's fork. 




## Features
*   Full support for the 4.7" E-Paper Display (960x540) used in the LilyGo T5-4.7 (non S3 variant).
*   Integration into the ESPHome Display Platform (Lambda support).
*   Works with ESP-IDF and Arduino framework within ESPHome.
*   **ESP-IDF 5 Compatible**: Uses the correct ADC calibration scheme (Line Fitting instead of Curve Fitting) for the ESP32.
*   **PSRAM Check**: Automatically validates if PSRAM is enabled to prevent runtime crashes.

## Pinout (Hardcoded)
The driver uses the following fixed pin mapping for the T5-4.7 board. These pins are automatically configured by the driver:

| Function     | Pin     | Description            |
| :----------- | :------ | :--------------------- |
| **Data Bus** |         |                        |
| D0           | GPIO 33 | Data Bit 0             |
| D1           | GPIO 32 | Data Bit 1             |
| D2           | GPIO 4  | Data Bit 2             |
| D3           | GPIO 19 | Data Bit 3             |
| D4           | GPIO 2  | Data Bit 4             |
| D5           | GPIO 27 | Data Bit 5             |
| D6           | GPIO 21 | Data Bit 6             |
| D7           | GPIO 22 | Data Bit 7             |
| **Control**  |         |                        |
| CKH          | GPIO 5  | Clock Horizontal       |
| CKV          | GPIO 25 | Clock Vertical         |
| STH          | GPIO 26 | Start Pulse Horizontal |
| V4_LATCH     | GPIO 15 | Latch Enable           |
| CFG_DATA     | GPIO 23 | Config Data            |
| CFG_CLK      | GPIO 18 | Config Clock           |
| CFG_STR      | GPIO 0  | Config Strobe          |

## Installation

Add the following to your ESPHome configuration. It is important that **both** components (`lilygo_t5_47_display` and `epdiy`) are loaded:

```yaml
# PSRAM is mandatory!
psram:
  mode: quad
  speed: 40MHz

external_components:
  - source: github://martin-at-schorsch/esphome-toshiba-hvac
    components: 
      - lilygo_t5_47_display
      - epdiy

display:
  - platform: lilygo_t5_47_display
    id: eink
    full_update_every: 30
    lambda: |-
      it.print(960/2, 540/2, id(font), TextAlign::CENTER, "Hello World!");
```

## Important Notes
*   **PSRAM Required**: This component strictly requires a board with enabled PSRAM (e.g., ESP32-WROVER). If PSRAM is not enabled, the build will fail with an error message to prevent runtime issues.
*   **ESP-IDF Version**: Tested and functional with newer ESPHome versions (from 2024.x) that use ESP-IDF 5.x.
*   **Color Handling**: This component treats **White as 255** and **Black as 0**.
    *   Many ESPHome display components (like OLEDs) assume a black background (0) by default.
    *   Since E-Paper defaults to a white background (phyiscally), you might need to ensure your content is drawn in Black (0) on a White (255) background.
    *   **New Feature**: You can set `invert_colors: true` in your configuration to automatically invert all colors (White becomes Black, Black becomes White). This mimics the behavior of other display types where "cleared" pixels are black.

---

## License

GNU General Public License v3.0
