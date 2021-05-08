#pragma once
#ifndef _SENSOR_H
#define _SENSOR_H

#include <stdint.h>

class sensor
{
public:
    sensor(uint8_t pin);
    virtual float getTemperature();
    virtual void printTemperature();

protected:
    uint8_t _pin;
    float temperature;
    virtual bool get() = 0;
    virtual void initialization() = 0;
};

#endif