
#include "lm35sensor.h"
#include "sensor.h"
#include "configuration.h"


sensor *lm35Initialization(uint8_t pin) {
  lm35Sensor *tempSensor = new lm35Sensor(pin);
  tempSensor->getTemperature();
  tempSensor->printTemperature();
  return (sensor *)tempSensor;
}


bool lm35Sensor::get()
{
  uint_fast16_t ar = analogRead(_pin);
  float v = ADC_LUT[ar];
  //float mv = (v/adcMax)*Vs;
  float mv = (v/adcMax)*3.05;
  temperature = mv*100;

#ifdef debug  
  Serial.printf("Ar %d, Vout %f, mv %f, temp %f\n",ar, v, mv, temperature);
#endif  

//  temperature = runningAverage.filtered(medianFilter.filtered(temperature));

   return true;
}
