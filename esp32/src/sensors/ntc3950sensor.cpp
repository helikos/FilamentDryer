
#include "ntc3950sensor.h"
#include "sensor.h"
#include "configuration.h"


//GFilterRA – компактная альтернатива фильтра экспоненциальное бегущее среднее (Running Average)
//GMedian3

// The ESP32 lookup table varies from device to device, use the program from 
// https://github.com/e-tinkers/esp32-adc-calibrate
// to generate lookup table for your own ESP32



sensor *ntc3950Initialization(uint8_t pin) {

  ntc3950Sensor *tempSensor = new ntc3950Sensor(pin);
  tempSensor->getTemperature();
  tempSensor->printTemperature();
  return (sensor *)tempSensor;

}


void ntc3950Sensor::initialization()
{


}

bool ntc3950Sensor::get()
{
  uint_fast16_t ar = analogRead(_pin);
  float v = ADC_LUT[ar];
  float Vout =  v * Vs/adcMax;
  float Rt = R1 * Vout / (Vs - Vout);
  float t = 1 / (1/To + log(Rt/Ro)*(1/Beta));  // Temperature in Kelvin
  temperature = t - 273.15;
#ifdef debug  
  Serial.printf("Ar %d, Vout %f, Rt %f, t %f, temp %f\n",ar, Vout,Rt,t, temperature);
#endif  
//  float ra = runningAverage.filtered(temperature);
//  Serial.printf("temp %f, median %f runningAverage %f\n", temperature, medianFilter.filtered(temperature), runningAverage.filtered(medianFilter.filtered(temperature)));
//  temperature = ringAverage.filteredFloat(medianFilter.filtered(t - 273.15));// Celsius
//  Tf = Tc * 9 / 5 + 32;            // Fahrenheit

  temperature = runningAverage.filtered(medianFilter.filtered(temperature));

  return true;
}
