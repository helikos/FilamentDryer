#ifndef _DHT_SENSORS_H
#define _DHT_SENSORS_H

#include <DHT.h>
#include <DHT_U.h>


struct dhtSensor {
 
  DHT_Unified sensor; // (DHT_PIN, DHTTYPE);
  int32_t delayMS;
  float _temperature;
  float _humidity;
  float getTemperature() {
      get();
      return _temperature;
  };
  float getHumidity() {
      get();
      return _humidity;
  };
  unsigned long last_millis;
  bool get();
};

void dhtSensorInitialization();


#endif
