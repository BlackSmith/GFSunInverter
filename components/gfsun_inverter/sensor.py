import logging


from esphome import pins
import esphome.codegen as cg
from esphome.components import sensor, modbus
import esphome.config_validation as cv
from esphome.core import CORE
from esphome.const import (
    CONF_ID,
    CONF_RESET_PIN,
    CONF_TIMEOUT,
    CONF_VOLTAGE,
    DEVICE_CLASS_VOLTAGE,
    DEVICE_CLASS_ENERGY,
    UNIT_VOLT,
    UNIT_WATT,
    UNIT_KILOWATT_HOURS,
    STATE_CLASS_MEASUREMENT,
)
from .const import ICON_ENERGY, ICON_FREQUENCY, ICON_VOLTAGE


DEPENDENCIES = ["modbus"]
AUTO_LOAD = ["spi"]

CONF_TOTAL_ENERGY = "total_energy"

inverter_ns = cg.esphome_ns.namespace("inverter")
InverterComponent = inverter_ns.class_(
    "InverterComponent", cg.PollingComponent, modbus.ModbusDevice
)


def validate_config(config):
    if not CORE.using_arduino:
        raise cv.Invalid("Other than arduino framework is not supported!")
    return config


CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(InverterComponent),
            cv.Optional(CONF_RESET_PIN): pins.internal_gpio_output_pin_schema,
            cv.Optional(CONF_TIMEOUT, default="2s"): cv.templatable(
                cv.positive_time_period_milliseconds
            ),
            cv.Optional(CONF_VOLTAGE): sensor.sensor_schema(
                icon=ICON_VOLTAGE,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_VOLTAGE,
                unit_of_measurement=UNIT_VOLT,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_TOTAL_ENERGY): sensor.sensor_schema(
                icon=ICON_ENERGY,
                accuracy_decimals=3,
                device_class=DEVICE_CLASS_ENERGY,
                # state_class=STATE_CLASS_TOTAL_INCREASING,
                unit_of_measurement=UNIT_KILOWATT_HOURS,
            ),
        }
    )
    .extend(modbus.modbus_device_schema(0x01))
    .extend(cv.polling_component_schema("5s")),
    validate_config,
)


async def to_code(config):

    # cg.add_library("miq19/eModbus", "1.0.0")

    # cg.add_define("USE_INVERTER")
    # cg.add_global(inverter_ns.using)

    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await modbus.register_modbus_device(var, config)

    # cg.add(var.set_protocol_version(config[CONF_PROTOCOL_VERSION]))

    cg.add(var.set_timeout(config[CONF_TIMEOUT].total_milliseconds))

    if reset_pin := config.get(CONF_RESET_PIN):
        pin = await cg.gpio_pin_expression(reset_pin)
        cg.add(var.set_reset_pin(pin))

    if voltage_config := config.get(CONF_VOLTAGE):
        sens = await sensor.new_sensor(voltage_config)
        cg.add(var.set_voltage_sensor(sens))

    if total_energy_config := config.get(CONF_TOTAL_ENERGY):
        sens = await sensor.new_sensor(total_energy_config)
        cg.add(var.set_total_energy_sensor(sens))
