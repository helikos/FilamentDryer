#include <ClickEncoder.h>  // Using this library: https://github.com/soligen2010/encoder.git
#include <Wire.h>
#include <array>
#include <menu.h>
#include <menuIO/u8g2Out.h>

#include <menuIO/clickEncoderIn.h>
#include <menuIO/chainStream.h>
#include <menuIO/serialOut.h>
#include <menuIO/serialIn.h>

#include "MainMenu.h"
#include "configuration.h"
#include "FilamentSettings.h"
#include "FilamentDryer.h"
#include "DoMenu.h"
//#include "sensors/dhtSensor.h"

extern "C" {
  #include "freertos/timers.h"
}

using namespace Menu;

#define offsetX   0
#define offsetY   3
#define MAX_DEPTH 5

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, SCREEW_SCL_PIN, SCREEW_SDA_PIN);
extern dryerSetting dryerParameters;
//extern dhtSensor  boxSensor;

bool running=true;//lock menu if false
const colorDef<uint8_t> colors[6] MEMMODE={
  {{0,0},{0,1,1}},//bgColor
  {{1,1},{1,0,0}},//fgColor
  {{1,1},{1,0,0}},//valColor
  {{1,1},{1,0,0}},//unitColor
  {{0,1},{0,0,1}},//cursorColor
  {{1,1},{1,0,0}},//titleColor
};

//using the customized menu class
//note that first parameter is the class name
altMENU(confirmSave,saveMenu,"Save",doNothing,noEvent,wrapStyle,(Menu::_menuData|Menu::_canNav)
  ,OP("Yes", menuSaveExecute, enterEvent)
  ,EXIT("Cancel")
);

result test(eventMask e,navNode& nav,prompt& item){
#ifdef debug
    Serial.println(F("----------------- test -------------------"));
#endif
  return proceed;
}

altMENU(confirmExit,exitMenu,"Exit",doNothing,noEvent,wrapStyle,(Menu::_menuData|Menu::_canNav)
  ,OP("Yes", test, enterEvent)
  ,EXIT("Cancel")
);

altMENU(confirmRestoreDefault,restoryDefaultMenu,"Restory Default",doNothing,noEvent,wrapStyle,(Menu::_menuData|Menu::_canNav)
  ,OP("Yes", settingsMenuRestoreDefaultExecute, enterEvent)
  ,EXIT("Cancel")
);



CHOOSE(dryerParameters.lastChoosenMaterial, chooseMaterialMenu, "Material", doNothing, noEvent, wrapStyle
       , VALUE(FILAMENT_ABS, _abs, doNothing, noEvent)
       , VALUE(FILAMENT_PLA, _pla, doNothing, noEvent)
       , VALUE(FILAMENT_PETG, _petg, doNothing, noEvent)
       , VALUE(FILAMENT_NYLON, _nylon, doNothing, noEvent)
       , VALUE(FILAMENT_TPU, _tpu, doNothing, noEvent)
       , VALUE(FILAMENT_PVA, _pva, doNothing, noEvent)
       , VALUE(FILAMENT_CUSTOM_1, _custom1, doNothing, noEvent)
       , VALUE(FILAMENT_CUSTOM_2, _custom2, doNothing, noEvent)
       , VALUE(FILAMENT_CUSTOM_3, _custom3, doNothing, noEvent)
       , VALUE(FILAMENT_CUSTOM_4, _custom4, doNothing, noEvent)
       , VALUE(FILAMENT_TEST, _test, doNothing, noEvent)
      );

MENU(autoDryingMenu, "Auto Drying", doNothing, anyEvent, wrapStyle
     , SUBMENU(chooseMaterialMenu)
     , OP("Start", autoDryingMenuStartExecute, anyEvent)
     , EXIT("<Back")
    );


MENU(manualDryingMenu, "Manual Drying", doNothing, anyEvent, wrapStyle
     , FIELD(dryerParameters.manualParam.param.minutes, "Minutes", "min", MIN_MINUTES, MAX_MINUTES, STEP_MINUTES, STEP_MINUTES_2, doNothing, noEvent, wrapStyle)
     , FIELD(dryerParameters.manualParam.param.temp, "Temp", "C", MIN_TEMP, MAX_TEMP, STEP_TEMP, STEP_TEMP_2, doNothing, noEvent, wrapStyle)
     , FIELD(dryerParameters.manualParam.param.fanSpeed, "Fan Speed", "%", MIN_FAN_SPEED, MAX_FAN_SPEED, STEP_FAN_SPEED_2, STEP_FAN_SPEED_2, doNothing, noEvent, wrapStyle)
//     , OP("Save", manualDryingMenuSaveExecute, anyEvent)
     , SUBMENU(saveMenu)
     , OP("Start", manualDryingMenuStartExecute, anyEvent)
     , EXIT("<Back")
    );


