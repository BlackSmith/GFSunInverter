import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID, UNIT_VOLT, UNIT_WATT, UNIT_KILOWATT_HOURS, ICON_EMPTY, ICON_POWER, CONF_SERIAL_PORT

gf_sun = GfSun2000()

gfsun2000_ns = cg.esphome_ns.namespace("gfsun2000")
GfSun2000Sensor = gfsun2000_ns.class_(
    "GfSun2000Sensor", cg.PollingComponent
)

CONF_SERIAL_PORT = "serial_port" 
CONF_SENSOR1 = "AC Voltage"
CONF_SENSOR2 = "DC Voltage"
CONF_SENSOR3 = "Power"
CONF_SENSOR4 = "Energy"
CONF_SENSOR5 = "Cumulative Energy"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(GfSun2000Sensor),
        cv.Optional(CONF_SERIAL_PORT): cv.use_id(sensor.Sensor),
        cv.Optional(CONF_SENSOR1): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT, icon=ICON_EMPTY, accuracy_decimals=1
        ),
        cv.Optional(CONF_SENSOR2): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT, icon=ICON_EMPTY, accuracy_decimals=1
        ),
        cv.Optional(CONF_SENSOR3): sensor.sensor_schema(
            unit_of_measurement=UNIT_WATT, icon=ICON_POWER, accuracy_decimals=2
        ),
        cv.Optional(CONF_SENSOR4): sensor.sensor_schema(
            unit_of_measurement=UNIT_KILOWATT_HOURS, icon=ICON_EMPTY, accuracy_decimals=2
        ),
        cv.Optional(CONF_SENSOR5): sensor.sensor_schema(
            unit_of_measurement=UNIT_KILOWATT_HOURS, icon=ICON_EMPTY, accuracy_decimals=2
        ),
    }
).extend(cv.polling_component_schema("3s"))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])

    serial_port = config[CONF_SERIAL_PORT] 
    var.setup(serial_port)

    # Přidání senzorů
    if CONF_SENSOR1 in config:
        sensor1 = var.get_sensor(config[CONF_SENSOR1])
        cg.add(var.set_ac_voltage_sensor(sensor1))

    if CONF_SENSOR2 in config:
        sensor2 = var.get_sensor(config[CONF_SENSOR2])
        cg.add(var.set_dc_voltage_sensor(sensor2))

    if CONF_SENSOR3 in config:
        sensor3 = var.get_sensor(config[CONF_SENSOR3])
        cg.add(var.set_power_sensor(sensor3))

    if CONF_SENSOR4 in config:
        sensor4 = var.get_sensor(config[CONF_SENSOR4])
        cg.add(var.set_energy_sensor(sensor4))

    if CONF_SENSOR5 in config:
        sensor5 = var.get_sensor(config[CONF_SENSOR5])
        cg.add(var.set_cumulative_energy_sensor(sensor5))

    cg.add(var.set_update_interval(3))



# import esphome.codegen as cg
# from esphome import core
# from esphome.components import sensor


# GfSun2000Sensor = cg.register_component("GfSun2000Sensor")

# ac_voltage = sensor.sensor_schema("AC Voltage")
# dc_voltage = sensor.sensor_schema("DC Voltage")
# power = sensor.sensor_schema("Power")
# energy = sensor.sensor_schema("Energy")
# c_energy = sensor.sensor_schema("Cumulative Energy")

# class GfSun2000SensorComponent:
#     def __init__(self):
#         self.gf_sun = GfSun2000()
#         self.ac_voltage_value = 0.0
#         self.dc_voltage_value = 0.0
#         self.power_value = 0.0
#         self.energy_value = 0.0
#         self.c_energy_value = 0.0

#     def update(self):
#         if self.gf_sun.readData():  # Čtení dat
#             self.ac_voltage = self.gf_sun.ACVoltage
#             self.dc_voltage = self.gf_sun.DCVoltage
#             self.power = self.gf_sun.averagePower
#             self.energy = self.gf_sun.customEnergyCounter
#             self.c_energy = self.gf_sun.totalEnergyCounter

#             ac_voltage.publish_state(self.ac_voltage_value)
#             dc_voltage.publish_state(self.dc_voltage_value)
#             power.publish_state(self.power_value)
#             energy.publish_state(self.energy_value)
#             c_energy.publish_state(self.c_energy_value)

# def to_code(config):
#     var = cg.new_Pvariable(config[CONF_ID])
#     var.setup(config[CONF_SERIAL_PORT])
#     cg.add(var.set_update_interval(3))
#     cg.add(var.update())  