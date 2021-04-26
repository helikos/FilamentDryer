#include <Arduino.h>
#include <menu.h>
#include <menuIO/u8g2Out.h>
#include <stdint.h>
#include "EEPROM.h"

#include "FilamentSettings.h"
#include "configuration.h"
#include "sensors.h"
#include "sensors/sensor.h"

extern void writeDefaultSettings();
extern bool readSettings(dryerSetting &settings);
extern dryerSetting dryerParameters;
extern bool running;//lock menu if false

extern void mainMenuinitializate();
extern navRoot nav;

extern sensor* dallasSensorsInitialization(uint8_t pin);

sensor* tempSensor1;
sensor* tempSensor2;


//#include <GyverPWM.h>

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

int freeRam () {
 extern int __heap_start, *__brkval; 
 int v; 
 return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}


void setup() {
  Serial.begin(115200);
//  #ifdef debug
//  Serial.println(F("------------------------------------"));
//  Serial.print(F("Free memory")); Serial.println(freeRam()); 
//  Serial.println("Filament Dryer 0.1"); Serial.flush();
//  #endif

  /*
  * EEPROM initialization
  */
  /*
  EEPROM.begin(512);
  if (readSettings(dryerParameters)) {
    Serial.println("Reading was successful"); 
  } else {
    Serial.println("Reading was not successful"); 
  }
  */
  /*
  * Menu initialization
  */
  //mainMenuinitializate();
  /*
  * Sensor initialization
  */
  sensorsInitialization();
  tempSensor1 = dallasSensorsInitialization(DALLASTEMP_HEATER_PIN);
  tempSensor2 = dallasSensorsInitialization(DALLASTEMP_BOX_PIN);



}

void loop() {
/*
//  if (running) {
    nav.doInput();
    if (nav.changed(0)) {//only draw if menu changed for gfx device
      //change checking leaves more time for other tasks
      u8g2.firstPage();
      do nav.doOutput(); while(u8g2.nextPage());
    }
//  }  
*/
  delay(800);//simulate other tasks delay
//  tempSensor1->getTemperature();
//  tempSensor2->getTemperature();

}