MENU(pidValueMenu, "Pids Value", doNothing, anyEvent, wrapStyle
     , FIELD(dryerParameters.pidParam.param.Kp, "Kp", "", MIN_PID, MAX_PID, STEP_PID, STEP_PID_2, doNothing, noEvent, wrapStyle)
     , FIELD(dryerParameters.pidParam.param.Ki, "Ki", "", MIN_PID, MAX_PID, STEP_PID, STEP_PID_2, doNothing, noEvent, wrapStyle)
     , FIELD(dryerParameters.pidParam.param.Kd, "Kd", "", MIN_PID, MAX_PID, STEP_PID, STEP_PID_2, doNothing, noEvent, wrapStyle)
     , SUBMENU(saveMenu)
//     , OP("Save", pidValueMenuSaveExecute, anyEvent)
     , EXIT("<Back")
    );

MENU(pidMenu, "PID", doNothing, anyEvent, wrapStyle
     , SUBMENU(pidValueMenu)
     , OP("AutoTune", pidMenuAutoTuneStartExecute, anyEvent)
     , EXIT("<Back")
    );

MENU(absMenu, FILAMENT_ABS, doNothing, anyEvent, wrapStyle
     , FIELD(dryerParameters.filamentParams[_abs].param.temp, "Temp", "", MIN_TEMP, MAX_TEMP, STEP_TEMP, STEP_TEMP_2, doNothing, noEvent, wrapStyle)
     , FIELD(dryerParameters.filamentParams[_abs].param.minutes, "Minutes", "", MIN_MINUTES, MAX_MINUTES, STEP_MINUTES, STEP_MINUTES_2, doNothing, noEvent, wrapStyle)
     , FIELD(dryerParameters.filamentParams[_abs].param.fanSpeed, "Fan Speed", "", MIN_FAN_SPEED, MAX_FAN_SPEED, STEP_FAN_SPEED, STEP_FAN_SPEED_2, doNothing, noEvent, wrapStyle)
     , SUBMENU(saveMenu)
     , EXIT("<Back")
    );

MENU(plaMenu, FILAMENT_PLA, doNothing, anyEvent, wrapStyle
     , FIELD(dryerParameters.filamentParams[_pla].param.temp, "Temp", "", MIN_TEMP, MAX_TEMP, STEP_TEMP, STEP_TEMP_2, doNothing, noEvent, wrapStyle)
     , FIELD(dryerParameters.filamentParams[_pla].param.minutes, "Minutes", "", MIN_MINUTES, MAX_MINUTES, STEP_MINUTES, STEP_MINUTES_2, doNothing, noEvent, wrapStyle)
     , FIELD(dryerParameters.filamentParams[_pla].param.fanSpeed, "Fan Speed", "", MIN_FAN_SPEED, MAX_FAN_SPEED, STEP_FAN_SPEED, STEP_FAN_SPEED_2, doNothing, noEvent, wrapStyle)
     , SUBMENU(saveMenu)
     , EXIT("<Back")
    );

MENU(petgMenu, FILAMENT_PETG, doNothing, anyEvent, wrapStyle
     , FIELD(dryerParameters.filamentParams[_petg].param.temp, "Temp", "", MIN_TEMP, MAX_TEMP, STEP_TEMP, STEP_TEMP_2, doNothing, noEvent, wrapStyle)
     , FIELD(dryerParameters.filamentParams[_petg].param.minutes, "Minutes", "", MIN_MINUTES, MAX_MINUTES, STEP_MINUTES, STEP_MINUTES_2, doNothing, noEvent, wrapStyle)
     , FIELD(dryerParameters.filamentParams[_petg].param.fanSpeed, "Fan Speed", "", MIN_FAN_SPEED, MAX_FAN_SPEED, STEP_FAN_SPEED, STEP_FAN_SPEED_2, doNothing, noEvent, wrapStyle)
     , SUBMENU(saveMenu)
     , EXIT("<Back")
    );

