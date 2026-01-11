# EPDiy E-Paper Driver (ESP-IDF 5.x Compatible)

This is a modified version of the [EPDiy library](https://github.com/vroland/epdiy), tailored for compatibility with **ESP-IDF 5.x** and specific support for the **LilyGo T5-4.7** in ESPHome.

## Base Fork

This component is based on the [Frogy76/epdiy](https://github.com/Frogy76/epdiy) fork, which introduced initial support for ESP-IDF 5.5.1 and resolved several breaking changes (GPIO HAL, RCC Atomic, GDMA).

## Modifications for ESPHome / LilyGo T5-4.7

In addition to the ESP-IDF 5 fixes from the Frogy76 fork, the following changes were applied to this local copy to ensure correct operation on the **LilyGo T5-4.7 (ESP32)** with modern ESPHome versions:

### 1. ADC Calibration Fix (ESP32 Legacy)
The standard ESP32 implementation in ESP-IDF 5 does not default to "Curve Fitting" for ADC calibration, which caused compilation errors in the original fork.
- Switched default calibration scheme from "Curve Fitting" to "Line Fitting" in `src/board/epd_board_common.c`.
- Replaced `adc_cali_curve_fitting_config_t` with `adc_cali_line_fitting_config_t`.

### 2. Component Structure
- Added `__init__.py` to allow this directory to be treated as a valid ESPHome external component.

## Features

- ✅ **ESP-IDF 5.x Support** - Compatible with the latest ESP-IDF framework used by ESPHome.
- ✅ **Parallel E-Paper Support** - Drivers for driving parallel e-paper displays (ED047TC1, ED060SC4, etc.) directly from the ESP32.

## Licenses

The original project is licensed under the Apache License 2.0 (or similar, see original repo).
The board and schematic from the original project are licensed under CC-BY-SA 4.0.
Firmware and examples are licensed under the terms of the GNU Lesser GPL version 3.
Utilities are licensed under the terms of the MIT license.
