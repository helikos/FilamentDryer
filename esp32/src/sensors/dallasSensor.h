#ifndef _DALLAS_SENSORS_H
#define _DALLAS_SENSORS_H

#include <OneWire.h>
#include <DallasTemperature.h>

#include <DallasTemperature.h>
#include "sensor.h"

class dallasSensor : sensor {
public:
  dallasSensor(uint8_t pin) {

     oneWire = OneWire(pin);
     _sensor = DallasTemperature(&oneWire);
    std::fill_n(thermometer, sizeof(thermometer), 0);
    initialization();
  }

  float getTemperature() {
      get();
      return _temperature;
  };
  void printTemperature();


private:
  OneWire oneWire;
  DallasTemperature _sensor;
  DeviceAddress thermometer;
  float _temperature;

  void initialization();
  void printAddress();
  void printAlarms();
  void printData();

  bool get();
};



sensor* dallasSensorsInitialization(uint8_t pin);


#endif
