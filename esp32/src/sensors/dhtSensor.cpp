#include <DHT.h>
#include <DHT_U.h>

#include "configuration.h"
#include "dhtSensor.h"

// Create DHT device.
struct dhtSensor boxSensor = {
    DHT_Unified(DHT_PIN, DHTTYPE)
    , 0 // delay
    ,0 // temperature
    ,0 // humidity
    ,0 // last_millis
};

// #include <OneWire.h>
// #include <DallasTemperature.h>

void dhtSensorInitialization()
{
#ifdef debug
    Serial.println(F("----- DHTxx Unified Sensor Initialization -----"));
#endif

    sensor_t _sensor;

    // Initialize DHT device.
    boxSensor.sensor.begin();
    // Print temperature sensor details.
    boxSensor.sensor.temperature().getSensor(&_sensor);
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
    boxSensor.sensor.humidity().getSensor(&_sensor);
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
    boxSensor.delayMS = _sensor.min_delay / 1000;

#ifdef debug
    Serial.print(F("delay:  "));
    Serial.print(boxSensor.delayMS);
    Serial.println("ms");
    Serial.printf("Temperature %f\n", boxSensor.getTemperature());
    Serial.printf("Humidity %f\n", boxSensor.getHumidity());
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
    if (last_millis + delayMS < timer_ms)
    {
#ifdef debug
        Serial.printf("----- get ----- last_millis %lu delayMS %d timer_ms %lu\n", last_millis, delayMS, timer_ms);
#endif
        // Get temperature event and print its value.
        sensors_event_t temperatureEvent;
        sensor.temperature().getEvent(&temperatureEvent);
        if (isnan(temperatureEvent.temperature))
        {
#ifdef debug
            Serial.println(F("Error reading temperature!"));
#endif
            ret &= false;
            _temperature = -1;
        }
        else
        {
            _temperature = temperatureEvent.temperature;
#ifdef debug
            Serial.print(F("Temperature: "));
            Serial.print(temperatureEvent.temperature);
            Serial.println(F("°C"));
#endif
        }
        sensors_event_t humidityEvent;
        sensor.humidity().getEvent(&humidityEvent);
        if (isnan(humidityEvent.relative_humidity))
        {
#ifdef debug
            Serial.println(F("Error reading humidity!"));
#endif
            ret &= false;
            _humidity = -1;
        }
        else
        {
            _humidity = humidityEvent.relative_humidity;
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
