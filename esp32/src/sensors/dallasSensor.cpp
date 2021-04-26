#include <OneWire.h>
#include <DallasTemperature.h>

#include "configuration.h"
#include "dallasSensor.h"

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
//OneWire heaterSensorWire(DALLASTEMP_HEATER_PIN);
//OneWire boxSensorWire(DALLASTEMP_BOX_PIN);

// function to print a device address
void dallasSensor::printAddress() {
#ifdef debug    
  Serial.println("----- printAddress -------");
  for (uint8_t i = 0; i < sizeof(DeviceAddress); i++) Serial.printf("%02x", thermometer[i]);
  Serial.println("");
  Serial.println("----- done printAddress -------");
#endif  
}

// function to print the temperature for a device
void dallasSensor::printTemperature() {
#ifdef debug    
  Serial.println("----- printTemperature -------");
  float tempC = _sensor.getTempC(thermometer);
  Serial.printf("Temp °C: %f\n", tempC);
  Serial.printf("Temp °F: %f\n", DallasTemperature::toFahrenheit(tempC));
  Serial.println("----- done printTemperature -------");
#endif  
}

void dallasSensor::printAlarms() {
#ifdef debug    
  Serial.println("----- printAlarms -------");
  int8_t temp = _sensor.getHighAlarmTemp(thermometer);
  Serial.printf("High Alarm: %d °C\n", temp);
  Serial.printf("High Alarm: %f °F\n", DallasTemperature::toFahrenheit(temp));
  temp = _sensor.getLowAlarmTemp(thermometer);
  Serial.printf("Low Alarm: %d °C\n", temp);
  Serial.printf("Low Alarm: %f °F\n", DallasTemperature::toFahrenheit(temp));
  Serial.println("----- done printAlarms -------");
#endif  
}

// main function to print information about a device
void dallasSensor::printData() {
#ifdef debug    
  Serial.println("-------- printData --------");
  printAddress();
  printTemperature();
  Serial.println("---------- done printData -----------------------");
#endif  
}

void dallasSensor::initialization() {
  _sensor.begin();
#ifdef debug
  Serial.printf("Found %d devices.\n", _sensor.getDeviceCount());
#endif

  if (!_sensor.getAddress(thermometer, 0)) 
   Serial.println("Addres for device 0 was not found");  
  else { 
   printAddress();
   printAlarms();
  }
}   

sensor* dallasSensorsInitialization(uint8_t pin) {

//   DallasTemperature heater = DallasTemperature(&heaterSensorWire);
   dallasSensor* heaterSensor = new dallasSensor( pin );

   heaterSensor->getTemperature();
   heaterSensor->printTemperature();
   return (sensor*)heaterSensor;
}

bool dallasSensor::get() {

#ifdef debug
 Serial.println("Requesting temperatures...");
#endif 
  _sensor.requestTemperatures(); // Send the command to get temperatures
#ifdef debug
  Serial.println("done Requesting temperatures...");
#endif 
  _temperature = _sensor.getTempC(thermometer);
#ifdef debug
  Serial.printf("Temperature: %f °C\n", _temperature);
#endif 
  return true;
}