#ifndef _SENSOR_H
#define _SENSOR_H

class sensor
{
public:    
    virtual float getTemperature();

protected:
    virtual bool get();
    float temperature;
};

#endif