MENU(nylonMenu, FILAMENT_NYLON, doNothing, anyEvent, wrapStyle
     , FIELD(dryerParameters.filamentParams[_nylon].param.temp, "Temp", "", MIN_TEMP, MAX_TEMP, STEP_TEMP, STEP_TEMP_2, doNothing, noEvent, wrapStyle)
     , FIELD(dryerParameters.filamentParams[_nylon].param.minutes, "Minutes", "", MIN_MINUTES, MAX_MINUTES, STEP_MINUTES, STEP_MINUTES_2, doNothing, noEvent, wrapStyle)
     , FIELD(dryerParameters.filamentParams[_nylon].param.fanSpeed, "Fan Speed", "", MIN_FAN_SPEED, MAX_FAN_SPEED, STEP_FAN_SPEED, STEP_FAN_SPEED_2, doNothing, noEvent, wrapStyle)
     , SUBMENU(saveMenu)
     , EXIT("<Back")
    );

MENU(tpuMenu, FILAMENT_TPU, doNothing, anyEvent, wrapStyle
     , FIELD(dryerParameters.filamentParams[_tpu].param.temp, "Temp", "", MIN_TEMP, MAX_TEMP, STEP_TEMP, STEP_TEMP_2, doNothing, noEvent, wrapStyle)
     , FIELD(dryerParameters.filamentParams[_tpu].param.minutes, "Minutes", "", MIN_MINUTES, MAX_MINUTES, STEP_MINUTES, STEP_MINUTES_2, doNothing, noEvent, wrapStyle)
     , FIELD(dryerParameters.filamentParams[_tpu].param.fanSpeed, "Fan Speed", "", MIN_FAN_SPEED, MAX_FAN_SPEED, STEP_FAN_SPEED, STEP_FAN_SPEED_2, doNothing, noEvent, wrapStyle)
     , SUBMENU(saveMenu)
     , EXIT("<Back")
    );

MENU(pvaMenu, FILAMENT_PVA, doNothing, anyEvent, wrapStyle
     , FIELD(dryerParameters.filamentParams[_pva].param.temp, "Temp", "", MIN_TEMP, MAX_TEMP, STEP_TEMP, STEP_TEMP_2, doNothing, noEvent, wrapStyle)
     , FIELD(dryerParameters.filamentParams[_pva].param.minutes, "Minutes", "", MIN_MINUTES, MAX_MINUTES, STEP_MINUTES, STEP_MINUTES_2, doNothing, noEvent, wrapStyle)
     , FIELD(dryerParameters.filamentParams[_pva].param.fanSpeed, "Fan Speed", "", MIN_FAN_SPEED, MAX_FAN_SPEED, STEP_FAN_SPEED, STEP_FAN_SPEED_2, doNothing, noEvent, wrapStyle)
     , SUBMENU(saveMenu)
     , EXIT("<Back")
    );

MENU(custom1Menu, FILAMENT_CUSTOM_1, doNothing, anyEvent, wrapStyle
     , FIELD(dryerParameters.filamentParams[_custom1].param.temp, "Temp", "", MIN_TEMP, MAX_TEMP, STEP_TEMP, STEP_TEMP_2, doNothing, noEvent, wrapStyle)
     , FIELD(dryerParameters.filamentParams[_custom1].param.minutes, "Minutes", "", MIN_MINUTES, MAX_MINUTES, STEP_MINUTES, STEP_MINUTES_2, doNothing, noEvent, wrapStyle)
     , FIELD(dryerParameters.filamentParams[_custom1].param.fanSpeed, "Fan Speed", "", MIN_FAN_SPEED, MAX_FAN_SPEED, STEP_FAN_SPEED, STEP_FAN_SPEED_2, doNothing, noEvent, wrapStyle)
     , SUBMENU(saveMenu)
     , EXIT("<Back")
    );

MENU(custom2Menu, FILAMENT_CUSTOM_2, doNothing, anyEvent, wrapStyle
     , FIELD(dryerParameters.filamentParams[_custom2].param.temp, "Temp", "", MIN_TEMP, MAX_TEMP, STEP_TEMP, STEP_TEMP_2, doNothing, noEvent, wrapStyle)
     , FIELD(dryerParameters.filamentParams[_custom2].param.minutes, "Minutes", "", MIN_MINUTES, MAX_MINUTES, STEP_MINUTES, STEP_MINUTES_2, doNothing, noEvent, wrapStyle)
     , FIELD(dryerParameters.filamentParams[_custom2].param.fanSpeed, "Fan Speed", "", MIN_FAN_SPEED, MAX_FAN_SPEED, STEP_FAN_SPEED, STEP_FAN_SPEED_2, doNothing, noEvent, wrapStyle)
     , SUBMENU(saveMenu)
     , EXIT("<Back")
    );

