#ifndef _LM_35_SENSOR_H
#define _LM_35_SENSOR_H


#include <Arduino.h>
#include "sensor.h"

sensor* lm35Initialization(uint8_t pin);

class lm35Sensor : public sensor
{
public:
    lm35Sensor(uint8_t pin) : sensor(pin) {
    initialization();
  }

  float getTemperature()
  {
    get();
    return temperature;
  };

private:
  void initialization(){};
  bool get();
//  RingAverage<float,50>  ringAverage;
};



#endif