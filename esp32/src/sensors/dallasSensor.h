#ifndef _DALLAS_SENSORS_H
#define _DALLAS_SENSORS_H

#include <OneWire.h>
#include <DallasTemperature.h>
#include "sensor.h"

sensor *dallasSensorsInitialization(uint8_t pin);

class dallasSensor : public sensor
{
public:
  dallasSensor(uint8_t pin) : sensor(pin) {

    oneWire = OneWire(pin);
    _sensor = DallasTemperature(&oneWire);
    std::fill_n(thermometer, sizeof(thermometer), 0);
    initialization();
  }



private:
  OneWire oneWire;
  DallasTemperature _sensor;
  DeviceAddress thermometer;

  void initialization();
  void printAddress();
  void printAlarms();
  void printData();

  bool get();
};


#endif
