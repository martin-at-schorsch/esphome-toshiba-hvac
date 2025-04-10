# ESPHome Toshiba HVAC Komponente

## Beschreibung

Diese ist eine ESPHome-Komponente zur Steuerung von Toshiba HVAC (Heizungs-, Lüftungs- und Klimaanlagen) Systemen. Sie ermöglicht die Integration deiner Toshiba Klimaanlage in dein ESPHome-Setup und somit die Steuerung über Home Assistant oder andere ESPHome-kompatible Systeme.

## Funktionen

* Steuerung von Toshiba HVAC Systemen
* Integration mit ESPHome und Home Assistant
* Setzen von Modus (Heizen, Kühlen, Auto, etc.)
* Einstellen der Zieltemperatur
* Steuerung der Lüftergeschwindigkeit

## Voraussetzungen

* ESPHome installiert
* Ein ESP8266- oder ESP32-Board
* Ein Toshiba HVAC System
* Verbindung zwischen dem ESP-Board und dem Toshiba HVAC System

## Installation

1.  **Repository klonen:**
    ```
    git clone [https://github.com/your_username/esphome-toshiba-hvac](https://github.com/your_username/esphome-toshiba-hvac)
    ```
    Ersetze `your_username` durch deinen GitHub-Benutzernamen.

2.  **Externe Komponente konfigurieren:**
    Füge die externe Komponente in deine ESPHome-Konfiguration ein:
    ```yaml
    external_components:
      - source:
          type: git
          url: [https://github.com/your_username/esphome-toshiba-hvac](https://github.com/your_username/esphome-toshiba-hvac)
        components:
          - toshiba_hvac
    ```

3.  **Climate Komponente konfigurieren:**
    Konfiguriere die Climate Komponente in deiner ESPHome-Konfiguration:
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

## Verwendung

Nachdem die Komponente installiert und konfiguriert ist, kannst du deine Toshiba HVAC-Anlage über Home Assistant oder andere ESPHome-kompatible Systeme steuern.

## Konfiguration

* `name`: Der Name der HVAC-Anlage in ESPHome.
* `uart_id`: Die ID der verwendeten UART-Verbindung
* `supported_modes`: Die von deiner Anlage unterstützten Modi.
* `supported_fan_modes`: Die von deiner Anlage unterstützten Lüftermodi.
* `default_temperature`: Die Standardtemperatur.
* `visual`: Einstellungen für die Temperaturanzeige.

## Customization

Du kannst die Komponente an deine spezifischen Bedürfnisse anpassen, indem du die C++-Code-Datei `toshiba_hvac.cpp` änderst.

## Contributing

Beiträge sind willkommen! Bitte sende einen Pull Request, um Änderungen vorzuschlagen.

## Lizenz

[Füge hier die Lizenz ein]
