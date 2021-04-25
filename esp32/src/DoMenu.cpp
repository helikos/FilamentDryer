#include <Arduino.h>

#include "DoMenu.h"
#include "configuration.h"
#include "FilamentSettings.h"
#include <menuIO/u8g2Out.h>

extern dryerSetting dryerParameters;
extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;
extern navRoot nav;

#define pidsValueSave "Settings.PID.Pids Value.Save.Yes"
#define temperatureValueSave "Settings.Temperature.Save.Yes"
#define manualValueSave "Manual Drying.Save.Yes"
#define presetSave "Settings.Preset.Save.Yes"

#define PRESET_MENU_SUFFIX  ".Save.Yes"
#define PRESET_MENU_PREFIX  "Settings.Preset."

#define filamentABSValueSave PRESET_MENU_PREFIX  FILAMENT_ABS  PRESET_MENU_SUFFIX
#define filamentPLAValueSave PRESET_MENU_PREFIX  FILAMENT_PLA  PRESET_MENU_SUFFIX
#define filamentNylonValueSave PRESET_MENU_PREFIX  FILAMENT_NYLON  PRESET_MENU_SUFFIX
#define filamentPETGValueSave PRESET_MENU_PREFIX  FILAMENT_PETG  PRESET_MENU_SUFFIX
#define filamentTPUValueSave PRESET_MENU_PREFIX  FILAMENT_TPU  PRESET_MENU_SUFFIX
#define filamentPVAValueSave PRESET_MENU_PREFIX  FILAMENT_PVA  PRESET_MENU_SUFFIX
#define filamentCustom1ValueSave PRESET_MENU_PREFIX  FILAMENT_CUSTOM_1  PRESET_MENU_SUFFIX
#define filamentCustom2ValueSave PRESET_MENU_PREFIX  FILAMENT_CUSTOM_2  PRESET_MENU_SUFFIX
#define filamentCustom3ValueSave PRESET_MENU_PREFIX  FILAMENT_CUSTOM_3  PRESET_MENU_SUFFIX
#define filamentCustom4ValueSave PRESET_MENU_PREFIX  FILAMENT_CUSTOM_4  PRESET_MENU_SUFFIX
#define filamentTestValueSave PRESET_MENU_PREFIX  FILAMENT_TEST  PRESET_MENU_SUFFIX

void showPath(navRoot& root) {
  Serial.println( root.selected().getText());
  Serial.print("nav level:");
  Serial.print(root.level);
  Serial.print(" path:[");
  for(int n=0;n<=root.level;n++) {
    Serial.print(n?",":"");
    Serial.print(root.path[n].selected().getText());
  }
  Serial.println("]");
}

constexpr 
unsigned int hash(const char* str, int h = 0)
{
    return !str[h] ? 5381 : (hash(str, h+1)*33) ^ str[h];
}

result showEvent(eventMask e,navNode& nav,prompt& item) {
  Serial.println();
  Serial.println("========");
  Serial.print("Event for target: 0x");
  Serial.println((long)nav.target,HEX);
  showPath(*nav.root);
  Serial.print(e);
  switch(e) {
    case noEvent://just ignore all stuff
      Serial.println(" noEvent");break;
    case activateEvent://this item is about to be active (system event)
      Serial.println(" activateEvent");break;
    case enterEvent://entering navigation level (this menu is now active)
      Serial.println(" enterEvent");break;
    case exitEvent://leaving navigation level
      Serial.println(" exitEvent");break;
    case returnEvent://TODO:entering previous level (return)
      Serial.println(" returnEvent");break;
    case focusEvent://element just gained focus
      Serial.println(" focusEvent");break;
    case blurEvent://element about to lose focus
      Serial.println(" blurEvent");break;
    case selFocusEvent://TODO:child just gained focus
      Serial.println(" selFocusEvent");break;
    case selBlurEvent://TODO:child about to lose focus
      Serial.println(" selBlurEvent");break;
    case updateEvent://Field value has been updated
      Serial.println(" updateEvent");break;
    case anyEvent:
      Serial.println(" anyEvent");break;
  }
  return proceed;
}

result menuExecute(eventMask e,navNode& nav,prompt& item) {
  return showEvent(e, nav, item);
}

std::string getPath(navRoot& root) {
  std::string ret = "";
  for(int n=0;n<=root.level;n++) {
    ret += n?".":"";
    ret += root.path[n].selected().getText();
  }
  return ret;
}

bool filamentSave(typeOfFilament type, filamentSettings value) {
    bool result = true;
    filamentSettings set;
    writeFilamentSetting(type, value);
    readFilamentSetting(type, set);
    result = set.isCrcCorrect();
    return result;               
}

