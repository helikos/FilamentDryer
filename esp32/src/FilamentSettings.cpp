#include "FilamentSettings.h"
#include "EEPROM.h"
#include <CRC32.h>
#include "Arduino.h"
#include "configuration.h"
#include <array>

//#define debug_trace_memory
/*
 Default 
*/
std::array<filamentSettings, COUNT_TYPE_OF_FILAMENT> listOfFilamentSettings{{
    {60, 120, 20, 0} // ABS
    ,{45, 240, 20, 0} // PLA
    ,{65, 120, 20, 0} // PETG
    ,{70, 720, 20, 0} // Nylon
    ,{50, 240, 20, 0} // TPU
    ,{45, 240, 20, 0} // PVA
    ,{50, 240, 20, 0} // Custom 1
    ,{50, 240, 20, 0} // Custom 2
    ,{50, 240, 20, 0} // Custom 3
    ,{50, 240, 20, 0} // Custom 4
    ,{30, 10, 10, 0} // test
}};

struct pidsSettings defaultPids = {default_PID_Kd, default_PID_Ki, default_PID_Kp, 0};
struct filamentSettings manualDryerSettings = {60, 120, 20, 0};
struct temperatureSettings defaultTemperature = {80, 40};
uint8_t defaultLastChosenMaterial = _abs;

bool internalReadSettings(dryerSetting &settings, uint8_t &attempt_cnt);


void printFilamentSettingsCRC(std::array<filamentSettings, COUNT_TYPE_OF_FILAMENT> list, filamentSettings manualSetting, pidsSettings pidSetting)
{
#ifdef debug
    Serial.println(F("----------------- CRC -------------------"));
    Serial.println(F("----------------- Filament -------------------"));
    Serial.printf("ABS %08X\n", list[_abs].getCrc());
    Serial.printf("PLA %08X\n", list[_pla].getCrc());
    Serial.printf("PETG %08X\n", list[_petg].getCrc());
    Serial.printf("Nylon %08X\n", list[_nylon].getCrc());
    Serial.printf("TPU %08X\n", list[_tpu].getCrc());
    Serial.printf("PVA %08X\n", list[_pva].getCrc());
    Serial.printf("Custom 1 %08X\n", list[_custom1].getCrc());
    Serial.printf("Custom 2 %08X\n", list[_custom2].getCrc());
    Serial.printf("Custom 3 %08X\n", list[_custom3].getCrc());
    Serial.printf("Custom 4 %08X\n", list[_custom4].getCrc());
    Serial.printf("test %08X\n", list[_test].getCrc());
    Serial.println(F("----------------- PID -------------------"));
    Serial.printf("pid %08X\n", pidSetting.getCrc());
    Serial.println(F("----------------- Manual Dryer -------------------"));
    Serial.printf("manual dryer %08X\n", manualSetting.getCrc());
    Serial.println(F("----------------- CRC -------------------"));
#endif
}

void printFilamentSettings(dryerSetting settings)
{
#ifdef debug
    Serial.println(F("----------------- Settings -------------------"));
    Serial.println(F("----------------- Filament -------------------"));
    for (auto item : settings.filamentParams) item.print();
    Serial.println(F("----------------- PID -------------------"));
    settings.pidParam.print();
    Serial.println(F("----------------- Manual Dryer -------------------"));
    settings.manualParam.print();
    Serial.println(F("----------------- Temperature -------------------"));
    settings.temperatureParam.print();
    Serial.println(F("----------------- Last chosen material -------------------"));
    Serial.println(settings.lastChoosenMaterial);
    Serial.println(F("----------------- Settings -------------------"));
#endif
}

void writeFilamentSettings(const std::array<filamentSettings, COUNT_TYPE_OF_FILAMENT> &list)
{
#ifdef debug
    Serial.println(F("----------------- writeFilamentSettings -------------------"));
    for (auto item : list) item.print();
#endif
    int addr = FILAMENT_SETTINGS_ADDRESS;
    for (auto item : list)
    {
        writeFilamentSetting(addr, item);
        addr += sizeof(item);
    }
#ifdef debug
    Serial.println(F("----------------- end writeFilamentSettings -------------------"));
#endif
}

