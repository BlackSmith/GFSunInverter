#pragma once
#include "esphome/core/defines.h"

// #include <memory>
#include <map>

#include "esphome/components/sensor/sensor.h"
#include "esphome/components/modbus/modbus.h"
#include "esphome/core/component.h"
#include "esphome/core/hal.h"

#include <Arduino.h>

#define MODBUS_REGISTRY_FROM 0
#define MODBUS_REGISTRY_TO 125
#define MODBUS_TIMEOUT 2000
#define MODBUS_INDEX_OFFSET 3  // id server and function ID

#define REGISTRY_DEVICE_ID 3               // 10 chars; registers 3,4,5,6,7
#define REGISTRY_CUSTOM_ENERGY_COUNTER 60  // 2 bytes
#define REGISTRY_TOTAL_ENERGY_COUNTER 63   // 2 bytes
#define REGISTRY_AC_VOLTAGE 70             // 2 bytes
#define REGISTRY_AVERAGE_POWER 86          // 2 bytes
#define REGISTRY_DC_VOLTAGE 109            // 2 bytes

namespace esphome {
namespace inverter {

typedef struct {
  char deviceID[11];
  double ACVoltage;
  double DCVoltage;
  double averagePower;
  double customEnergyCounter;
  double totalEnergyCounter;
  std::map<int16_t, int16_t> modbusRegistry;
} GfSun2000Data;

class InverterComponent : public PollingComponent, public modbus::ModbusDevice {
  SUB_SENSOR(voltage)
  SUB_SENSOR(total_energy)

 public:
  InverterComponent() = default;

  void setup() override;
  void update() override;
  void dump_config() override;

  float get_setup_priority() const override;

  void set_reset_pin(InternalGPIOPin *pin) { this->reset_pin_ = pin; }
  void set_timeout(uint32_t timeout) { this->modbus_timeout_ = timeout; }

  void on_modbus_data(const std::vector<uint8_t> &data) override;

 protected:
  bool waiting_to_update_;
  uint32_t last_send_;

  int8_t remoteNum_ = 1;
  uint32_t modbus_timeout_{MODBUS_TIMEOUT};

 private:
  InternalGPIOPin *reset_pin_{nullptr};
};

}  // namespace inverter
}  // namespace esphome
