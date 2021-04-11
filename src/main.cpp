#include <Arduino.h>
#include <map>


#include "GfSun2000.h"

GfSun2000 GF = GfSun2000();


void dataHandler(GfSun2000Data data) {
  Serial.println("\n\n");
  Serial.printf("Device ID     : %s\n", data.deviceID);
  Serial.printf("AC Voltage    : %.1f\tV\n", data.ACVoltage);
  Serial.printf("DC Voltage    : %.1f\tV\n", data.DCVoltage);  
  Serial.printf("Output Power  : %.1f\tW (5min avg)\n", data.averagePower);
  Serial.printf("Custom Energy : %.1f\tkW/h (can be reseted)\n", data.customEnergyCounter);
  Serial.printf("Total Energy  : %.1f\tkW/h\n", data.totalEnergyCounter);
  Serial.println("-----------------------------");
  
  std::map<int16_t, int16_t>::iterator itr;
  for (itr = data.modbusRegistry.begin(); itr != data.modbusRegistry.end(); ++itr) {
        Serial.printf("Registry %d: %d \n", itr->first, itr->second);
  }  
}

void setup() {
  Serial.begin(9600);
  while (!Serial) {}
  Serial.println("__ OK __");  
  GF.setup(Serial2);  
  GF.setDataHandler(dataHandler);
}

void loop() {
  GF.readData();
  sleep(3);
}
