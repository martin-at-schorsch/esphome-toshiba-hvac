# esphome/components/toshiba_hvac/climate.py

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate, uart
from esphome.const import CONF_ID, CONF_UART_ID

# Namespace für den C++ Code definieren
toshiba_hvac_ns = cg.esphome_ns.namespace("toshiba_hvac")

# Die C++ Klasse deklarieren, die von climate::Climate und Component erbt
ToshibaHVACClimate = toshiba_hvac_ns.class_("ToshibaHVACClimate", climate.Climate, cg.Component)

# Schema-Definition mit der modernen climate.climate_schema Funktion
CONFIG_SCHEMA = (
    climate.climate_schema(ToshibaHVACClimate)
    .extend(
        {
            # Die UART-Komponente als Abhängigkeit definieren
            cv.Required(CONF_UART_ID): cv.use_id(uart.UARTComponent),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
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
    uart_comp = await cg.get_variable(config[CONF_UART_ID])
    cg.add(var.set_uart(uart_comp))