#include <Arduino.h>
#include <ModbusClientRTU.h>
#include "GfSun2000.h"

ModbusClientRTU* modbus;

ModbusMessage* response;
GfSun2000OnData onData;
GfSun2000OnError onError;
GfSun2000Callback* cobj;

int8_t remoteNum = 1;
bool done = true;

void handleError(Error error, uint32_t token) {
  // ModbusError wraps the error code and provides a readable error message for it
  done = true;
  response = nullptr;
  ModbusError me(error);
  if (onError) {
      onError((int)me, (char*)(const char *)me);
  }
  if (cobj) {
      cobj->errorHandler((int)me, (char*)(const char *)me);
  }
}

int16_t reg2ix(int16_t regNumber) {
    return (regNumber * 2) + MODBUS_INDEX_OFFSET;
}


void handleData(ModbusMessage response, uint32_t token) {
    GfSun2000Data data;
    byte num8;
    for (byte i=0; i<10; i++) {
        response.get(i + reg2ix(REGISTRY_DEVICE_ID), num8);
        data.deviceID[i] = (char)num8;
    }
    data.deviceID[10] = '\0';

    int16_t num16;
    // AC Voltage
    response.get(reg2ix(REGISTRY_AC_VOLTAGE), num16);
    data.ACVoltage = num16/10.00;

    // DC Voltage
    response.get(reg2ix(REGISTRY_DC_VOLTAGE), num16);
    data.DCVoltage = num16/10.00;

    // output Power
    response.get(reg2ix(REGISTRY_AVERAGE_POWER), num16);
    data.averagePower = num16/10.00;

    // custom energy (can by reseted)
    response.get(reg2ix(REGISTRY_CUSTOM_ENERGY_COUNTER), num16);
    data.customEnergyCounter = num16/10.00;

    // custom energy (can by reseted)
    response.get(reg2ix(REGISTRY_TOTAL_ENERGY_COUNTER), num16);
    data.totalEnergyCounter = num16/10.00;

    for (int16_t i=MODBUS_REGISTRY_FROM; i < MODBUS_REGISTRY_TO ; i++){
        response.get(reg2ix(i), num16);
        if (num16 !=0 ) {
            data.modbusRegistry[i] = num16;
        }
    }
    if (onData) {
        onData(data);
    }
    if (cobj) {
      cobj->dataHandler(data);
    }
}

void GfSun2000::setup(HardwareSerial& serial, int8_t rtsPin, int8_t remoteNumber) {
    serial.begin(9600, SERIAL_8N1);
    modbus = new ModbusClientRTU(serial, rtsPin);
    modbus->onDataHandler(&handleData);
    modbus->onErrorHandler(&handleError);
    modbus->setTimeout(MODBUS_TIMEOUT);
    modbus->begin();
    remoteNum = remoteNumber;
}

void GfSun2000::setDataHandler(GfSun2000OnData handler) {
    onData = handler;
}

void GfSun2000::setErrorHandler(GfSun2000OnError handler) {
    onError = handler;
}

void GfSun2000::setObjectHandler(GfSun2000Callback *obj) {
        cobj = obj;
}

bool GfSun2000::readData() {
  done = false;
  Error err = modbus->addRequest(111, remoteNum, READ_HOLD_REGISTER, MODBUS_REGISTRY_FROM, MODBUS_REGISTRY_TO);
  if (err!=SUCCESS) {
    ModbusError e(err);
    Serial.printf("Error creating request: %02X - %s\n", (int)e, (const char *)e);
    return false;
  }
  return true;
}