result menuSaveExecute(eventMask e,navNode& nav,prompt& item) {
#ifdef debug
    Serial.println(F("----------------- menuSaveExecute -------------------"));
#endif
  if (e == enterEvent) {
      std::string path = getPath(*nav.root);
#ifdef debug  
    Serial.println(path.c_str());
#endif   
    bool result = true; 
    switch (hash(path.c_str())) {
        case hash(pidsValueSave) : 
#ifdef debug  
    Serial.println(F("Save Pids"));
#endif   
            pidsSettings setting;
            writePidSettings(dryerParameters.pidParam);
            readPidSettings(setting);
            result = setting.isCrcCorrect();
            break;
        case hash(temperatureValueSave) : 
#ifdef debug  
    Serial.println(F("Save Temperature"));
#endif   
            temperatureSettings val1;
            writeTemperatureSettings(dryerParameters.temperatureParam);
            readTemperatureSettings(val1);
            result = val1.isCrcCorrect();
            break;
        case hash(manualValueSave) : 
#ifdef debug  
    Serial.println(F("Save manual Parameters"));
#endif   
            filamentSettings val2;
            writeManualSettings(dryerParameters.manualParam);
            readManualSettings(val2);
            result = val2.isCrcCorrect();
            break;
        case hash(presetSave) : 
#ifdef debug  
    Serial.println(F("Save Filament Parameters"));
#endif   
            std::array<filamentSettings, COUNT_TYPE_OF_FILAMENT> val3;
            writeFilamentSettings(dryerParameters.filamentParams);
            readFilamentSettings(val3);
            for (auto item : val3) result = result && item.isCrcCorrect(); 
            break;
        case hash(filamentABSValueSave) :   
            result = filamentSave(_abs, dryerParameters.filamentParams[_abs]);
            break;
        case hash(filamentPLAValueSave) :   
            result = filamentSave(_pla, dryerParameters.filamentParams[_pla]);
            break;
        case hash(filamentNylonValueSave) :   
            result = filamentSave(_nylon, dryerParameters.filamentParams[_nylon]);
            break;
        case hash(filamentPETGValueSave) :   
            result = filamentSave(_petg, dryerParameters.filamentParams[_petg]);
            break;
        case hash(filamentTPUValueSave) :   
            result = filamentSave(_tpu, dryerParameters.filamentParams[_tpu]);
            break;
        case hash(filamentPVAValueSave) :   
            result = filamentSave(_pva, dryerParameters.filamentParams[_pva]);
            break;
        case hash(filamentCustom1ValueSave) :   
            result = filamentSave(_custom1, dryerParameters.filamentParams[_custom1]);
            break;
        case hash(filamentCustom2ValueSave) :   
            result = filamentSave(_custom2, dryerParameters.filamentParams[_custom2]);
            break;
        case hash(filamentCustom3ValueSave) :   
            result = filamentSave(_custom3, dryerParameters.filamentParams[_custom3]);
            break;
        case hash(filamentCustom4ValueSave) :   
            result = filamentSave(_custom4, dryerParameters.filamentParams[_custom4]);
            break;
        case hash(filamentTestValueSave) :   
            result = filamentSave(_test, dryerParameters.filamentParams[_test]);
            break;
        default: Serial.println("nothing");   
    }
    if (!result) {
      printDisplayMessage(F("failed"));  
    } else {
      printDisplayMessage(F("successfully"));  
    }  
  }
#ifdef debug
    Serial.println(F("----------------- menuSaveExecute -------------------"));
#endif
  return quit;
}

void printDisplayMessage(const String &message) {
  u8g2.firstPage();
  do {
    u8g2.setColorIndex(1);
    nav.out[0].setCursor(0,2);
    nav.out[0].print(message);
  } while(u8g2.nextPage());
    char c;
    do {
    c = nav.in.read();
    } while (options->getCmdChar(enterCmd) != c && options->getCmdChar(escCmd)!=c);
    nav.idleOff();
}

result printEvent(eventMask e) {
  Serial.print(e);
  switch(e) {
    case noEvent://just ignore all stuff
      Serial.println(" noEvent");break;
    case activateEvent://this item is about to be active (system event)
      Serial.println(" activateEvent");break;
    case enterEvent://entering navigation level (this menu is now active)
      Serial.println(" enterEvent");break;
    case exitEvent://leaving navigation level
      Serial.println(" exitEvent");break;
    case returnEvent://TODO:entering previous level (return)
      Serial.println(" returnEvent");break;
    case focusEvent://element just gained focus
      Serial.println(" focusEvent");break;
    case blurEvent://element about to lose focus
      Serial.println(" blurEvent");break;
    case selFocusEvent://TODO:child just gained focus
      Serial.println(" selFocusEvent");break;
    case selBlurEvent://TODO:child about to lose focus
      Serial.println(" selBlurEvent");break;
    case updateEvent://Field value has been updated
      Serial.println(" updateEvent");break;
    case anyEvent:
      Serial.println(" anyEvent");break;
  }
  return proceed;
}



result manualDryingMenuStartExecute(eventMask e,navNode& nav,prompt& item){
#ifdef debug
    Serial.println(F("----------------- manualDryingMenuStartExecute -------------------"));
    printEvent(e);
#endif
  return proceed;
}

result autoDryingMenuStartExecute(eventMask e,navNode& nav,prompt& item) {
#ifdef debug
    Serial.println(F("----------------- autoDryingMenuStartExecute -------------------"));
    printEvent(e);
#endif
  return proceed;
}

result pidMenuAutoTuneStartExecute(eventMask e,navNode& nav,prompt& item) {
#ifdef debug
    Serial.println(F("----------------- pidMenuAutoTuneStartExecute -------------------"));
    printEvent(e);
#endif
  return proceed;
}


result settingsMenuRestoreDefaultExecute(eventMask e,navNode& nav,prompt& item) {
#ifdef debug
    Serial.println(F("----------------- settingsMenuRestoreDefaultExecute -------------------"));
    printEvent(e);
#endif
    if (e == enterEvent) {
        writeDefaultSettings();
        bool result = readSettings(dryerParameters);
    if (!result) printDisplayMessage(F("failed"));  
     else printDisplayMessage(F("successfully"));  
    }
    return quit;
}


