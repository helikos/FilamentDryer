#include <DHT.h>
#include <DHT_U.h>

#include "configuration.h"
#include "dhtSensor.h"

DHT_Unified _dhtSensor(DHT_PIN, DHTTYPE);


dhtSensor* dhtSensorInitialization(uint8_t pin) {

  dhtSensor *tempSensor = new dhtSensor(pin);
  tempSensor->getTemperature();
  tempSensor->printTemperature();
  return tempSensor;
}


void dhtSensor::initialization()
{
#ifdef debug
    Serial.println(F("----- DHTxx Unified Sensor Initialization -----"));
#endif
    sensor_t _sensor;
    this->DHT_Sensor = &_dhtSensor;
    this->DHT_Sensor ->begin();

    // Initialize DHT device.
 
        sensors_event_t temperatureEvent;
        this->DHT_Sensor->temperature().getEvent(&temperatureEvent);
        
        if (isnan(temperatureEvent.temperature))
        {
#ifdef debug
            Serial.println(F("Error reading temperature!"));
#endif
            temperature = -1;
        }
        else
        {
            temperature = temperatureEvent.temperature;
#ifdef debug
            Serial.print(F("Temperature: "));
            Serial.print(temperatureEvent.temperature);
            Serial.println(F("°C"));
#endif
        }

    // Print temperature sensor details.
    this->DHT_Sensor->temperature().getSensor(&_sensor);
#ifdef debug
    Serial.println(F("------------------------------------"));
    Serial.println(F("Temperature Sensor"));
    Serial.print(F("Sensor Type: "));
    Serial.println(_sensor.name);
    Serial.print(F("Driver Ver:  "));
    Serial.println(_sensor.version);
    Serial.print(F("Unique ID:   "));
    Serial.println(_sensor.sensor_id);
    Serial.print(F("Max Value:   "));
    Serial.print(_sensor.max_value);
    Serial.println(F("°C"));
    Serial.print(F("Min Value:   "));
    Serial.print(_sensor.min_value);
    Serial.println(F("°C"));
    Serial.print(F("Resolution:  "));
    Serial.print(_sensor.resolution);
    Serial.println(F("°C"));
    Serial.println(F("------------------------------------"));
    // Print humidity sensor details.
    this->DHT_Sensor->humidity().getSensor(&_sensor);
    Serial.println(F("Humidity Sensor"));
    Serial.print(F("Sensor Type: "));
    Serial.println(_sensor.name);
    Serial.print(F("Driver Ver:  "));
    Serial.println(_sensor.version);
    Serial.print(F("Unique ID:   "));
    Serial.println(_sensor.sensor_id);
    Serial.print(F("Max Value:   "));
    Serial.print(_sensor.max_value);
    Serial.println(F("%"));
    Serial.print(F("Min Value:   "));
    Serial.print(_sensor.min_value);
    Serial.println(F("%"));
    Serial.print(F("Resolution:  "));
    Serial.print(_sensor.resolution);
    Serial.println(F("%"));
    Serial.println(F("------------------------------------"));
#endif
    // Set delay between sensor readings based on sensor details.
    delayMS = _sensor.min_delay / 1000;

#ifdef debug
    Serial.print(F("delay:  "));
    Serial.print(delayMS);
    Serial.println("ms");
    Serial.printf("Temperature %f\n", getTemperature());
    Serial.printf("Humidity %f\n", getHumidity());
    Serial.println(F("----- done DHTxx Unified Sensor Initialization -----"));
#endif

}


bool dhtSensor::get()
{
#ifdef debug
    Serial.println(F("----- dhtSensor::get() -----"));
#endif
    unsigned long timer_ms = millis(); // reset of timer
    bool ret = true;
    if (last_millis + delayMS + 100 < timer_ms)
    {
        // Get temperature event and print its value.
        sensors_event_t temperatureEvent;
#ifdef debug
        Serial.printf("----- get ----- last_millis %lu delayMS %d timer_ms %lu\n", last_millis, delayMS, timer_ms);
#endif
        DHT_Sensor->temperature().getEvent(&temperatureEvent);
        if (isnan(temperatureEvent.temperature))
        {
#ifdef debug
            Serial.println(F("Error reading temperature!"));
#endif
            ret &= false;
            temperature = -1;
        }
        else
        {
            temperature = temperatureEvent.temperature;
#ifdef debug
            Serial.print(F("Temperature: "));
            Serial.print(temperatureEvent.temperature);
            Serial.println(F("°C"));
#endif
        }
        sensors_event_t humidityEvent;
        DHT_Sensor->humidity().getEvent(&humidityEvent);
        if (isnan(humidityEvent.relative_humidity))
        {
#ifdef debug
            Serial.println(F("Error reading humidity!"));
#endif
            ret &= false;
            humidity = -1;
        }
        else
        {
            humidity = humidityEvent.relative_humidity;
#ifdef debug
            Serial.print(F("Humidity: "));
            Serial.print(humidityEvent.relative_humidity);
            Serial.println(F("°C"));
#endif
        }
     last_millis = timer_ms;
    }
#ifdef debug
    Serial.println(F("----- done dhtSensor::get() -----"));
#endif
    return ret;
}