void writeManualSettings(filamentSettings setting)
{
#ifdef debug
    Serial.println(F("----------------- writeManualSettings -------------------"));
    setting.print();
#endif
    writeFilamentSetting(MANUAL_SETTINGS_ADDRESS, setting);
#ifdef debug
    Serial.println(F("----------------- end writeManualSettings -------------------"));
#endif
}

void writePidSettings(pidsSettings setting)
{
#ifdef debug
    Serial.println(F("----------------- writePidSettings -------------------"));
    setting.print();
#endif
    writePidSetting(PID_SETTINGS_ADDRESS, setting);
#ifdef debug
    Serial.println(F("----------------- end writePidSettings -------------------"));
#endif
}

void writeTemperatureSettings(temperatureSettings setting)
{
#ifdef debug
    Serial.println(F("----------------- write temperature settings -------------------"));
    setting.print();
#endif
    writeTemperatureSetting(HEATER_TEMPERATURE_SETTINGS_ADDRESS, setting);
#ifdef debug
    Serial.println(F("----------------- end write temperature settings -------------------"));
#endif
}

void writeDefaultSettings()
{
#ifdef debug
    Serial.println(F("----------------- writeDefaultSettings -------------------"));
    Serial.printf("LAST_CHOSEN_MATERIAL_ADDRESS %d\n", LAST_CHOSEN_MATERIAL_ADDRESS);
    Serial.printf("FILAMENT_SETTINGS_ADDRESS %d\n", FILAMENT_SETTINGS_ADDRESS);
    Serial.printf("MANUAL_SETTINGS_ADDRESS %d\n", MANUAL_SETTINGS_ADDRESS);
    Serial.printf("HEATER_TEMPERATURE_SETTINGS_ADDRESS %d\n", HEATER_TEMPERATURE_SETTINGS_ADDRESS);
    Serial.printf("PID_SETTINGS_ADDRESS %d\n", PID_SETTINGS_ADDRESS);
#endif
    writeFilamentSettings(listOfFilamentSettings);
    writeManualSettings(manualDryerSettings);
    writePidSettings(defaultPids);
    writeTemperatureSettings(defaultTemperature);
    writeLastChosenMaterial(defaultLastChosenMaterial);
#ifdef debug
    Serial.println(F("----------------- writeDefaultSettings -------------------"));
#endif
}


void writeLastChosenMaterial(uint8_t value)
{
#ifdef debug
    Serial.println(F("----------------- writeLastChosenMaterial -------------------"));
#endif
    EEPROM_writeAnything(LAST_CHOSEN_MATERIAL_ADDRESS, value);
#ifdef debug
    Serial.println(F("----------------- writeLastChosenMaterial -------------------"));
#endif
}

void readLastChosenMaterial(uint8_t &value) {
#ifdef debug
    Serial.println(F("----------------- readLastChosenMaterial -------------------"));
#endif
    EEPROM_readAnything(LAST_CHOSEN_MATERIAL_ADDRESS, value);
#ifdef debug
    Serial.println(value);
    Serial.println(F("----------------- readLastChosenMaterial -------------------"));
#endif
}

void readFilamentSettings(const std::array<filamentSettings, COUNT_TYPE_OF_FILAMENT> &list)
{
#ifdef debug
    Serial.println(F("----------------- readFilamentSettings -------------------"));
#endif
    int addr = FILAMENT_SETTINGS_ADDRESS;
    for (uint8_t i = 0; i < list.size(); i++)
    {
        EEPROM_readAnything(addr, list[i]);
        addr += sizeof(list[i]);
    }
#ifdef debug
    Serial.println(F("----------------- filament settings -------------------"));
    for (auto item : list) item.print();
    Serial.println(F("----------------- end readFilamentSettings -------------------"));
#endif
}

void readFilamentSetting(typeOfFilament TypeOf, filamentSettings &value)
{
#ifdef debug
    Serial.printf("----------------- readFilamentSetting %d-------------------\n",TypeOf); 
#endif
    int addr = FILAMENT_SETTINGS_ADDRESS + sizeof(filamentSettings);
    EEPROM_readAnything(addr, value);
#ifdef debug
    Serial.println(F("----------------- filament settings -------------------"));
    value.print();
    Serial.println(F("----------------- end readFilamentSetting -------------------"));
#endif
}


