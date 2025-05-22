import esphome.codegen as cg
from esphome import core
from esphome.components import sensor


GfSun2000Sensor = cg.register_component("GfSun2000Sensor")

ac_voltage = sensor.sensor_schema("AC Voltage")
dc_voltage = sensor.sensor_schema("DC Voltage")
power = sensor.sensor_schema("Power")
energy = sensor.sensor_schema("Energy")
c_energy = sensor.sensor_schema("Cumulative Energy")

class GfSun2000SensorComponent:
    def __init__(self):
        self.gf_sun = GfSun2000()
        self.ac_voltage_value = 0.0
        self.dc_voltage_value = 0.0
        self.power_value = 0.0
        self.energy_value = 0.0
        self.c_energy_value = 0.0

    def update(self):
        if self.gf_sun.readData():  # Čtení dat
            self.ac_voltage = self.gf_sun.ACVoltage
            self.dc_voltage = self.gf_sun.DCVoltage
            self.power = self.gf_sun.averagePower
            self.energy = self.gf_sun.customEnergyCounter
            self.c_energy = self.gf_sun.totalEnergyCounter

            ac_voltage.publish_state(self.ac_voltage_value)
            dc_voltage.publish_state(self.dc_voltage_value)
            power.publish_state(self.power_value)
            energy.publish_state(self.energy_value)
            c_energy.publish_state(self.c_energy_value)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    var.setup(config[CONF_SERIAL_PORT])
    cg.add(var.set_update_interval(3))
    cg.add(var.update())  