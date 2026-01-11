# esphome/components/toshiba_hvac/climate.py

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate, uart, sensor, select, switch, button
from esphome.const import (
    CONF_ID, 
    CONF_UART_ID, 
    CONF_OUTDOOR_TEMPERATURE, 
    UNIT_CELSIUS, 
    DEVICE_CLASS_TEMPERATURE, 
    STATE_CLASS_MEASUREMENT
)

CONF_POWER_SELECT = "power_select"
CONF_PURE_SWITCH = "pure_switch"
CONF_QUERY_BUTTON = "query_button"

# Namespace für den C++ Code definieren
toshiba_hvac_ns = cg.esphome_ns.namespace("toshiba_hvac")

# Die C++ Klasse deklarieren, die von climate::Climate und Component erbt
ToshibaHVACClimate = toshiba_hvac_ns.class_("ToshibaHVACClimate", climate.Climate, uart.UARTDevice, cg.Component)
ToshibaPowerSelect = toshiba_hvac_ns.class_("ToshibaPowerSelect", select.Select, cg.Component)
ToshibaPureSwitch = toshiba_hvac_ns.class_("ToshibaPureSwitch", switch.Switch, cg.Component)
ToshibaQueryButton = toshiba_hvac_ns.class_("ToshibaQueryButton", button.Button, cg.Component)

# Schema-Definition mit der modernen climate.climate_schema Funktion
CONFIG_SCHEMA = (
    climate.climate_schema(ToshibaHVACClimate)
    .extend(uart.UART_DEVICE_SCHEMA)
    .extend(cv.COMPONENT_SCHEMA)
    .extend({
        cv.Optional(CONF_OUTDOOR_TEMPERATURE): sensor.sensor_schema(
            unit_of_measurement=UNIT_CELSIUS,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_POWER_SELECT): select.select_schema(ToshibaPowerSelect),
        cv.Optional(CONF_PURE_SWITCH): switch.switch_schema(ToshibaPureSwitch),
        cv.Optional(CONF_QUERY_BUTTON): button.button_schema(ToshibaQueryButton),
    })
)

# Moderne asynchrone to_code Funktion
async def to_code(config):
    # Eine Instanz deiner C++ Klasse erstellen
    var = cg.new_Pvariable(config[CONF_ID])
    
    # Die Komponente registrieren
    await cg.register_component(var, config)
    # Die Klimakomponente registrieren
    await climate.register_climate(var, config)

    # Die UART-Komponente holen und an deine Klasse übergeben
    # Register as UART device
    await uart.register_uart_device(var, config)

    if CONF_OUTDOOR_TEMPERATURE in config:
        sens = await sensor.new_sensor(config[CONF_OUTDOOR_TEMPERATURE])
        cg.add(var.set_outdoor_temperature_sensor(sens))

    if CONF_POWER_SELECT in config:
        conf = config[CONF_POWER_SELECT]
        sel = await select.new_select(conf, options=["100%", "75%", "50%"])
        await cg.register_component(sel, conf)
        cg.add(sel.set_parent(var))
        cg.add(var.set_power_select(sel))

    if CONF_PURE_SWITCH in config:
        conf = config[CONF_PURE_SWITCH]
        sw = await switch.new_switch(conf)
        await cg.register_component(sw, conf)
        cg.add(sw.set_parent(var))
        cg.add(var.set_pure_switch(sw))
        
    if CONF_QUERY_BUTTON in config:
        conf = config[CONF_QUERY_BUTTON]
        btn = await button.new_button(conf)
        await cg.register_component(btn, conf)
        cg.add(btn.set_parent(var))
        cg.add(var.set_query_button(btn))