void readManualSettings(filamentSettings &setting)
{
#ifdef debug
    Serial.println(F("----------------- readManualSettings -------------------"));
#endif
    EEPROM_readAnything(MANUAL_SETTINGS_ADDRESS, setting);
#ifdef debug
    Serial.println(F("----------------- manual settings -------------------"));
    setting.print();
    Serial.println(F("----------------- end readManualSettings -------------------"));
#endif
}

void readPidSettings(pidsSettings &setting)
{
#ifdef debug
    Serial.println(F("----------------- readPidSettings -------------------"));
#endif
    EEPROM_readAnything(PID_SETTINGS_ADDRESS, setting);
#ifdef debug
    Serial.println(F("----------------- pid settings -------------------"));
    setting.print();
    Serial.println(F("----------------- end readPidSettings -------------------"));
#endif
}

void readTemperatureSettings(temperatureSettings &settings) {
#ifdef debug
    Serial.println(F("----------------- readHeaterTemperature -------------------"));
#endif
    EEPROM_readAnything(HEATER_TEMPERATURE_SETTINGS_ADDRESS, settings);
#ifdef debug
    Serial.println(F("----------------- readHeaterTemperature settings -------------------"));
   settings.print();
    Serial.println(F("----------------- end readHeaterTemperature -------------------"));
#endif
}

bool readSettings(dryerSetting &settings) {
    uint8_t cnt = 0;
    bool res = internalReadSettings(settings, cnt);
    if (!res) {
#ifdef debug
    Serial.println(F("----------------- set default settings -------------------"));
#endif
      settings.filamentParams =  listOfFilamentSettings;
      settings.manualParam = manualDryerSettings;
      settings.pidParam = defaultPids;
      settings.temperatureParam = defaultTemperature;
      settings.lastChoosenMaterial = defaultLastChosenMaterial;
    };
    return res;
}

bool internalReadSettings(dryerSetting &settings, uint8_t &attempt_cnt) {
#ifdef debug
    Serial.println(F("----------------- readSettings -------------------"));
    Serial.printf("LAST_CHOSEN_MATERIAL_ADDRESS %d\n", LAST_CHOSEN_MATERIAL_ADDRESS);
    Serial.printf("FILAMENT_SETTINGS_ADDRESS %d\n", FILAMENT_SETTINGS_ADDRESS);
    Serial.printf("MANUAL_SETTINGS_ADDRESS %d\n", MANUAL_SETTINGS_ADDRESS);
    Serial.printf("HEATER_TEMPERATURE_SETTINGS_ADDRESS %d\n", HEATER_TEMPERATURE_SETTINGS_ADDRESS);
    Serial.printf("PID_SETTINGS_ADDRESS %d\n", PID_SETTINGS_ADDRESS);
//    printFilamentSettings(settings.filamentParams, settings.manualParam, settings.pidParam);    
//    printEeprom(0, 256); 
#endif

    attempt_cnt++;
    if (attempt_cnt <= COUNT_OF_ATTEMPT_READ_SETTINGS) {
        bool isCrcCorrect = true;
        readFilamentSettings(settings.filamentParams);
        readManualSettings(settings.manualParam);
        readPidSettings(settings.pidParam);
        readTemperatureSettings(settings.temperatureParam);
        readLastChosenMaterial(settings.lastChoosenMaterial);
        for (auto item : settings.filamentParams) {
            isCrcCorrect = isCrcCorrect && item.isCrcCorrect(); 
        }
    #ifdef debug
        Serial.printf("filament crc %d\n", isCrcCorrect);
    #endif
        isCrcCorrect = isCrcCorrect && settings.manualParam.isCrcCorrect(); 
    #ifdef debug
        Serial.printf("manual param crc %d\n", isCrcCorrect);
    #endif
        isCrcCorrect = isCrcCorrect && settings.pidParam.isCrcCorrect(); 
    #ifdef debug
        Serial.printf("pid crc %d\n", isCrcCorrect);
    #endif
        isCrcCorrect = isCrcCorrect && settings.temperatureParam.isCrcCorrect(); 
    #ifdef debug
        Serial.printf("temperature crc %d\n", isCrcCorrect);
    #endif
        if (!isCrcCorrect) {
    #ifdef debug
        Serial.println(F("----------------- crc is not correct -------------------"));
    #endif
            writeDefaultSettings();
            internalReadSettings(settings, attempt_cnt);
        }
        
    #ifdef debug
        printFilamentSettings(settings);    
        Serial.println(F("----------------- end readSettings -------------------"));
    #endif
    return true;
    } else 
       return false;
}

