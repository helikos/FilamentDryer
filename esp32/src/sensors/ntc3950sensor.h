#ifndef _NTC_3950_SENSOR_H
#define _NTC_3950_SENSOR_H

#include <Arduino.h>
#include "sensor.h"
#include "filters/median3.h"
#include "filters/RingAverage.h"
#include "filters/runningAverage.h"


#define  R1      100000.0       // voltage divider resistor value
#define  Beta    3950.0         // Beta value
#define  To      298.15         // Temperature in Kelvin for 25 degree Celsius 
#define  Ro      100000.0       // Resistance of Thermistor at 25 degree Celsius

sensor* ntc3950Initialization(uint8_t pin);

class ntc3950Sensor : public sensor
{
public:
  ntc3950Sensor(uint8_t pin) : sensor(pin) {
    initialization();
    runningAverage.setCoef(0.5);
    runningAverage.setStep(100);
  }

  float getTemperature()
  {
    get();
    return temperature;
  };

private:
  void initialization();
  bool get();
//  RingAverage<float,50>  ringAverage;
  GMedian3<float>       medianFilter;
  GFilterRA             runningAverage;
};

#endif