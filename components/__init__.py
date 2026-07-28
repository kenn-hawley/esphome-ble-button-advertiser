import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import gpio
from esphome.const import CONF_PIN, CONF_ID

CODEOWNERS = ["@kenn-hawley"]
DEPENDENCIES = []

ble_button_advertiser_ns = cg.esphome_ns.namespace("ble_button_advertiser")
BLEButtonAdvertiser = ble_button_advertiser_ns.class_(
    "BLEButtonAdvertiser", cg.Component
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(BLEButtonAdvertiser),
        cv.Optional(CONF_PIN): gpio.gpio_pin_schema,
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvt_var(config[cv.GenerateID()], BLEButtonAdvertiser)
    await cg.register_component(var, config)

    if CONF_PIN in config:
        pin = await gpio.gpio_pin_to_code(config[CONF_PIN])
        cg.add(var.set_button_pin(pin))