int writeFilamentParam(int adrr, const filamentParam &value)
{
    return EEPROM_writeAnything(adrr, value);
}

int writeFilamentSetting(typeOfFilament typeOf, const filamentSettings &value) {
#ifdef debug
    Serial.println(F("----------------- writeFilamentSetting -------------------"));
#endif
    int addr = FILAMENT_SETTINGS_ADDRESS + typeOf*sizeof(filamentSettings);
    auto item = value;
    item.crc = item.getCrc();
    return EEPROM_writeAnything(addr, item);
}


int writeFilamentSetting(int adrr, const filamentSettings &value) {
    auto item = value;
    item.crc = item.getCrc();
    return EEPROM_writeAnything(adrr, item);
}

int writeTemperatureSetting(int adrr, const temperatureSettings &value)
{
    auto item = value;
    item.crc = item.getCrc();
    return EEPROM_writeAnything(adrr, item);
}

int writePidSetting(int adrr, const pidsSettings &value)
{
    auto item = value;
    item.crc = item.getCrc();
    return EEPROM_writeAnything(adrr, item);
}

template <class T>
int EEPROM_writeAnything(int ee, const T &value)
{
#ifdef debug
#ifdef debug_trace_memory
    Serial.println(F("EEPROM_writeAnything"));
    Serial.printf("addr %d\n", ee);
    Serial.printf("sizeof(value) %d\n", sizeof(value));
#endif
#endif
    Serial.println("begin write");
    const byte *p = (const byte *)(const void *)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++)
    {
#ifdef debug
#ifdef debug_trace_memory
        Serial.printf("addr %d, value %d\n", ee, *p);
#endif
#endif
        EEPROM.writeByte(ee++, *p++);
    }
    Serial.println("begin end");
    EEPROM.commit();
    Serial.println("completed");
    return i;
}

template <class T>
int EEPROM_readAnything(int ee, T &value)
{
#ifdef debug
#ifdef debug_trace_memory
    Serial.println(F("EEPROM_readAnything"));
    Serial.printf("addr %d\n", ee);
    Serial.printf("sizeof(value) %d\n", sizeof(value));
#endif
#endif

    byte *p = (byte *)(void *)&value;
    unsigned int i;
    for (i = 0; i < sizeof(value); i++) {
#ifdef debug
#ifdef debug_trace_memory
        Serial.printf("addr %d, value %d\n", ee, EEPROM.read(ee));
#endif
#endif
        *p++ = EEPROM.read(ee++);
    }
    return i;
}

template <class T>
uint32_t crc32(const T &value)
{
    const byte *p = (const byte *)(const void *)&value;
    CRC32 crc;
    size_t numBytes = sizeof(value) - 1;
    // Here we add each byte to the checksum, caclulating the checksum as we go.
    for (size_t i = 0; i < numBytes; i++)
    {
        crc.update(*p++);
    }
    // Once we have added all of the data, generate the final CRC32 checksum.
    uint32_t checksum = crc.finalize();
    return checksum;
}

void printEeprom(int addr, int cnt) {
#ifdef debug
 Serial.println(F("----------------- print eeprom -------------------"));
 int _cnt = cnt + cnt%4;
 Serial.printf("addr %d cnt %d\n", addr, _cnt);
 byte buffer[_cnt];
 for (auto i = 0; i < _cnt;i++)
  buffer[i] = 0;
 for (auto i = 0; i < _cnt; i++) 
  buffer[i] = EEPROM.read(addr+i);
 for (auto i = 0; i < _cnt/4; i++) {
  Serial.printf("addr %d:  %d %d %d %d\n", addr+i*4, buffer[addr+i*4+0],buffer[addr+i*4+1],buffer[addr+i*4+2],buffer[addr+i*4+3]  );
 }
 Serial.println(F("----------------- print eeprom -------------------"));
#endif
}

uint32_t calcCRC32(const filamentParam &value) {
  return crc32(value);
};

uint32_t calcCRC32(const pidParam &value) {
  return crc32(value);
};

uint32_t calcCRC32(const temperatureParam &value) {
  return crc32(value);
};
