
#include "GfSun2000.h"

#include "esphome/core/log.h"
#include "esphome/core/component.h"

#ifdef USE_ARDUINO
#include <Arduino.h>
#endif

namespace esphome {
namespace inverter {

static const char *TAG = "GfSun2000";

static const uint8_t MODBUS_CMD_READ_111 = 111;
static const uint8_t MODBUS_REGISTER_COUNT = MODBUS_REGISTRY_TO - MODBUS_REGISTRY_FROM + 1;  // 126

int16_t reg2ix(int16_t regNumber) { return (regNumber * 2) + MODBUS_INDEX_OFFSET; }

void InverterComponent::on_modbus_data(const std::vector<uint8_t> &data) {
  // void handleData(ModbusMessage response, uint32_t token) {
  // GfSun2000Data data;
  // byte num8;
  // for (byte i = 0; i < 10; i++) {
  //   response.get(i + reg2ix(REGISTRY_DEVICE_ID), num8);
  //   data.deviceID[i] = (char) num8;
  // }
  // data.deviceID[10] = '\0';

  // int16_t num16;
  // // AC Voltage
  // response.get(reg2ix(REGISTRY_AC_VOLTAGE), num16);
  // data.ACVoltage = num16 / 10.00;

  // // DC Voltage
  // response.get(reg2ix(REGISTRY_DC_VOLTAGE), num16);
  // data.DCVoltage = num16 / 10.00;

  // // output Power
  // response.get(reg2ix(REGISTRY_AVERAGE_POWER), num16);
  // data.averagePower = num16 / 10.00;

  // // custom energy (can by reseted)
  // response.get(reg2ix(REGISTRY_CUSTOM_ENERGY_COUNTER), num16);
  // data.customEnergyCounter = num16 / 10.00;

  // // custom energy (can by reseted)
  // response.get(reg2ix(REGISTRY_TOTAL_ENERGY_COUNTER), num16);
  // data.totalEnergyCounter = num16 / 10.00;

  // for (int16_t i = MODBUS_REGISTRY_FROM; i < MODBUS_REGISTRY_TO; i++) {
  //   response.get(reg2ix(i), num16);
  //   if (num16 != 0) {
  //     data.modbusRegistry[i] = num16;
  //   }
  // }
  // if (onData) {
  //   onData(data);
  // }
  // if (cobj) {
  //   cobj->dataHandler(data);
  // }

  // todo check if correct
  if (this->total_energy_sensor_ != nullptr) {
    int16_t total_energy = 69;
    this->total_energy_sensor_->publish_state(total_energy / 10.0f);
  }
}

float InverterComponent::get_setup_priority() const { return setup_priority::DATA; }

void InverterComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "GfSun2000:");
  ESP_LOGCONFIG(TAG, "  Address: 0x%02X", this->address_);

  LOG_SENSOR("  ", "Voltage", this->voltage_sensor_);
  LOG_SENSOR("  ", "Total Energy", this->total_energy_sensor_);
}

void InverterComponent::setup() {
  int8_t rtsPin = -1;
  ESP_LOGW(TAG, "setup!");

  if (reset_pin_ != nullptr) {
    rtsPin = reset_pin_->get_pin();
  }
}

void InverterComponent::update() {
  ESP_LOGD(TAG, "UPDATE!");

  if (this->waiting_for_response()) {
    this->waiting_to_update_ = true;
    ESP_LOGD(TAG, "Waiting for response from inverter...");
    return;
  }

  this->waiting_to_update_ = false;
  // Error err = modbus_->addRequest(111, remoteNum_, READ_HOLD_REGISTER, MODBUS_REGISTRY_FROM, MODBUS_REGISTRY_TO);
  this->send(MODBUS_CMD_READ_111, MODBUS_REGISTRY_FROM, MODBUS_REGISTER_COUNT);
  this->last_send_ = millis();
}

}  // namespace inverter
}  // namespace esphome
