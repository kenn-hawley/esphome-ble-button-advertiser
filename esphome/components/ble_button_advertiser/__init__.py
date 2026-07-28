import esphome.config_validation as cv
import esphome.codegen as cg
import esphome.automation as automation
from esphome import pins
from esphome.const import CONF_ID

CONF_PIN = "pin"

ns = cg.esphome_ns.namespace("ble_button_advertiser")
BleButtonAdvertiser = ns.class_("BleButtonAdvertiser", cg.Component)
AdvertisePressAction = ns.class_("AdvertisePressAction", automation.Action)

CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(BleButtonAdvertiser),
    cv.Optional(CONF_PIN): pins.gpio_input_pin_schema,
})

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    
    if CONF_PIN in config:
        pin = await cg.gpio_pin_expression(config[CONF_PIN])
        cg.add(var.set_pin(pin))


@automation.register_action(
    "ble_button_advertiser.advertise_press",
    AdvertisePressAction,
    cv.Schema({
        cv.Required(cv.GenerateID()): cv.use_id(BleButtonAdvertiser),
    })
)
async def advertise_press_to_code(config, action_id, template_arg, args):
    paren = await cg.get_variable(config[cv.GenerateID()])
    var = cg.new_Pvariable(action_id, template_arg, paren)
    return var
