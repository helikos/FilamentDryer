#ifndef _CONFIGURATION_H
#define _CONFIGURATION_H

#define debug

/* Macros for getting elapsed time */
#define numberOfSeconds(_time_) ((_time_ / 1000) % 60)
#define numberOfMinutes(_time_) (((_time_ / 1000) / 60) % 60)
#define numberOfHours(_time_) (((_time_ / 1000) / 60) / 60)
#define numberOfDays(_time_) ((((_time_ / 1000) / 60) / 60) / 60)

#define MAX_TEMP 120
#define MAX_HEATER_TEMP 150
#define MAX_FAN_SPEED 100
#define MAX_MINUTES 24 * 60
#define MAX_PID 5

#define MIN_TEMP 0
#define MIN_HEATER_TEMP 0
#define MIN_FAN_SPEED 0
#define MIN_MINUTES 1
#define MIN_PID 0

#define STEP_TEMP 10  // step of changing the temperature
#define STEP_TEMP_2 1 // the second step of changing the temperature
#define STEP_HEATER_TEMP 10
#define STEP_HEATER_TEMP_2 11
#define STEP_FAN_SPEED 10
#define STEP_FAN_SPEED_2 1
#define STEP_MINUTES 10
#define STEP_MINUTES_2 1
#define STEP_PID 0.1
#define STEP_PID_2 0.01

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C

//#define SCREEN_WIDTH 128 // OLED display width, in pixels
//#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
//#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

#define DHTTYPE DHT22 // DHT 22 (AM2302)

/*
 Pins
*/

#define SCREEW_SCL_PIN 21
#define SCREEW_SDA_PIN 22

#define DHT_PIN               19        // Digital pin connected to the DHT sensor
#define DALLASTEMP_HEATER_PIN 5         // Heater Digital pin connected to the Dallas Temperature sensor
#define DALLASTEMP_BOX_PIN    18        // Heater Digital pin connected to the Dallas Temperature sensor

#define HEATER_LED_PIN 23 //
#define LED_PIN        27 //
#define HEATER_PIN     26     //
#define HEATER_FAN_PIN 25        //
#define BOX_FAN_PIN    -1        //


#define ENCODER_SW_PIN 4   // Digital pin connected to the encoder switch
#define ENCODER_CLK_PIN 15 // Digital pin connected to the encoder CLK
#define ENCODER_DT_PIN 2   // Digital pin connected to the encoder DT
#define ENCODER_STEPS_PER_NOTCH 1



// Screen Configuration
#define U8_Width 128
#define U8_Height 64
#define fontName u8g2_font_7x13_mf
#define fontX 7
#define fontY 16

#define default_PID_Kp 1
#define default_PID_Ki 1
#define default_PID_Kd 1

#define COUNT_TYPE_OF_FILAMENT 11

#define FILAMENT_ABS "ABS" 
#define FILAMENT_PLA "PLA" 
#define FILAMENT_NYLON "Nylon" 
#define FILAMENT_PETG "PETG" 
#define FILAMENT_TPU "TPU" 
#define FILAMENT_PVA "PVA" 
#define FILAMENT_CUSTOM_1 "Custom 1" 
#define FILAMENT_CUSTOM_2 "Custom 2" 
#define FILAMENT_CUSTOM_3 "Custom 3" 
#define FILAMENT_CUSTOM_4 "Custom 4" 
#define FILAMENT_TEST "Test" 


#endif