MENU(custom3Menu, FILAMENT_CUSTOM_3, doNothing, anyEvent, wrapStyle
     , FIELD(dryerParameters.filamentParams[_custom3].param.temp, "Temp", "", MIN_TEMP, MAX_TEMP, STEP_TEMP, STEP_TEMP_2, doNothing, noEvent, wrapStyle)
     , FIELD(dryerParameters.filamentParams[_custom3].param.minutes, "Minutes", "", MIN_MINUTES, MAX_MINUTES, STEP_MINUTES, STEP_MINUTES_2, doNothing, noEvent, wrapStyle)
     , FIELD(dryerParameters.filamentParams[_custom3].param.fanSpeed, "Fan Speed", "", MIN_FAN_SPEED, MAX_FAN_SPEED, STEP_FAN_SPEED, STEP_FAN_SPEED_2, doNothing, noEvent, wrapStyle)
     , SUBMENU(saveMenu)
     , EXIT("<Back")
    );

MENU(custom4Menu, FILAMENT_CUSTOM_4, doNothing, anyEvent, wrapStyle
     , FIELD(dryerParameters.filamentParams[_custom4].param.temp, "Temp", "", MIN_TEMP, MAX_TEMP, STEP_TEMP, STEP_TEMP_2, doNothing, noEvent, wrapStyle)
     , FIELD(dryerParameters.filamentParams[_custom4].param.minutes, "Minutes", "", MIN_MINUTES, MAX_MINUTES, STEP_MINUTES, STEP_MINUTES_2, doNothing, noEvent, wrapStyle)
     , FIELD(dryerParameters.filamentParams[_custom4].param.fanSpeed, "Fan Speed", "", MIN_FAN_SPEED, MAX_FAN_SPEED, STEP_FAN_SPEED, STEP_FAN_SPEED_2, doNothing, noEvent, wrapStyle)
     , SUBMENU(saveMenu)
     , EXIT("<Back")
    );

MENU(testMenu, FILAMENT_TEST, doNothing, anyEvent, wrapStyle
     , FIELD(dryerParameters.filamentParams[_test].param.temp, "Temp", "", MIN_TEMP, MAX_TEMP, STEP_TEMP, STEP_TEMP_2, doNothing, noEvent, wrapStyle)
     , FIELD(dryerParameters.filamentParams[_test].param.minutes, "Minutes", "", MIN_MINUTES, MAX_MINUTES, STEP_MINUTES, STEP_MINUTES_2, doNothing, noEvent, wrapStyle)
     , FIELD(dryerParameters.filamentParams[_test].param.fanSpeed, "Fan Speed", "", MIN_FAN_SPEED, MAX_FAN_SPEED, STEP_FAN_SPEED, STEP_FAN_SPEED_2, doNothing, noEvent, wrapStyle)
     , SUBMENU(saveMenu)
     , EXIT("<Back")
    );

MENU(presetMenu, "Preset", doNothing, anyEvent, wrapStyle
     , SUBMENU(absMenu)
     , SUBMENU(plaMenu)
     , SUBMENU(petgMenu)
     , SUBMENU(nylonMenu)
     , SUBMENU(tpuMenu)
     , SUBMENU(pvaMenu)
     , SUBMENU(custom1Menu)
     , SUBMENU(custom2Menu)
     , SUBMENU(custom3Menu)
     , SUBMENU(custom4Menu)
     , SUBMENU(testMenu)
     , SUBMENU(saveMenu)
     , EXIT("<Back")
    );

MENU(temperatureMenu, "Temperature", doNothing, anyEvent, wrapStyle
     , FIELD(dryerParameters.temperatureParam.param.heaterTemperature, "Heater Temp", "C", 0, 100, 10, 1, doNothing, noEvent, wrapStyle)
     , FIELD(dryerParameters.temperatureParam.param.boxTemperature, "Box Temp", "C", 0, 100, 10, 1, doNothing, noEvent, wrapStyle)
     , SUBMENU(saveMenu)
     , EXIT("<Back")
    );

MENU(settingsMenu, "Settings", doNothing, anyEvent, wrapStyle
     , SUBMENU(pidMenu)
     , SUBMENU(presetMenu)
     , SUBMENU(temperatureMenu)
     , SUBMENU(restoryDefaultMenu)
     , EXIT("<Back")
    );

