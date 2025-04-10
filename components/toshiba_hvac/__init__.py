# __init__.py
import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.components import climate, uart
from esphome.const import CONF_NAME, CONF_UART_ID

#from esphome.core import CORE # Nicht mehr direkt importieren, wird übergeben

# Schema für die Konfiguration der Komponente
CONFIG_SCHEMA = climate.CLIMATE_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(climate.Climate),
    cv.Required(CONF_NAME): cv.string,
    cv.Required(CONF_UART_ID): cv.use_id(uart.UARTComponent), # Verwende CONF_UART_ID
})

# Funktion, um die Komponente im Code zu registrieren
def to_code(config):
    # Erzeuge eine Instanz der ToshibaHVACClimate Klasse
    var = cg.new_Pvariable(config[CONF_NAME], cg.global_ns.class_("ToshibaHVACClimate", climate.Climate)) # Übergabe von climate.Climate
    cg.Pvariable(var) # Registriere die Variable als ESPHome State

    # Rufe die Konfigurationsfunktionen der Basisklasse auf
    climate.cg.add_climate_config(var, config)

    # Setze die UART-Komponente
    uart_comp = cg.get_variable(config[CONF_UART_ID]) # Verwende CONF_UART_ID
    cg.add(var.set_uart(uart_comp))
    
    # Die externen lambdas werden nun in der C++-Datei definiert.
    

