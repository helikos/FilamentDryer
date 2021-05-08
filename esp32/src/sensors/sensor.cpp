#include "sensor.h"
#include "configuration.h"
#include <Arduino.h>

sensor::sensor(uint8_t pin) {
    _pin = pin;
};

float sensor::getTemperature()
{
    get();
    return temperature;
};

// function to print the temperature for a device
void sensor::printTemperature()
{
//#ifdef debug
    get();
    Serial.printf("%d pin Temp °C: %f\n", _pin, temperature);
//#endif
}