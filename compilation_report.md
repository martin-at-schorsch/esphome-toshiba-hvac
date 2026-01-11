# ESPHome Compilation Report

Date: Fri Dec 12 16:52:54 2025

## Summary
- Total Files Checked: 55
- Success: 19
- Failure: 36

## Details
### ❌ FAIL - esp32s3-ttgo-d-2-test.yaml (70.8s)
```
Hz, 320KB RAM, 4MB Flash
 - contrib-piohome @ 3.4.4 
 - framework-arduinoespressif32 @ 3.3.2 
 - framework-arduinoespressif32-libs @ 5.5.0+sha.129cd0d247 
 - framework-espidf @ 3.50501.0 (5.5.1) 
 - tool-cmake @ 4.0.3 
 - tool-esp-rom-elfs @ 2024.10.11 
 - tool-esptoolpy @ 5.1.0 
 - tool-mklittlefs @ 3.2.0 
 - tool-ninja @ 1.13.1 
 - tool-scons @ 4.40801.0 (4.8.1) 
 - toolchain-xtensa-esp-elf @ 14.2.0+20241119
Creating a new virtual environment for IDF Python dependencies using uv
Using Python 3.12.10 environment at: /root/.platformio/penv/.espidf-5.5.1
Installing ESP-IDF's Python dependencies with uv
Using Python 3.12.10 environment at: /root/.platformio/penv/.espidf-5.5.1
Resolved 27 packages in 856ms
Downloading pydantic-core (1.9MiB)
Downloading cryptography (4.0MiB)
 Downloaded pydantic-core
 Downloaded cryptography
Prepared 17 packages in 852ms
Installed 27 packages in 108ms
 + annotated-types==0.7.0
 + certifi==2025.11.12
 + cffi==2.0.0
 + charset-normalizer==3.4.4
 + click==8.3.1
 + colorama==0.4.6
 + cryptography==44.0.3
 + esp-idf-kconfig==2.5.0
 + idf-component-manager==2.4.3
 + idna==3.11
 + jsonref==1.1.0
 + pycparser==2.23
 + pydantic==2.11.10
 + pydantic-core==2.33.2
 + pydantic-settings==2.12.0
 + pyparsing==3.2.5
 + python-dotenv==1.2.1
 + requests==2.32.5
 + requests-file==3.0.1
 + requests-toolbelt==1.0.0
 + ruamel-yaml==0.18.16
 + ruamel-yaml-clib==0.2.15
 + tqdm==4.67.1
 + truststore==0.10.4
 + typing-extensions==4.15.0
 + typing-inspection==0.4.2
 + urllib3==1.26.20
Reading CMake configuration...
Library Manager: Installing Wire @ 2.0.0
Warning! Could not find the package with 'Wire @ 2.0.0' requirements for your system 'linux_x86_64'
Library Manager: Installing Wire @ 2.0.0
Warning! Could not find the package with 'Wire @ 2.0.0' requirements for your system 'linux_x86_64'
Dependency Graph
|-- Networking @ 3.3.2
|-- ESPmDNS @ 3.3.2
|-- ArduinoJson @ 7.4.2
Compiling .pioenvs/liligo-t5-47-epaper-plus/src/esphome/components/t547/t547.cpp.o
Compiling .pioenvs/liligo-t5-47-epaper-plus/src/esphome/core/color.cpp.o
Compiling .pioenvs/liligo-t5-47-epaper-plus/src/esphome/core/component.cpp.o
Compiling .pioenvs/liligo-t5-47-epaper-plus/src/esphome/core/component_iterator.cpp.o
In file included from src/esphome/components/t547/t547.cpp:1:
src/esphome/components/t547/t547.h:8:10: fatal error: epd_driver.h: No such file or directory

**************************************************************************
* Looking for epd_driver.h dependency? Check our library registry!
*
* CLI  > platformio lib search "header:epd_driver.h"
* Web  > https://registry.platformio.org/search?q=header:%1B%5Bm%1B%5BKepd_driver.h
*
**************************************************************************

    8 | #include "epd_driver.h"
      |          ^~~~~~~~~~~~~~
compilation terminated.
*** [.pioenvs/liligo-t5-47-epaper-plus/src/esphome/components/t547/t547.cpp.o] Error 1
========================= [FAILED] Took 64.35 seconds =========================

```
### ❌ FAIL - esp_bme680_7_bsec_filter_c3_v3.yaml (600.0s)
```
TIMEOUT
```
### ❌ FAIL - esp32-t5-75.yaml (3.0s)
```
INFO ESPHome 2025.11.4
INFO Reading configuration /config/esp32-t5-75.yaml...
Failed config

sensor.mqtt_subscribe: [source /config/esp32-t5-75.yaml:78]
  
  Duplicate sensor entity with name 'Data from topic' found. Conflicts with entity 'Data from topic' (id: lr_temp) from component 'sensor.mqtt_subscribe'. Each entity on a device must have a unique name within its platform.
  platform: mqtt_subscribe
  name: Data from topic
  id: termostat_temp
  topic: esp_bme680_2/sensor/bme680_temperature/state
  filters: 
    - throttle: 60s
  on_value: 
    then: 

```
### ❌ FAIL - toshiba-tcc-link.yaml (3.9s)
```
INFO ESPHome 2025.11.4
INFO Reading configuration /config/toshiba-tcc-link.yaml...
ERROR Unable to load component tcc_link.climate:
Traceback (most recent call last):
  File "/esphome/esphome/loader.py", line 199, in _lookup_module
    module = importlib.import_module(f"esphome.components.{domain}")
             ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  File "/usr/local/lib/python3.12/importlib/__init__.py", line 90, in import_module
    return _bootstrap._gcd_import(name[level:], package, level)
           ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  File "<frozen importlib._bootstrap>", line 1387, in _gcd_import
  File "<frozen importlib._bootstrap>", line 1360, in _find_and_load
  File "<frozen importlib._bootstrap>", line 1331, in _find_and_load_unlocked
  File "<frozen importlib._bootstrap>", line 935, in _load_unlocked
  File "<frozen importlib._bootstrap_external>", line 999, in exec_module
  File "<frozen importlib._bootstrap>", line 488, in _call_with_frames_removed
  File "/config/.esphome/external_components/16a00c19/components/tcc_link/climate.py", line 44, in <module>
    CONFIG_SCHEMA = climate.CLIMATE_SCHEMA.extend(
                    ^^^^^^^^^^^^^^^^^^^^^^
AttributeError: module 'esphome.components.climate' has no attribute 'CLIMATE_SCHEMA'. Did you mean: '_CLIMATE_SCHEMA'?
Failed config

Platform not found: 'climate.tcc_link'


```
### ❌ FAIL - clock.yaml (1.7s)
```
INFO ESPHome 2025.11.4
INFO Reading configuration /config/clock.yaml...
Failed config

'esphome' section missing from configuration. Please make sure your configuration has an 'esphome:' line in it.


```
### ❌ FAIL - esp32_ikea_bme680_bsec_filter.yaml (101.1s)
```
.2.1
 + requests==2.32.5
 + requests-file==3.0.1
 + requests-toolbelt==1.0.0
 + ruamel-yaml==0.18.16
 + ruamel-yaml-clib==0.2.15
 + tqdm==4.67.1
 + truststore==0.10.4
 + typing-extensions==4.15.0
 + typing-inspection==0.4.2
 + urllib3==1.26.20
Reading CMake configuration...
Dependency Graph
|-- Networking @ 3.3.2
|-- ESP32 Async UDP @ 3.3.2
|-- DNSServer @ 3.3.2
|-- WiFi @ 3.3.2
|-- ESPmDNS @ 3.3.2
|-- Update @ 3.3.2
|-- Wire @ 3.3.2
|-- ArduinoJson @ 7.4.2
|-- SPI @ 3.3.2
|-- BSEC Software Library @ 1.6.1480
Building .pioenvs/esp32_bme680_3/bootloader.bin
Creating ESP32 image...
Successfully created ESP32 image.
Linking .pioenvs/esp32_bme680_3/firmware.elf
                            Memory Type Usage Summary                             
┏━━━━━━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━┳━━━━━━━━━━┳━━━━━━━━━━━━━━━━┳━━━━━━━━━━━━━━━┓
┃ Memory Type/Section ┃ Used [bytes] ┃ Used [%] ┃ Remain [bytes] ┃ Total [bytes] ┃
┡━━━━━━━━━━━━━━━━━━━━━╇━━━━━━━━━━━━━━╇━━━━━━━━━━╇━━━━━━━━━━━━━━━━╇━━━━━━━━━━━━━━━┩
│ Flash Code          │       856072 │          │                │               │
│    .text            │       856072 │          │                │               │
│ Flash Data          │       162476 │          │                │               │
│    .rodata          │       162220 │          │                │               │
│    .appdesc         │          256 │          │                │               │
│ IRAM                │        95095 │    72.55 │          35977 │        131072 │
│    .text            │        94067 │    71.77 │                │               │
│    .vectors         │         1028 │     0.78 │                │               │
│ DRAM                │        44548 │    24.65 │         136188 │        180736 │
│    .bss             │        28416 │    15.72 │                │               │
│    .data            │        16132 │     8.93 │                │               │
└─────────────────────┴──────────────┴──────────┴────────────────┴───────────────┘
Total image size: 1129775 bytes (.bin may be padded larger)
Note: The reported total sizes may be smaller than those in the technical reference manual due to reserved memory and application configuration. The total flash size available for the application is not included by default, as it cannot be reliably determined due to the presence of other data like the bootloader, partition table, and application partition size.
RAM:   [=         ]  13.6% (used 44548 bytes from 327680 bytes)
Flash: [======    ]  61.6% (used 1129519 bytes from 1835008 bytes)
Building .pioenvs/esp32_bme680_3/firmware.bin
Traceback (most recent call last):
  File "/root/.platformio/penv/bin/esptool", line 4, in <module>
    from esptool.__init__ import _main
  File "/root/.platformio/penv/bin/esptool.py", line 34, in <module>
    import esptool
ModuleNotFoundError: No module named 'esptool'
*** [.pioenvs/esp32_bme680_3/firmware.bin] Error 1
========================= [FAILED] Took 96.12 seconds =========================

```
### ❌ FAIL - esp32-pasco2.yaml (67.6s)
```
orced_calibration(uint16_t)':
src/esphome/components/pasco2/pasco2.cpp:304:24: error: no matching function for call to 'esphome::pasco2::PASCO2Component::write_byte(const uint8_t&, int, bool)'
  304 |   if (!this->write_byte(XENSIV_PASCO2_REG_MEAS_CFG,
      |        ~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~~
  305 |                         XENSIV_PASCO2_REG_MEAS_CFG_OP_MODE_IDLE | XENSIV_PASCO2_REG_MEAS_CFG_BOC_CFG_FORCE |
      |                         ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  306 |                             XENSIV_PASCO2_REG_MEAS_CFG_PWM_OUTEN_EN,
      |                             ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  307 |                         true)) {
      |                         ~~~~~
src/esphome/components/i2c/i2c.h:266:8: note: candidate: 'bool esphome::i2c::I2CDevice::write_byte(uint8_t, uint8_t) const'
  266 |   bool write_byte(uint8_t a_register, uint8_t data) const { return write_bytes(a_register, &data, 1); }
      |        ^~~~~~~~~~
src/esphome/components/i2c/i2c.h:266:8: note:   candidate expects 2 arguments, 3 provided
src/esphome/components/pasco2/pasco2.cpp: In member function 'bool esphome::pasco2::PASCO2Component::start_measurement_()':
src/esphome/components/pasco2/pasco2.cpp:366:24: error: no matching function for call to 'esphome::pasco2::PASCO2Component::write_byte(const uint8_t&, int, bool)'
  366 |   if (!this->write_byte(XENSIV_PASCO2_REG_MEAS_CFG,
      |        ~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~~
  367 |                         XENSIV_PASCO2_REG_MEAS_CFG_OP_MODE_IDLE | XENSIV_PASCO2_REG_MEAS_CFG_BOC_CFG_ENABLE, true)) {
      |                         ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
src/esphome/components/i2c/i2c.h:266:8: note: candidate: 'bool esphome::i2c::I2CDevice::write_byte(uint8_t, uint8_t) const'
  266 |   bool write_byte(uint8_t a_register, uint8_t data) const { return write_bytes(a_register, &data, 1); }
      |        ^~~~~~~~~~
src/esphome/components/i2c/i2c.h:266:8: note:   candidate expects 2 arguments, 3 provided
src/esphome/components/pasco2/pasco2.cpp:379:26: error: no matching function for call to 'esphome::pasco2::PASCO2Component::write_byte(const uint8_t&, uint8_t&, bool)'
  379 |     if (!this->write_byte(XENSIV_PASCO2_REG_MEAS_CFG, measurement_command, true)) {
      |          ~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
src/esphome/components/i2c/i2c.h:266:8: note: candidate: 'bool esphome::i2c::I2CDevice::write_byte(uint8_t, uint8_t) const'
  266 |   bool write_byte(uint8_t a_register, uint8_t data) const { return write_bytes(a_register, &data, 1); }
      |        ^~~~~~~~~~
src/esphome/components/i2c/i2c.h:266:8: note:   candidate expects 2 arguments, 3 provided
*** [.pioenvs/esp32-pasco2/src/esphome/components/pasco2/pasco2.cpp.o] Error 1
========================= [FAILED] Took 63.42 seconds =========================

```
### ❌ FAIL - air_sensor_demo_wemos.yaml (2.0s)
```
INFO ESPHome 2025.11.4
INFO Reading configuration /config/air_sensor_demo_wemos.yaml...
INFO Cloning https://github.com/sermayoral/esphome.git@pm1006
Failed config

external_components: [source /config/air_sensor_demo_wemos.yaml:29]
  - 
    could not read Username for 'https://github.com': No such device or address.
    source: github://sermayoral/esphome@pm1006
    components: 
      - pm1006

```
### ❌ FAIL - esp_ikea_bme680_2_bsec_filter.yaml (600.0s)
```
TIMEOUT
```
### ❌ FAIL - esp_bme680_1_bsec_filter_v2.yaml (186.4s)
```
ponents/esphome/ota/ota_esphome.cpp.o
Compiling .pioenvs/esp_bme680_1/src/esphome/components/i2c/i2c.cpp.o
Compiling .pioenvs/esp_bme680_1/src/esphome/components/i2c/i2c_bus_arduino.cpp.o
Compiling .pioenvs/esp_bme680_1/src/esphome/components/json/json_util.cpp.o
Compiling .pioenvs/esp_bme680_1/src/esphome/components/logger/logger.cpp.o
Compiling .pioenvs/esp_bme680_1/src/esphome/components/logger/logger_esp8266.cpp.o
Compiling .pioenvs/esp_bme680_1/src/esphome/components/md5/md5.cpp.o
Compiling .pioenvs/esp_bme680_1/src/esphome/components/mdns/mdns_component.cpp.o
Compiling .pioenvs/esp_bme680_1/src/esphome/components/mdns/mdns_esp8266.cpp.o
Compiling .pioenvs/esp_bme680_1/src/esphome/components/mqtt/custom_mqtt_device.cpp.o
Compiling .pioenvs/esp_bme680_1/src/esphome/components/mqtt/mqtt_alarm_control_panel.cpp.o
Compiling .pioenvs/esp_bme680_1/src/esphome/components/mqtt/mqtt_binary_sensor.cpp.o
Compiling .pioenvs/esp_bme680_1/src/esphome/components/mqtt/mqtt_button.cpp.o
Compiling .pioenvs/esp_bme680_1/src/esphome/components/mqtt/mqtt_client.cpp.o
Compiling .pioenvs/esp_bme680_1/src/esphome/components/mqtt/mqtt_climate.cpp.o
Compiling .pioenvs/esp_bme680_1/src/esphome/components/mqtt/mqtt_component.cpp.o
Compiling .pioenvs/esp_bme680_1/src/esphome/components/mqtt/mqtt_cover.cpp.o
Compiling .pioenvs/esp_bme680_1/src/esphome/components/mqtt/mqtt_date.cpp.o
Compiling .pioenvs/esp_bme680_1/src/esphome/components/mqtt/mqtt_datetime.cpp.o
Compiling .pioenvs/esp_bme680_1/src/esphome/components/mqtt/mqtt_event.cpp.o
Compiling .pioenvs/esp_bme680_1/src/esphome/components/mqtt/mqtt_fan.cpp.o
Compiling .pioenvs/esp_bme680_1/src/esphome/components/mqtt/mqtt_light.cpp.o
Compiling .pioenvs/esp_bme680_1/src/esphome/components/mqtt/mqtt_lock.cpp.o
Compiling .pioenvs/esp_bme680_1/src/esphome/components/mqtt/mqtt_number.cpp.o
Compiling .pioenvs/esp_bme680_1/src/esphome/components/mqtt/mqtt_select.cpp.o
Compiling .pioenvs/esp_bme680_1/src/esphome/components/mqtt/mqtt_sensor.cpp.o
Compiling .pioenvs/esp_bme680_1/src/esphome/components/mqtt/mqtt_switch.cpp.o
Compiling .pioenvs/esp_bme680_1/src/esphome/components/mqtt/mqtt_text.cpp.o
Compiling .pioenvs/esp_bme680_1/src/esphome/components/mqtt/mqtt_text_sensor.cpp.o
Compiling .pioenvs/esp_bme680_1/src/esphome/components/mqtt/mqtt_time.cpp.o
Compiling .pioenvs/esp_bme680_1/src/esphome/components/mqtt/mqtt_update.cpp.o
Compiling .pioenvs/esp_bme680_1/src/esphome/components/mqtt/mqtt_valve.cpp.o
Compiling .pioenvs/esp_bme680_1/src/esphome/components/network/util.cpp.o
Compiling .pioenvs/esp_bme680_1/src/esphome/components/ota/ota_backend.cpp.o
Compiling .pioenvs/esp_bme680_1/src/esphome/components/ota/ota_backend_arduino_esp8266.cpp.o
*** [.pioenvs/esp_bme680_1/src/esphome/components/restart/button/restart_button.cpp.o] /config/.esphome/build/esp_bme680_1/src/esphome/components/restart/button/restart_button.cpp: No such file or directory
========================= [FAILED] Took 171.81 seconds =========================

```
### ❌ FAIL - lilygo.yaml (2.4s)
```
INFO ESPHome 2025.11.4
INFO Reading configuration /config/lilygo.yaml...
ERROR Error while reading config: Invalid YAML syntax:

mapping values are not allowed here
  in "/config/lilygo.yaml", line 28, column 13

```
### ❌ FAIL - esp32s3-ttgo-d-2-test-2.yaml (3.5s)
```
INFO ESPHome 2025.11.4
INFO Reading configuration /config/esp32s3-ttgo-d-2-test-2.yaml...
Failed config

esp32: [source /config/esp32s3-ttgo-d-2-test-2.yaml:27]
  
  Only Arduino 3.0+ is supported.
  variant: esp32s3
  board: esp32-s3-devkitc-1
  framework: 
    type: arduino
    version: 2.0.3
    platform_version: 5.1.1

```
### ❌ FAIL - esp_bme680_6_bsec_filter_v2.yaml (321.5s)
```
me680_6/src/esphome/core/util.cpp.o
Compiling .pioenvs/esp_bme680_6/src/main.cpp.o
Assembler messages:
Fatal error: can't create .pioenvs/esp_bme680_6/src/esphome/core/helpers.cpp.o: No such file or directory
*** [.pioenvs/esp_bme680_6/src/esphome/core/helpers.cpp.o] Error 1
Assembler messages:
Fatal error: can't create .pioenvs/esp_bme680_6/src/esphome/core/scheduler.cpp.o: No such file or directory
*** [.pioenvs/esp_bme680_6/src/esphome/core/scheduler.cpp.o] Error 1
Assembler messages:
Fatal error: can't create .pioenvs/esp_bme680_6/src/esphome/core/util.cpp.o: No such file or directory
*** [.pioenvs/esp_bme680_6/src/esphome/core/util.cpp.o] Error 1
Assembler messages:
Fatal error: can't create .pioenvs/esp_bme680_6/src/main.cpp.o: No such file or directory
*** [.pioenvs/esp_bme680_6/src/main.cpp.o] Error 1
FileNotFoundError: [Errno 2] No such file or directory: '/config/.esphome/build/esp_bme680_6/.pioenvs/esp_bme680_6/.sconsign312.tmp':
  File "/config/.esphome/platformio/packages/tool-scons/scons-local-4.8.1/SCons/Script/Main.py", line 1508:
    _exec_main(parser, values)
  File "/config/.esphome/platformio/packages/tool-scons/scons-local-4.8.1/SCons/Script/Main.py", line 1462:
    _main(parser)
  File "/config/.esphome/platformio/packages/tool-scons/scons-local-4.8.1/SCons/Script/Main.py", line 1201:
    nodes = _build_targets(fs, options, targets, target_top)
  File "/config/.esphome/platformio/packages/tool-scons/scons-local-4.8.1/SCons/Script/Main.py", line 1396:
    jobs.run(postfunc = jobs_postfunc)
  File "/config/.esphome/platformio/packages/tool-scons/scons-local-4.8.1/SCons/Taskmaster/Job.py", line 114:
    postfunc()
  File "/config/.esphome/platformio/packages/tool-scons/scons-local-4.8.1/SCons/Script/Main.py", line 1393:
    SCons.SConsign.write()
  File "/config/.esphome/platformio/packages/tool-scons/scons-local-4.8.1/SCons/SConsign.py", line 131:
    syncmethod()
  File "/config/.esphome/platformio/packages/tool-scons/scons-local-4.8.1/SCons/dblite.py", line 195:
    with self._open(self._tmp_name, "wb", opener=self.opener) as f:
  File "/config/.esphome/platformio/packages/tool-scons/scons-local-4.8.1/SCons/dblite.py", line 177:
    return os.open(path, flags, mode=self._mode)
Exception ignored in: <function _Dblite.__del__ at 0x7f8bb2191b20>
Traceback (most recent call last):
  File "/config/.esphome/platformio/packages/tool-scons/scons-local-4.8.1/SCons/dblite.py", line 184, in __del__
  File "/config/.esphome/platformio/packages/tool-scons/scons-local-4.8.1/SCons/dblite.py", line 181, in close
  File "/config/.esphome/platformio/packages/tool-scons/scons-local-4.8.1/SCons/dblite.py", line 195, in sync
  File "/config/.esphome/platformio/packages/tool-scons/scons-local-4.8.1/SCons/dblite.py", line 177, in opener
FileNotFoundError: [Errno 2] No such file or directory: '/config/.esphome/build/esp_bme680_6/.pioenvs/esp_bme680_6/.sconsign312.tmp'
========================= [FAILED] Took 316.48 seconds =========================

```
### ❌ FAIL - esp32-t5 4.7.yaml (55.9s)
```
: declared here
  240 | void rtc_clk_apll_enable(bool enable);
      |      ^~~~~~~~~~~~~~~~~~~
*** [.pioenvs/esp32-t5-47/src/esphome/components/t547/ed097oc4.c.o] Error 1
*** [.pioenvs/esp32-t5-47/src/esphome/components/t547/i2s_data_bus.c.o] Error 1
Compiling .pioenvs/esp32-t5-47/src/esphome/components/t547/rmt_pulse.c.o
In file included from src/esphome/components/t547/rmt_pulse.c:3:
/config/.esphome/platformio/packages/framework-espidf/components/driver/deprecated/driver/rmt.h:18:2: warning: #warning "The legacy RMT driver is deprecated, please use driver/rmt_tx.h and/or driver/rmt_rx.h" [-Wcpp]
   18 | #warning "The legacy RMT driver is deprecated, please use driver/rmt_tx.h and/or driver/rmt_rx.h"
      |  ^~~~~~~
src/esphome/components/t547/rmt_pulse.c: In function 'rmt_pulse_init':
src/esphome/components/t547/rmt_pulse.c:55:5: error: implicit declaration of function 'rmt_ll_enable_tx_end_interrupt'; did you mean 'rmt_ll_enable_interrupt'? [-Wimplicit-function-declaration]
   55 |     rmt_ll_enable_tx_end_interrupt(&RMT, row_rmt_config.channel, true);
      |     ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      |     rmt_ll_enable_interrupt
src/esphome/components/t547/rmt_pulse.c: At top level:
src/esphome/components/t547/rmt_pulse.c:63:1: warning: ignoring attribute 'section (".iram1.4")' because it conflicts with previous 'section (".iram1.2")' [-Wattributes]
   63 | {
      | ^
In file included from src/esphome/components/t547/rmt_pulse.c:1:
src/esphome/components/t547/rmt_pulse.h:42:16: note: previous declaration here
   42 | void IRAM_ATTR pulse_ckv_ticks(uint16_t high_time_us, uint16_t low_time_us,
      |                ^~~~~~~~~~~~~~~
src/esphome/components/t547/rmt_pulse.c: In function 'pulse_ckv_ticks':
src/esphome/components/t547/rmt_pulse.c:67:11: error: 'RMTMEM' undeclared (first use in this function)
   67 |         &(RMTMEM.chan[row_rmt_config.channel].data32[0]);
      |           ^~~~~~
src/esphome/components/t547/rmt_pulse.c:67:11: note: each undeclared identifier is reported only once for each function it appears in
src/esphome/components/t547/rmt_pulse.c: At top level:
src/esphome/components/t547/rmt_pulse.c:90:1: warning: ignoring attribute 'section (".iram1.5")' because it conflicts with previous 'section (".iram1.0")' [-Wattributes]
   90 | {
      | ^
src/esphome/components/t547/rmt_pulse.h:27:16: note: previous declaration here
   27 | void IRAM_ATTR pulse_ckv_us(uint16_t high_time_us, uint16_t low_time_us,
      |                ^~~~~~~~~~~~
src/esphome/components/t547/rmt_pulse.c:95:1: warning: ignoring attribute 'section (".iram1.6")' because it conflicts with previous 'section (".iram1.1")' [-Wattributes]
   95 | {
      | ^
src/esphome/components/t547/rmt_pulse.h:32:16: note: previous declaration here
   32 | bool IRAM_ATTR rmt_busy();
      |                ^~~~~~~~
*** [.pioenvs/esp32-t5-47/src/esphome/components/t547/rmt_pulse.c.o] Error 1
========================= [FAILED] Took 48.25 seconds =========================

```
### ❌ FAIL - esp32_ProBreeze.yaml (600.0s)
```
TIMEOUT
```
### ❌ FAIL - esp32-t5.yaml (4.1s)
```
INFO ESPHome 2025.11.4
INFO Reading configuration /config/esp32-t5.yaml...
Failed config

sensor.mqtt_subscribe: [source /config/esp32-t5.yaml:63]
  
  Duplicate sensor entity with name 'Data from topic' found. Conflicts with entity 'Data from topic' (id: lr_temp) from component 'sensor.mqtt_subscribe'. Each entity on a device must have a unique name within its platform.
  platform: mqtt_subscribe
  name: Data from topic
  id: termostat_temp
  topic: esp_bme680_2/sensor/bme680_temperature/state
  on_value: 
    then: 
      - component.update: epaper

```
### ❌ FAIL - esp_bme680_6_bsec_filter_c3_v3.yaml (600.0s)
```
TIMEOUT
```
### ❌ FAIL - esp32-t5-jira-copilot.yaml (115.2s)
```
requests-toolbelt==1.0.0
 + ruamel-yaml==0.18.16
 + ruamel-yaml-clib==0.2.15
 + tqdm==4.67.1
 + truststore==0.10.4
 + typing-extensions==4.15.0
 + typing-inspection==0.4.2
 + urllib3==1.26.20
Reading CMake configuration...
Dependency Graph
|-- ArduinoJson @ 7.4.2
Compiling .pioenvs/coffeemaker-lnz-b01-2/src/main.cpp.o
Archiving .pioenvs/coffeemaker-lnz-b01-2/esp-idf/console/libconsole.a
Compiling .pioenvs/coffeemaker-lnz-b01-2/cxx/cxx_guards.cpp.o
Compiling .pioenvs/coffeemaker-lnz-b01-2/cxx/cxx_init.cpp.o
Compiling .pioenvs/coffeemaker-lnz-b01-2/driver/deprecated/adc_legacy.c.o
Compiling .pioenvs/coffeemaker-lnz-b01-2/driver/deprecated/adc_dma_legacy.c.o
Compiling .pioenvs/coffeemaker-lnz-b01-2/driver/deprecated/dac_common_legacy.c.o
Archiving .pioenvs/coffeemaker-lnz-b01-2/esp-idf/cxx/libcxx.a
Compiling .pioenvs/coffeemaker-lnz-b01-2/driver/deprecated/esp32/dac_legacy.c.o
Compiling .pioenvs/coffeemaker-lnz-b01-2/driver/deprecated/timer_legacy.c.o
Compiling .pioenvs/coffeemaker-lnz-b01-2/driver/i2c/i2c.c.o
Compiling .pioenvs/coffeemaker-lnz-b01-2/driver/deprecated/i2s_legacy.c.o
Compiling .pioenvs/coffeemaker-lnz-b01-2/driver/deprecated/mcpwm_legacy.c.o
Compiling .pioenvs/coffeemaker-lnz-b01-2/driver/deprecated/pcnt_legacy.c.o
Compiling .pioenvs/coffeemaker-lnz-b01-2/driver/deprecated/rmt_legacy.c.o
Compiling .pioenvs/coffeemaker-lnz-b01-2/driver/deprecated/sigma_delta_legacy.c.o
/config/esp32-t5-jira-copilot.yaml: In lambda function:
/config/esp32-t5-jira-copilot.yaml:153:43: error: invalid operands of types 'float' and 'const char [8]' to binary 'operator=='
  153 |         if (id(coffeemaker_error_report).state == "Problem") {
      |           ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ^~ ~~~~~~~~~
      |                                     |        |
      |                                     float    const char [8]
/config/esp32-t5-jira-copilot.yaml: In lambda function:
/config/esp32-t5-jira-copilot.yaml:28:9: error: 'ESPHomeMQTTClient' has not been declared
   28 |             ESPHomeMQTTClient::get()->publish("coffeemaker_lnz_b01_2/sensor/wakeup_report/state", "WAKEUP");
      |         ^   ~~~~~~~~~~~~~
/config/esp32-t5-jira-copilot.yaml: In lambda function:
/config/esp32-t5-jira-copilot.yaml:7:11: error: 'digitalRead' was not declared in this scope
    7 |           if (digitalRead(39) == HIGH) {
      |           ^~~~~~~~~~~
/config/esp32-t5-jira-copilot.yaml:7:30: error: 'HIGH' was not declared in this scope
    7 |           if (digitalRead(39) == HIGH) {
      |                              ^~~~
Compiling .pioenvs/coffeemaker-lnz-b01-2/driver/touch_sensor/touch_sensor_common.c.o
Compiling .pioenvs/coffeemaker-lnz-b01-2/driver/touch_sensor/esp32/touch_sensor.c.o
Compiling .pioenvs/coffeemaker-lnz-b01-2/driver/twai/twai.c.o
Compiling .pioenvs/coffeemaker-lnz-b01-2/driver/deprecated/adc_i2s_deprecated.c.o
*** [.pioenvs/coffeemaker-lnz-b01-2/src/main.cpp.o] Error 1
========================= [FAILED] Took 97.65 seconds =========================

```
### ❌ FAIL - esp_trafficlight_1.yaml (79.3s)
```
light1/ArduinoJson/src/ArduinoJson.h:9,
                 from src/esphome/components/json/json_util.h:12,
                 from src/esphome.h:10,
                 from src/main.cpp:3:
.piolibdeps/esptrafficlight1/ArduinoJson/src/ArduinoJson/Object/JsonObjectConst.hpp:85:8: note: declared here
   85 |   bool containsKey(TChar* key) const {
      |        ^~~~~~~~~~~
/config/esp_trafficlight_1.yaml:67:33: error: 'class esphome::light::AddressableLightState' has no member named 'get_controller'
   67 |                             id(my_light)->get_controller()->leds()[index].setRGB(r, g, b);
      |                                 ^~~~~~~~~~~~~~
/config/esp_trafficlight_1.yaml:68:33: error: 'class esphome::light::AddressableLightState' has no member named 'get_controller'
   68 |                             id(my_light)->get_controller()->leds()[index].maximizeBrightness(brightness);
      |                                 ^~~~~~~~~~~~~~
/config/esp_trafficlight_1.yaml:69:33: error: 'class esphome::light::AddressableLightState' has no member named 'schedule_show'
   69 |                             id(my_light)->schedule_show();
      |                                 ^~~~~~~~~~~~~
/config/esp_trafficlight_1.yaml:72:33: error: 'class esphome::light::AddressableLightState' has no member named 'get_controller'
   72 |                             id(my_light)->get_controller()->leds()[index].setRGB(0, 0, 0);
      |                                 ^~~~~~~~~~~~~~
/config/esp_trafficlight_1.yaml:73:33: error: 'class esphome::light::AddressableLightState' has no member named 'schedule_show'
   73 |                             id(my_light)->schedule_show();
      |                                 ^~~~~~~~~~~~~
In file included from src/esphome/core/component.h:8,
                 from src/esphome/components/captive_portal/captive_portal.h:11,
                 from src/esphome.h:4,
                 from src/main.cpp:3:
/config/esp_trafficlight_1.yaml:79:106: error: 'class esphome::light::AddressableLightState' has no member named 'size'
   79 |                     ESP_LOGD("on_json_message", "INVALID led_index out of bounds 0 < led_index < %d",id(my_light)->size());
      |                                                                                                          ^~~~
src/esphome/core/log.h:99:100: note: in definition of macro 'esph_log_d'
   99 |   ::esphome::esp_log_printf_(ESPHOME_LOG_LEVEL_DEBUG, tag, __LINE__, ESPHOME_LOG_FORMAT(format), ##__VA_ARGS__)
      |                                                                                                    ^~~~~~~~~~~
/config/esp_trafficlight_1.yaml:79:15: note: in expansion of macro 'ESP_LOGD'
   79 |                     ESP_LOGD("on_json_message", "INVALID led_index out of bounds 0 < led_index < %d",id(my_light)->size());
      |               ^     ~~
*** [.pioenvs/esptrafficlight1/src/main.cpp.o] Error 1
========================= [FAILED] Took 64.83 seconds =========================

```
### ❌ FAIL - esp32s3-ttgo-d-2.yaml (5.4s)
```
INFO ESPHome 2025.11.4
INFO Reading configuration /config/esp32s3-ttgo-d-2.yaml...
WARNING GPIO0 is a strapping PIN and should only be used for I/O with care.
Attaching external pullup/down resistors to strapping pins can cause unexpected failures.
See https://esphome.io/guides/faq.html#why-am-i-getting-a-warning-about-strapping-pins
Failed config

psram: None
  {}

ESP32S3 requires PSRAM mode selection; one of quad, octal
Selection of the wrong mode for the board will cause a runtime failure to initialise PSRAM



```
### ❌ FAIL - esp32_ikea_bme680_bsec_filter_display_v3.yaml (525.5s)
```
/components/mqtt/mqtt_event.cpp.o: No such file or directory
*** [.pioenvs/esp32_bme680_3/src/esphome/components/mqtt/mqtt_event.cpp.o] Error 1
Assembler messages:
Fatal error: can't create .pioenvs/esp32_bme680_3/src/esphome/components/mqtt/mqtt_fan.cpp.o: No such file or directory
*** [.pioenvs/esp32_bme680_3/src/esphome/components/mqtt/mqtt_fan.cpp.o] Error 1
Assembler messages:
Fatal error: can't create .pioenvs/esp32_bme680_3/src/esphome/components/mqtt/mqtt_light.cpp.o: No such file or directory
*** [.pioenvs/esp32_bme680_3/src/esphome/components/mqtt/mqtt_light.cpp.o] Error 1
Assembler messages:
Fatal error: can't create .pioenvs/esp32_bme680_3/src/esphome/components/mqtt/mqtt_lock.cpp.o: No such file or directory
*** [.pioenvs/esp32_bme680_3/src/esphome/components/mqtt/mqtt_lock.cpp.o] Error 1
FileNotFoundError: [Errno 2] No such file or directory: '/config/.esphome/build/esp32_bme680_3/.pioenvs/esp32_bme680_3/.sconsign312.tmp':
  File "/config/.esphome/platformio/packages/tool-scons/scons-local-4.8.1/SCons/Script/Main.py", line 1508:
    _exec_main(parser, values)
  File "/config/.esphome/platformio/packages/tool-scons/scons-local-4.8.1/SCons/Script/Main.py", line 1462:
    _main(parser)
  File "/config/.esphome/platformio/packages/tool-scons/scons-local-4.8.1/SCons/Script/Main.py", line 1201:
    nodes = _build_targets(fs, options, targets, target_top)
  File "/config/.esphome/platformio/packages/tool-scons/scons-local-4.8.1/SCons/Script/Main.py", line 1396:
    jobs.run(postfunc = jobs_postfunc)
  File "/config/.esphome/platformio/packages/tool-scons/scons-local-4.8.1/SCons/Taskmaster/Job.py", line 114:
    postfunc()
  File "/config/.esphome/platformio/packages/tool-scons/scons-local-4.8.1/SCons/Script/Main.py", line 1393:
    SCons.SConsign.write()
  File "/config/.esphome/platformio/packages/tool-scons/scons-local-4.8.1/SCons/SConsign.py", line 131:
    syncmethod()
  File "/config/.esphome/platformio/packages/tool-scons/scons-local-4.8.1/SCons/dblite.py", line 195:
    with self._open(self._tmp_name, "wb", opener=self.opener) as f:
  File "/config/.esphome/platformio/packages/tool-scons/scons-local-4.8.1/SCons/dblite.py", line 177:
    return os.open(path, flags, mode=self._mode)
Exception ignored in: <function _Dblite.__del__ at 0x7f09aecdd9e0>
Traceback (most recent call last):
  File "/config/.esphome/platformio/packages/tool-scons/scons-local-4.8.1/SCons/dblite.py", line 184, in __del__
  File "/config/.esphome/platformio/packages/tool-scons/scons-local-4.8.1/SCons/dblite.py", line 181, in close
  File "/config/.esphome/platformio/packages/tool-scons/scons-local-4.8.1/SCons/dblite.py", line 195, in sync
  File "/config/.esphome/platformio/packages/tool-scons/scons-local-4.8.1/SCons/dblite.py", line 177, in opener
FileNotFoundError: [Errno 2] No such file or directory: '/config/.esphome/build/esp32_bme680_3/.pioenvs/esp32_bme680_3/.sconsign312.tmp'
========================= [FAILED] Took 515.73 seconds =========================

```
### ❌ FAIL - esp32-t5-4.7-example.yaml (5.7s)
```
INFO ESPHome 2025.11.4
INFO Reading configuration /config/esp32-t5-4.7-example.yaml...
INFO Updating https://github.com/grapefrukt/esphome-components.git@None
WARNING API 'password' authentication has been deprecated since May 2022 and will be removed in version 2026.1.0. Please migrate to the 'encryption' configuration. See https://esphome.io/components/api.html#configuration-variables
INFO Detected timezone 'Etc/UTC'
WARNING `attenuation: 11db` is deprecated, use `attenuation: 12db` instead
WARNING Captive portal is enabled but no WiFi AP is configured. The captive portal will not be accessible. Add 'ap:' to your WiFi configuration to enable the captive portal.
Failed config

deep_sleep: [source /config/esp32-t5-4.7-example.yaml:31]
  
  Pin 39 is used in multiple places.
  run_duration: 5min
  sleep_duration: 20min
  id: deep_sleep_1
  esp32_ext1_wakeup: 
    pins: 
      - number: 39
        mode: 
          input: True
          output: False
          open_drain: False
          pullup: False
          pulldown: False
binary_sensor.gpio: [source /config/esp32-t5-4.7-example.yaml:129]
  
  Pin 39 is used in multiple places.
  platform: gpio
  pin: 
    number: 39
    inverted: True
    mode: 
      input: True
      output: False
      open_drain: False
      pullup: False
      pulldown: False
    ignore_pin_validation_error: False
    ignore_strapping_warning: False

```
### ❌ FAIL - esp32-regenmengensensor.yaml (3.3s)
```
INFO ESPHome 2025.11.4
INFO Reading configuration /config/esp32-regenmengensensor.yaml...
Failed config

sensor.esp32_hall: [source /config/esp32-regenmengensensor.yaml:37]
  
  The esp32_hall component has been removed as of ESPHome 2025.7.0. See https://github.com/esphome/esphome/pull/9117 for details.
  platform: esp32_hall
  name: ESP32 Hall Sensor
  update_interval: 1s

```
### ❌ FAIL - esp_bme680_5_bsec_filter_c3_v3.yaml (600.0s)
```
TIMEOUT
```
### ❌ FAIL - esp32-t5-4.7-test.yaml (97.3s)
```
nents/esp_hw_support/port/esp32/include/soc/rtc.h:240:6: note: declared here
  240 | void rtc_clk_apll_enable(bool enable);
      |      ^~~~~~~~~~~~~~~~~~~
Compiling .pioenvs/esp-display-test/src/esphome/components/t547/rmt_pulse.c.o
*** [.pioenvs/esp-display-test/src/esphome/components/t547/i2s_data_bus.c.o] Error 1
In file included from src/esphome/components/t547/rmt_pulse.c:3:
/config/.esphome/platformio/packages/framework-espidf/components/driver/deprecated/driver/rmt.h:18:2: warning: #warning "The legacy RMT driver is deprecated, please use driver/rmt_tx.h and/or driver/rmt_rx.h" [-Wcpp]
   18 | #warning "The legacy RMT driver is deprecated, please use driver/rmt_tx.h and/or driver/rmt_rx.h"
      |  ^~~~~~~
src/esphome/components/t547/rmt_pulse.c: In function 'rmt_pulse_init':
src/esphome/components/t547/rmt_pulse.c:55:5: error: implicit declaration of function 'rmt_ll_enable_tx_end_interrupt'; did you mean 'rmt_ll_enable_interrupt'? [-Wimplicit-function-declaration]
   55 |     rmt_ll_enable_tx_end_interrupt(&RMT, row_rmt_config.channel, true);
      |     ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      |     rmt_ll_enable_interrupt
src/esphome/components/t547/rmt_pulse.c: At top level:
src/esphome/components/t547/rmt_pulse.c:63:1: warning: ignoring attribute 'section (".iram1.4")' because it conflicts with previous 'section (".iram1.2")' [-Wattributes]
   63 | {
      | ^
In file included from src/esphome/components/t547/rmt_pulse.c:1:
src/esphome/components/t547/rmt_pulse.h:42:16: note: previous declaration here
   42 | void IRAM_ATTR pulse_ckv_ticks(uint16_t high_time_us, uint16_t low_time_us,
      |                ^~~~~~~~~~~~~~~
src/esphome/components/t547/rmt_pulse.c: In function 'pulse_ckv_ticks':
src/esphome/components/t547/rmt_pulse.c:67:11: error: 'RMTMEM' undeclared (first use in this function)
   67 |         &(RMTMEM.chan[row_rmt_config.channel].data32[0]);
      |           ^~~~~~
src/esphome/components/t547/rmt_pulse.c:67:11: note: each undeclared identifier is reported only once for each function it appears in
src/esphome/components/t547/rmt_pulse.c: At top level:
src/esphome/components/t547/rmt_pulse.c:90:1: warning: ignoring attribute 'section (".iram1.5")' because it conflicts with previous 'section (".iram1.0")' [-Wattributes]
   90 | {
      | ^
src/esphome/components/t547/rmt_pulse.h:27:16: note: previous declaration here
   27 | void IRAM_ATTR pulse_ckv_us(uint16_t high_time_us, uint16_t low_time_us,
      |                ^~~~~~~~~~~~
src/esphome/components/t547/rmt_pulse.c:95:1: warning: ignoring attribute 'section (".iram1.6")' because it conflicts with previous 'section (".iram1.1")' [-Wattributes]
   95 | {
      | ^
src/esphome/components/t547/rmt_pulse.h:32:16: note: previous declaration here
   32 | bool IRAM_ATTR rmt_busy();
      |                ^~~~~~~~
*** [.pioenvs/esp-display-test/src/esphome/components/t547/rmt_pulse.c.o] Error 1
========================= [FAILED] Took 70.17 seconds =========================

```
### ❌ FAIL - esp_nfc_1.yaml (122.0s)
```
 2025.11.4
INFO Reading configuration /config/esp_nfc_1.yaml...
INFO Generating C++ source...
INFO Setting CONFIG_LWIP_MAX_SOCKETS to 12 (registered: captive_portal=4, mdns=2, mqtt=1, ota=1, socket.wake_loop_threadsafe=1, web_server=3)
INFO Compiling app... Build path: /config/.esphome/build/espnfc1
Processing espnfc1 (board: esp32dev; framework: arduino, espidf; platform: https://github.com/pioarduino/platform-espressif32/releases/download/55.03.31-2/platform-espressif32.zip)
--------------------------------------------------------------------------------
Created pioarduino Python virtual environment using uv: /root/.platformio/penv
Installed esptool from tl-install path: /config/.esphome/platformio/packages/tool-esptoolpy
INFO Package configuration completed successfully
INFO Package configuration completed successfully
HARDWARE: ESP32 240MHz, 320KB RAM, 4MB Flash
 - contrib-piohome @ 3.4.4 
 - framework-arduinoespressif32 @ 3.3.2 
 - framework-arduinoespressif32-libs @ 5.5.0+sha.129cd0d247 
 - framework-espidf @ 3.50501.0 (5.5.1) 
 - tool-cmake @ 4.0.3 
 - tool-esp-rom-elfs @ 2024.10.11 
 - tool-esptoolpy @ 5.1.0 
 - tool-mklittlefs @ 3.2.0 
 - tool-ninja @ 1.13.1 
 - tool-scons @ 4.40801.0 (4.8.1) 
 - toolchain-xtensa-esp-elf @ 14.2.0+20241119
Creating a new virtual environment for IDF Python dependencies using uv
Using Python 3.12.10 environment at: /root/.platformio/penv/.espidf-5.5.1
Installing ESP-IDF's Python dependencies with uv
Using Python 3.12.10 environment at: /root/.platformio/penv/.espidf-5.5.1
Resolved 27 packages in 1.45s
Downloading cryptography (4.0MiB)
Downloading pydantic-core (1.9MiB)
 Downloaded pydantic-core
 Downloaded cryptography
Prepared 17 packages in 1.42s
Installed 27 packages in 260ms
 + annotated-types==0.7.0
 + certifi==2025.11.12
 + cffi==2.0.0
 + charset-normalizer==3.4.4
 + click==8.3.1
 + colorama==0.4.6
 + cryptography==44.0.3
 + esp-idf-kconfig==2.5.0
 + idf-component-manager==2.4.3
 + idna==3.11
 + jsonref==1.1.0
 + pycparser==2.23
 + pydantic==2.11.10
 + pydantic-core==2.33.2
 + pydantic-settings==2.12.0
 + pyparsing==3.2.5
 + python-dotenv==1.2.1
 + requests==2.32.5
 + requests-file==3.0.1
 + requests-toolbelt==1.0.0
 + ruamel-yaml==0.18.16
 + ruamel-yaml-clib==0.2.15
 + tqdm==4.67.1
 + truststore==0.10.4
 + typing-extensions==4.15.0
 + typing-inspection==0.4.2
 + urllib3==1.26.20
Reading CMake configuration...
Dependency Graph
|-- Networking @ 3.3.2
|-- ESP32 Async UDP @ 3.3.2
|-- DNSServer @ 3.3.2
|-- WiFi @ 3.3.2
|-- ESPmDNS @ 3.3.2
|-- Update @ 3.3.2
|-- Wire @ 3.3.2
|-- ArduinoJson @ 7.4.2
|-- NeoPixelBus @ 2.8.0
Building .pioenvs/espnfc1/bootloader.bin
Traceback (most recent call last):
  File "/root/.platformio/penv/bin/esptool", line 4, in <module>
    from esptool.__init__ import _main
ModuleNotFoundError: No module named 'esptool.__init__'; 'esptool' is not a package
*** [.pioenvs/espnfc1/bootloader.bin] Error 1
========================= [FAILED] Took 113.81 seconds =========================

```
### ❌ FAIL - esp32_ikea_bme680_bsec_filter_display.yaml (600.0s)
```
TIMEOUT
```
### ❌ FAIL - esp_bme680_1_bsec_filter_c3_v3.yaml (600.0s)
```
TIMEOUT
```
### ❌ FAIL - esp_bme680_7_bsec_filter_v1.yaml (379.4s)
```
ts/mqtt/custom_mqtt_device.cpp.o
Compiling .pioenvs/esp_bme680_7/src/esphome/components/mqtt/mqtt_alarm_control_panel.cpp.o
Compiling .pioenvs/esp_bme680_7/src/esphome/components/mqtt/mqtt_binary_sensor.cpp.o
Compiling .pioenvs/esp_bme680_7/src/esphome/components/mqtt/mqtt_button.cpp.o
Compiling .pioenvs/esp_bme680_7/src/esphome/components/mqtt/mqtt_client.cpp.o
Compiling .pioenvs/esp_bme680_7/src/esphome/components/mqtt/mqtt_climate.cpp.o
Compiling .pioenvs/esp_bme680_7/src/esphome/components/mqtt/mqtt_component.cpp.o
Compiling .pioenvs/esp_bme680_7/src/esphome/components/mqtt/mqtt_cover.cpp.o
Compiling .pioenvs/esp_bme680_7/src/esphome/components/mqtt/mqtt_date.cpp.o
Compiling .pioenvs/esp_bme680_7/src/esphome/components/mqtt/mqtt_datetime.cpp.o
Compiling .pioenvs/esp_bme680_7/src/esphome/components/mqtt/mqtt_event.cpp.o
Compiling .pioenvs/esp_bme680_7/src/esphome/components/mqtt/mqtt_fan.cpp.o
Compiling .pioenvs/esp_bme680_7/src/esphome/components/mqtt/mqtt_light.cpp.o
Compiling .pioenvs/esp_bme680_7/src/esphome/components/mqtt/mqtt_lock.cpp.o
Compiling .pioenvs/esp_bme680_7/src/esphome/components/mqtt/mqtt_number.cpp.o
Compiling .pioenvs/esp_bme680_7/src/esphome/components/mqtt/mqtt_select.cpp.o
Compiling .pioenvs/esp_bme680_7/src/esphome/components/mqtt/mqtt_sensor.cpp.o
Compiling .pioenvs/esp_bme680_7/src/esphome/components/mqtt/mqtt_switch.cpp.o
Compiling .pioenvs/esp_bme680_7/src/esphome/components/mqtt/mqtt_text.cpp.o
Compiling .pioenvs/esp_bme680_7/src/esphome/components/mqtt/mqtt_text_sensor.cpp.o
Compiling .pioenvs/esp_bme680_7/src/esphome/components/mqtt/mqtt_time.cpp.o
Compiling .pioenvs/esp_bme680_7/src/esphome/components/mqtt/mqtt_update.cpp.o
Compiling .pioenvs/esp_bme680_7/src/esphome/components/mqtt/mqtt_valve.cpp.o
Compiling .pioenvs/esp_bme680_7/src/esphome/components/network/util.cpp.o
Compiling .pioenvs/esp_bme680_7/src/esphome/components/ota/ota_backend.cpp.o
Compiling .pioenvs/esp_bme680_7/src/esphome/components/ota/ota_backend_arduino_esp8266.cpp.o
Assembler messages:
Fatal error: can't create .pioenvs/esp_bme680_7/src/esphome/components/ota/ota_backend.cpp.o: No such file or directory
*** [.pioenvs/esp_bme680_7/src/esphome/components/ota/ota_backend.cpp.o] Error 1
Assembler messages:
Fatal error: can't create .pioenvs/esp_bme680_7/src/esphome/components/mqtt/mqtt_valve.cpp.o: No such file or directory
*** [.pioenvs/esp_bme680_7/src/esphome/components/mqtt/mqtt_valve.cpp.o] Error 1
Assembler messages:
Fatal error: can't create .pioenvs/esp_bme680_7/src/esphome/components/network/util.cpp.o: No such file or directory
*** [.pioenvs/esp_bme680_7/src/esphome/components/network/util.cpp.o] Error 1
Assembler messages:
Fatal error: can't create .pioenvs/esp_bme680_7/src/esphome/components/ota/ota_backend_arduino_esp8266.cpp.o: No such file or directory
*** [.pioenvs/esp_bme680_7/src/esphome/components/ota/ota_backend_arduino_esp8266.cpp.o] Error 1
========================= [FAILED] Took 369.93 seconds =========================

```
### ❌ FAIL - esp32-pasco2-lambda.yaml (180.9s)
```
l-clib==0.2.15
 + tqdm==4.67.1
 + truststore==0.10.4
 + typing-extensions==4.15.0
 + typing-inspection==0.4.2
 + urllib3==1.26.20
Reading CMake configuration...
Dependency Graph
|-- Networking @ 3.3.2
|-- ESP32 Async UDP @ 3.3.2
|-- DNSServer @ 3.3.2
|-- WiFi @ 3.3.2
|-- ESPmDNS @ 3.3.2
|-- Update @ 3.3.2
|-- noise-c @ 0.1.10
|-- Wire @ 3.3.2
Compiling .pioenvs/esp32-pasco2-lambda/src/main.cpp.o
Compiling .pioenvs/esp32-pasco2-lambda/lib80b/Network/NetworkManager.cpp.o
Compiling .pioenvs/esp32-pasco2-lambda/lib80b/Network/NetworkServer.cpp.o
Compiling .pioenvs/esp32-pasco2-lambda/lib80b/Network/NetworkUdp.cpp.o
Compiling .pioenvs/esp32-pasco2-lambda/libac4/AsyncUDP/AsyncUDP.cpp.o
Compiling .pioenvs/esp32-pasco2-lambda/lib5e9/DNSServer/DNSServer.cpp.o
Compiling .pioenvs/esp32-pasco2-lambda/lib18f/WiFi/AP.cpp.o
src/main.cpp: In lambda function:
src/main.cpp:556:11: error: 'i2c_bus' was not declared in this scope
  556 |       if (i2c_bus == nullptr) { // Prüfen, ob der globale i2c_bus Zeiger gültig ist
      |           ^~~~~~~
src/main.cpp:561:12: error: 'i2c_bus' was not declared in this scope
  561 |       if (!i2c_bus->probe_device(device->get_i2c_address())) { // Korrigiert: i2c_bus direkt verwenden und get_i2c_address
      |            ^~~~~~~
/config/esp32-pasco2-lambda.yaml: In lambda function:
/config/esp32-pasco2-lambda.yaml:256:13: error: 'i2c_bus' was not declared in this scope
  256 |         if (i2c_bus == nullptr) { // Prüfen, ob der globale i2c_bus Zeiger gültig ist
      |             ^~~~~~~
/config/esp32-pasco2-lambda.yaml:260:58: error: 'i2c_bus' was not declared in this scope
  260 |         id(pas_co2_device_ptr) = new i2c::I2CDevice(i2c_bus, id(pas_co2_i2c_address)); // Korrigiert: .state entfernt
      |                                                          ^~~~~~~
/config/esp32-pasco2-lambda.yaml: In lambda function:
/config/esp32-pasco2-lambda.yaml:298:14: error: 'i2c_bus' was not declared in this scope
  298 |          if (i2c_bus == nullptr) {
      |              ^~~~~~~
/config/esp32-pasco2-lambda.yaml:302:59: error: 'i2c_bus' was not declared in this scope
  302 |          id(pas_co2_device_ptr) = new i2c::I2CDevice(i2c_bus, id(pas_co2_i2c_address)); // Korrigiert
      |                                                           ^~~~~~~
/config/esp32-pasco2-lambda.yaml: In lambda function:
/config/esp32-pasco2-lambda.yaml:326:15: error: 'i2c_bus' was not declared in this scope
  326 |           if (i2c_bus == nullptr) {
      |               ^~~~~~~
/config/esp32-pasco2-lambda.yaml:330:59: error: 'i2c_bus' was not declared in this scope
  330 |          id(pas_co2_device_ptr) = new i2c::I2CDevice(i2c_bus, id(pas_co2_i2c_address)); // Korrigiert
      |                                                           ^~~~~~~
Compiling .pioenvs/esp32-pasco2-lambda/lib18f/WiFi/STA.cpp.o
*** [.pioenvs/esp32-pasco2-lambda/src/main.cpp.o] Error 1
========================= [FAILED] Took 172.66 seconds =========================

```
### ❌ FAIL - esp_bme680_6_bsec_filter_v1.yaml (600.0s)
```
TIMEOUT
```
### ❌ FAIL - esp_ikea_bme680_bsec_filter.yaml (4.8s)
```
INFO ESPHome 2025.11.4
INFO Reading configuration /config/esp_ikea_bme680_bsec_filter.yaml...
INFO Cloning https://github.com/sermayoral/esphome.git@pm1006
Failed config

external_components: [source /config/esp_ikea_bme680_bsec_filter.yaml:21]
  - 
    could not read Username for 'https://github.com': No such device or address.
    source: github://sermayoral/esphome@pm1006
    components: 
      - pm1006

```
### ❌ FAIL - esp_bme680_7_bsec_filter_v2.yaml (600.0s)
```
TIMEOUT
```
### ❌ FAIL - esp32c3-ble-proxy.yaml (115.0s)
```
r: Installing INFO Installing https://github.com/pioarduino/registry/releases/download/0.0.1/esptoolpy-v5.1.0.zip
https://github.com/pioarduino/registry/releases/download/0.0.1/esptoolpy-v5.1.0.zip

[?25lUnpacking  [------------------------------------]    0%
[?25lUnpacking  [##################------------------]   50%
[?25lUnpacking  [####################################]  100%[?25h
Tool Manager: tool-esptoolpy@5.1.0 has been installed!
INFO tool-esptoolpy@5.1.0 has been installed!
INFO Installing tools via idf_tools.py (this may take several minutes)...
INFO Installing file:///root/.platformio/tools/tool-esptoolpy
Tool Manager: Installing file:///root/.platformio/tools/tool-esptoolpy
Tool Manager: INFO tool-esptoolpy@1.30000.201119 has been installed!
tool-esptoolpy@1.30000.201119 has been installed!
INFO Tool tool-esptoolpy successfully installed
error: /config/.esphome/platformio/packages/tool-esptoolpy@src-4cd5cc7e165265fec9e384cd502be751 does not appear to be a Python project, as neither `pyproject.toml` nor `setup.py` are present in the directory
Warning: Failed to install esptool from /config/.esphome/platformio/packages/tool-esptoolpy@src-4cd5cc7e165265fec9e384cd502be751 (exit 2)
INFO Package configuration completed successfully
HARDWARE: ESP32C3 160MHz, 320KB RAM, 4MB Flash
 - contrib-piohome @ 3.4.4 
 - framework-espidf @ 3.50501.0 (5.5.1) 
 - tool-cmake @ 4.0.3 
 - tool-esp-rom-elfs @ 2024.10.11 
 - tool-mklittlefs @ 3.2.0 
 - tool-ninja @ 1.13.1 
 - tool-scons @ 4.40801.0 (4.8.1) 
 - toolchain-riscv32-esp @ 14.2.0+20241119
Creating a new virtual environment for IDF Python dependencies using uv
Using Python 3.12.10 environment at: /root/.platformio/penv/.espidf-5.5.1
Installing ESP-IDF's Python dependencies with uv
Using Python 3.12.10 environment at: /root/.platformio/penv/.espidf-5.5.1
Resolved 27 packages in 1.46s
Downloading cryptography (4.0MiB)
Downloading pydantic-core (1.9MiB)
 Downloaded pydantic-core
 Downloaded cryptography
Prepared 18 packages in 1.71s
Installed 27 packages in 586ms
 + annotated-types==0.7.0
 + certifi==2025.11.12
 + cffi==2.0.0
 + charset-normalizer==3.4.4
 + click==8.3.1
 + colorama==0.4.6
 + cryptography==44.0.3
 + esp-idf-kconfig==2.5.0
 + idf-component-manager==2.4.3
 + idna==3.11
 + jsonref==1.1.0
 + pycparser==2.23
 + pydantic==2.11.10
 + pydantic-core==2.33.2
 + pydantic-settings==2.12.0
 + pyparsing==3.2.5
 + python-dotenv==1.2.1
 + requests==2.32.5
 + requests-file==3.0.1
 + requests-toolbelt==1.0.0
 + ruamel-yaml==0.18.16
 + ruamel-yaml-clib==0.2.15
 + tqdm==4.67.1
 + truststore==0.10.4
 + typing-extensions==4.15.0
 + typing-inspection==0.4.2
 + urllib3==1.26.20
Reading CMake configuration...
Dependency Graph
|-- noise-c @ 0.1.10
|-- ArduinoJson @ 7.4.2
Building .pioenvs/esp32c3-ble-proxy/bootloader.bin
sh: 1: /root/.platformio/penv/bin/esptool: not found
*** [.pioenvs/esp32c3-ble-proxy/bootloader.bin] Error 127
========================= [FAILED] Took 98.91 seconds =========================

```
### ❌ FAIL - esp_bme680_5_bsec_filter_v1.yaml (600.0s)
```
TIMEOUT
```
### ❌ FAIL - esp32-t5-jira-gemini.yaml (5.6s)
```
INFO ESPHome 2025.11.4
INFO Reading configuration /config/esp32-t5-jira-gemini.yaml...
Failed config

esp32: [source /config/esp32-t5-jira-gemini.yaml:11]
  board: esp32dev
  framework: 
    type: esp-idf
  
  [ext1_wakeup] is an invalid option for [esp32]. Please check the indentation.
  ext1_wakeup: 
    pins: 
      - number: 39
        inverted: True
      - number: 35
        inverted: True
    mode: ANY_HIGH

```
### ✅ PASS - esp_bme680_5_bsec_filter_v2.yaml (263.3s)
### ✅ PASS - esp_meter_1-W.yaml (250.6s)
### ✅ PASS - wt32-eth01-bt-proxy.yaml (126.8s)
### ✅ PASS - wt32-eth01-bt-proxy-a13bb8.yaml (63.0s)
### ✅ PASS - esp32-c3-042lcd-demo.yaml (76.0s)
### ✅ PASS - esp32-t5-4.7-example-partial-update.yaml (140.6s)
### ✅ PASS - esp32-heizungssteuerung.yaml (64.0s)
### ✅ PASS - air_sensor_demo.yaml (21.4s)
### ✅ PASS - esp_trafficlight_2.yaml (16.6s)
### ✅ PASS - esp32-s2-ledstring.yaml (67.7s)
### ✅ PASS - esp_bme680_1_bsec_filter_v1.yaml (148.1s)
### ✅ PASS - esp32-s2-mini.yaml (73.4s)
### ✅ PASS - livingroom.yaml (161.3s)
### ✅ PASS - toshiba-hvac-gemini-code-v1.yaml (102.7s)
### ✅ PASS - esp32_bme680_bsec_filter.yaml (170.9s)
### ✅ PASS - test1.yaml (106.6s)
### ✅ PASS - esp_meter_1-kW.yaml (594.0s)
### ✅ PASS - aubess-ir.yaml (134.7s)
### ✅ PASS - esp32-t5-sepp.yaml (119.9s)
