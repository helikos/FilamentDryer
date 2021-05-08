
//#include "sensors/dhtSensor.h"
//#include "sensors/dallasSensor.h"
#include "configuration.h"
#include <Arduino.h>


void sensorsInitialization()
{
#ifdef debug
    Serial.println(F("----------------- Sensor Initialization -------------------"));
#endif

//    dhtSensorInitialization();
//    dallasSensorsInitialization();

#ifdef debug
    Serial.println(F("----------------- done Sensor Initialization -------------------"));
#endif
}