MENU(mainMenu, "Fylament Dryer", doNothing, noEvent, wrapStyle
    ,SUBMENU(autoDryingMenu)
    ,SUBMENU(manualDryingMenu)
    ,SUBMENU(settingsMenu)    
    );

// Declare the clickencoder
// Disable doubleclicks in setup makes the response faster.  See: https://github.com/soligen2010/encoder/issues/6
ClickEncoder clickEncoder = ClickEncoder(ENCODER_CLK_PIN, ENCODER_DT_PIN, ENCODER_SW_PIN, ENCODER_STEPS_PER_NOTCH);
ClickEncoderStream encStream(clickEncoder, 1);

#ifdef debug
serialIn serial(Serial);
MENU_INPUTS(in, &encStream, &serial);
#else
MENU_INPUTS(in, &encStream);
#endif

#ifdef debug
MENU_OUTPUTS(out, MAX_DEPTH
  ,U8G2_OUT(u8g2,colors,fontX,fontY,offsetX,offsetY,{0,0,U8_Width/fontX,U8_Height/fontY})
  ,SERIAL_OUT(Serial)
);
#else
MENU_OUTPUTS(out, MAX_DEPTH
  ,U8G2_OUT(u8g2,colors,fontX,fontY,offsetX,offsetY,{0,0,U8_Width/fontX,U8_Height/fontY})
  ,NONE
);
#endif


//macro to create navigation control root object (nav) using mainMenu
NAVROOT(nav, mainMenu, MAX_DEPTH, in, out);

result systemExit() {
  Serial.println();
  Serial.println("Terminating...");
  //do some termiination stuff here
  running=false;//prevents the menu from running again!
  Serial.println("done.");
  nav.idleOn();//suspend the menu system
  return quit;
};

//when menu is suspended
result MyIdleOn(menuOut& o,idleEvent e) {
  o.clear();
  switch(e) {
    case idleStart:o.println("suspending menu!");break;
    case idling: 
Serial.println("Idle");    
  u8g2.firstPage();
  do {
    u8g2.setColorIndex(1);
    nav.out[0].setCursor(0,0);
//    nav.out[0].printf("Temp %f",boxSensor.getTemperature());
    nav.out[0].setCursor(0,1);
//    nav.out[0].printf("Humidity %f", boxSensor.getHumidity());
    nav.out[0].setCursor(0,2);
//    nav.out[0].printf("Temp %f",boxSensor.getTemperature());
    nav.out[0].setCursor(0,3);
//    nav.out[0].printf("Humidity %f", boxSensor.getHumidity());
  } while(u8g2.nextPage());
      break;
    case idleEnd:o.println("resuming menu.");break;
  }
  return proceed;
}

result alert(menuOut& o,idleEvent e) {
  if (e==idling) {
    o.setCursor(0,0);
    o.print("alert test");
    o.setCursor(0,2);
    o.print("press [select]");
    o.setCursor(0,3);
    o.print("to continue...");
  }
  return proceed;
}

result doAlert(eventMask e, prompt &item) {
  nav.idleOn(alert);
  return proceed;
}


void mainMenuinitializate() {
  #ifdef debug
   Serial.println(F("------------------------------------"));
   Serial.println(F(" main menu initializate "));
  #endif

  clickEncoder.setAccelerationEnabled(true);
  clickEncoder.setDoubleClickEnabled(false); // Disable doubleclicks makes the response faster.  See: https://github.com/soligen2010/encoder/issues/6

  Wire.begin();
  u8g2.begin();
  u8g2.setFont(fontName);

  u8g2.firstPage();
  do {
    u8g2.setColorIndex(1);
    nav.out[0].setCursor(0,1);
    nav.out[0].print(F("Filament Dryer 0.1"));
  } while(u8g2.nextPage());
  delay(2000);

  nav.timeOut=60;//seconds
  TimerHandle_t _encoderTimer = xTimerCreate("menuTimer", 4/portTICK_RATE_MS, pdTRUE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(encoderTimer));
  xTimerStart(_encoderTimer, 0);

//  nav.timeOut=5;//made very short for demo purposes
  nav.idleTask=MyIdleOn;//point a function to be used when menu is suspended

  #ifdef debug
   Serial.println(F(" main menu setup done."));
   Serial.println(F("------------------------------------"));Serial.flush();
  #endif

}

void encoderTimer() {
  clickEncoder.service();
}




