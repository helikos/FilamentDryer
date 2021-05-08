#pragma once

#ifndef _DHT_SENSOR_H
#define _DHT_SENSOR_H

#include <stdint.h>
#include <DHT_U.h>

#include "sensor.h"
#include "configuration.h"

class dhtSensor : public sensor
{

public:

  dhtSensor(uint8_t pin) : sensor(pin) {
   last_millis =  0;
   initialization();
  }; 

  float getHumidity() {
      get();
      return humidity;
  };

protected:
    DHT_Unified* DHT_Sensor;
    bool get();
    void initialization();
    int32_t delayMS;
    unsigned long last_millis;
    float humidity;

};

dhtSensor* dhtSensorInitialization(uint8_t pin);

#endif
