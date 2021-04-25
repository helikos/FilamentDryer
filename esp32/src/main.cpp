#include <Arduino.h>

#include "configuration.h"
#include <menu.h>
#include <menuIO/u8g2Out.h>

#include <Wire.h>

#include "FilamentSettings.h"
#include <stdint.h>


#include "EEPROM.h"


extern void writeDefaultSettings();
extern bool readSettings(dryerSetting &settings);
extern dryerSetting dryerParameters;
extern bool running;//lock menu if false

extern void mainMenuinitializate();
extern navRoot nav;

// #include <DHT.h>
// #include <DHT_U.h>

// #include <OneWire.h>
// #include <DallasTemperature.h>


//#include <GyverPWM.h>



U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, SCREEW_SCL_PIN, SCREEW_SDA_PIN);

/*

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire tempSensorWire(DALLASTEMP_PIN);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&tempSensorWire);
DeviceAddress insideThermometer;

// Create DHT device.
DHT_Unified dht(DHT_PIN, DHTTYPE);


uint32_t delayMS;
uint32_t tmr;
int val = 0;
bool dir = true;



unsigned long period_time_ms = (long)2*1000;    // 2 sec
// переменная таймера, максимально большой целочисленный тип (он же uint32_t)
unsigned long my_timer_ms;

#ifdef debug
// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  Serial.print("Temp °C: ");
  Serial.print(tempC);
  Serial.print(" Temp °F: ");
  Serial.print(DallasTemperature::toFahrenheit(tempC));
}

void printAlarms(uint8_t deviceAddress[])
{
  char temp;
  temp = sensors.getHighAlarmTemp(deviceAddress);
  Serial.print("High Alarm: ");
  Serial.print(temp, DEC);
  Serial.print("C/");
  Serial.print(DallasTemperature::toFahrenheit(temp));
  Serial.print("F | Low Alarm: ");
  temp = sensors.getLowAlarmTemp(deviceAddress);
  Serial.print(temp, DEC);
  Serial.print("°C/");
  Serial.print(DallasTemperature::toFahrenheit(temp));
  Serial.print("°F");
}

// main function to print information about a device
void printData(DeviceAddress deviceAddress)
{
  Serial.print("Device Address: ");
  printAddress(deviceAddress);
  Serial.print(" ");
  printTemperature(deviceAddress);
  Serial.println();
}

void checkAlarm(DeviceAddress deviceAddress)
{
  if (sensors.hasAlarm(deviceAddress))
  {
    Serial.print("ALARM: ");
    printData(deviceAddress);
  }
}
#endif
*/

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
  oled.begin(&Adafruit128x64, I2C_ADDRESS);
  oled.setFont(menuFont);

  oled.clear();
  oled.setCursor(0, 0);
  oled.print(F("Filament Dryer 0.1"));
  delay(2000);
  oled.clear();
*/
  EEPROM.begin(512);

//  writeDefaultSettings();
 
  if (readSettings(dryerParameters)) {
    Serial.println("Reading was successful"); 
  } else {
    Serial.println("Reading was not successful"); 
  }
  mainMenuinitializate();

/*

  // Initialize DHT device.
  dht.begin();
  Serial.println(F("DHTxx Unified Sensor Example"));
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  #ifdef debug
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
  #endif
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
  #ifdef debug
  Serial.print  (F("delay:  ")); Serial.print(delayMS); Serial.println("ms");
  Serial.println(F("------------------------------------"));
  Serial.println("Dallas Temperature IC Control Library Demo");
  #endif
  
  pinMode(4, OUTPUT);
  sensors.begin();


  // Start up the library
  // locate devices on the bus
  #ifdef debug
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");
  if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Не найден адрес датчика 0");  

  Serial.print("Device Address: ");
  printAddress(insideThermometer);
  Serial.println();

  Serial.println("Setting alarm temps...");
  // alarm when temp is higher than 30C
  sensors.setHighAlarmTemp(insideThermometer, 30);
  
  // alarm when temp is lower than -10C
  sensors.setLowAlarmTemp(insideThermometer, -10);
  
  Serial.print("New Device 0 Alarms: ");
  printAlarms(insideThermometer);
  Serial.println();
  #endif
  
  pinMode(3, OUTPUT);
  PWM_frequency(3, 100000, FAST_PWM);
*/

}



void loop() {

//  if (running) {
    nav.doInput();
    if (nav.changed(0)) {//only draw if menu changed for gfx device
      //change checking leaves more time for other tasks
      u8g2.firstPage();
      do nav.doOutput(); while(u8g2.nextPage());
    }
//  }  

  delay(100);//simulate other tasks delay


}


