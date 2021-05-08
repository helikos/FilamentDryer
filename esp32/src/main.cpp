#include <Arduino.h>
#include <menu.h>
#include <menuIO/u8g2Out.h>
#include <stdint.h>
#include "EEPROM.h"

#include "FilamentSettings.h"
#include "configuration.h"
#include "sensors.h"
#include "sensors/sensor.h"

#include "sensors/ntc3950sensor.h"
#include "sensors/dhtSensor.h"
#include "sensors/lm35sensor.h"

//#include <analogWrite.h>

#include <PID_v1.h>


//Define Variables we'll be connecting to
double Setpoint, Input, Output;
 
//Specify the links and initial tuning parameters
double Kp=2, Ki=5, Kd=1;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

extern void writeDefaultSettings();
extern bool readSettings(dryerSetting &settings);
extern dryerSetting dryerParameters;
extern bool running;//lock menu if false

extern void mainMenuinitializate();
extern navRoot nav;

extern sensor* dallasSensorsInitialization(uint8_t pin);
extern sensor* ntc3950Initialization(uint8_t pin);
extern sensor* lm35Initialization(uint8_t pin);

extern dhtSensor* dhtSensorInitialization(uint8_t pin);

sensor* tempSensor1;
sensor* tempSensor2;
sensor* tempSensor3;
sensor* tempSensor5;
sensor* tempSensor6;

dhtSensor* tempSensor4;

int fanfreq = 20000;
int fanChannel = 0;
int heaterfreq = 10000;
int heaterChannel = 1;

  int resolution = 8;


bool dir = true;
unsigned long tmr;
uint8_t val = 0;
unsigned long last_millis = 3000;

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
  //sensorsInitialization();
  
  tempSensor1 = dallasSensorsInitialization(DALLASTEMP_HEATER_PIN);
  tempSensor2 = dallasSensorsInitialization(DALLASTEMP_BOX_PIN);
  tempSensor3 = ntc3950Initialization(NTC3950_PIN);
  tempSensor4 = dhtSensorInitialization(DHT_PIN);
  tempSensor5 = ntc3950Initialization(35);
  tempSensor6 = lm35Initialization(33);

  pinMode(HEATER_LED_PIN, OUTPUT);
  pinMode(LED_PIN_1, OUTPUT);

  
  ledcSetup(fanChannel, fanfreq, resolution);
  ledcSetup(heaterChannel, heaterfreq, resolution);

  ledcAttachPin(HEATER_FAN_PIN, fanChannel);
  ledcAttachPin(HEATER_PIN, heaterChannel);

   ledcWrite(fanChannel, 50);

//  Input = analogRead(PIN_INPUT);
  Setpoint = 40;

  //turn the PID on
  myPID.SetMode(AUTOMATIC);


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
/*
  analogWrite(HEATER_FAN_PIN, 255, 255);
  analogWrite(HEATER_PIN, 255, 255);
*/
//    unsigned long timer_ms = millis(); // reset of timer
//    if (last_millis >= timer_ms) {
//      ledcWrite(fanChannel, 255);
//      Serial.println(" 255 ");
//    } else {
//    }
   



   delay(500);//simulate other tasks delay
    dir = !dir; // разворачиваем
    if (dir) {
      digitalWrite(HEATER_LED_PIN, LOW);
      digitalWrite(LED_PIN_1, HIGH);

    }
    else{
      digitalWrite(HEATER_LED_PIN, HIGH);
      digitalWrite(LED_PIN_1, LOW);
    }
    
/*
  if (abs(tempSensor3->getTemperature()) > 80) {
      analogWrite(HEATER_PIN, 0,255);
      Serial.println("turn off heater");
  }
  else {
       analogWrite(HEATER_PIN, 255,255);
      Serial.println("turn on heater");
  }
  */

  tempSensor1->printTemperature();
  tempSensor2->printTemperature();
  tempSensor3->printTemperature();
  tempSensor5->printTemperature();
  tempSensor6->printTemperature();

  tempSensor4->printTemperature();

  Serial.printf("Humidity %f\n", tempSensor4->getHumidity());
  
  Input = tempSensor1->getTemperature();
  myPID.Compute();
  Serial.printf("Input %f Output %f\n", Input, Output);
  
  ledcWrite(heaterChannel, trunc(Output));


}
