#ifndef GfSun2000_h
#define GfSun2000_h
#include <Arduino.h>
#include <map>
#include <ModbusClientRTU.h>

#define MODBUS_REGISTRY_FROM  0
#define MODBUS_REGISTRY_TO    125
#define MODBUS_TIMEOUT     2000
#define MODBUS_INDEX_OFFSET  3     // id server and function ID


#define REGISTRY_DEVICE_ID               3     // 10 chars; registers 3,4,5,6,7
#define REGISTRY_CUSTOM_ENERGY_COUNTER  60     // 2 bytes
#define REGISTRY_TOTAL_ENERGY_COUNTER   63     // 2 bytes
#define REGISTRY_AC_VOLTAGE             70     // 2 bytes
#define REGISTRY_AVERAGE_POWER          86     // 2 bytes
#define REGISTRY_DC_VOLTAGE            109     // 2 bytes

typedef struct {
    char deviceID[11];
    double ACVoltage;
    double DCVoltage;
    double averagePower;
    double customEnergyCounter;
    double totalEnergyCounter;
    std::map<int16_t, int16_t> modbusRegistry;
} GfSun2000Data;

typedef void (*GfSun2000OnData) (GfSun2000Data data);
typedef void (*GfSun2000OnError) (int errorId, char* errorMessage);


class GfSun2000Callback {
public:
  virtual void dataHandler(GfSun2000Data data);
  virtual void errorHandler(int errorId, char* errorMessage);
};

class GfSun2000 {
public:
    void setup(HardwareSerial& serial, int8_t rtsPin = -1, int8_t remoteNumber = 1);
    bool readData();
    void setDataHandler(GfSun2000OnData handler);
    void setErrorHandler(GfSun2000OnError handler);
    void setObjectHandler(GfSun2000Callback *obj);
};

#endif