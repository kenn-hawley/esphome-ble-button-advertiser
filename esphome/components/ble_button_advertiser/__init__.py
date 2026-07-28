import esphome.config_validation as cv
import esphome.codegen as cg
from esphome import pins
from esphome.const import CONF_ID

ns = cg.esphome_ns.namespace("ble_button_advertiser")
BleButtonAdvertiser = ns.class_("BleButtonAdvertiser", cg.Component)

CONF_BUTTON_PIN = "button_pin"

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend(
    {
    cv.GenerateID(): cv.declare_id(BleButtonAdvertiser),
    cv.Required(CONF_BUTTON_PIN): pins.gpio_input_pin_schema,
})

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    pin = await cg.gpio_pin_expression(config[CONF_BUTTON_PIN])
    cg.add(var.set_button_pin(pin))
