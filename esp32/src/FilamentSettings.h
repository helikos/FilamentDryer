
#ifndef _FILAMENTSETTINGS_H
#define _FILAMENTSETTINGS_H

#include <stdint.h>
#include "FilamentDryer.h"
#include <array>
#include "Arduino.h"
#include "configuration.h"

#define LAST_CHOSEN_MATERIAL_ADDRESS 0
#define FILAMENT_SETTINGS_ADDRESS LAST_CHOSEN_MATERIAL_ADDRESS + sizeof(uint8_t)
#define MANUAL_SETTINGS_ADDRESS FILAMENT_SETTINGS_ADDRESS + COUNT_TYPE_OF_FILAMENT * sizeof(filamentSettings)
#define HEATER_TEMPERATURE_SETTINGS_ADDRESS MANUAL_SETTINGS_ADDRESS + sizeof(filamentSettings)
#define PID_SETTINGS_ADDRESS HEATER_TEMPERATURE_SETTINGS_ADDRESS + sizeof(temperatureSettings)
#define COUNT_OF_ATTEMPT_READ_SETTINGS 3

struct filamentParam
{
    uint8_t temp;
    uint16_t minutes;
    uint8_t fanSpeed;
};

// unsigned std::

struct pidParam
{
    float Kp;
    float Ki;
    float Kd;
};

/*
 Maximum temperature
*/
struct temperatureParam
{
    float heaterTemperature;
    float boxTemperature;
};

template <class T> uint32_t crc32(const T &value);
uint32_t calcCRC32(const filamentParam &value);
uint32_t calcCRC32(const pidParam &value); 
uint32_t calcCRC32(const temperatureParam &value); 

struct filamentSettings
{
    filamentParam param;
    uint32_t crc;
    uint32_t getCrc()
    {
        return calcCRC32(param);
    };
    void print()
    {
        Serial.print(F("Temperature:"));
        Serial.println(param.temp);
        Serial.print(F("Minutes:"));
        Serial.println(param.minutes);
        Serial.print(F("Fan Speed:"));
        Serial.println(param.fanSpeed);
        Serial.printf("CRC %08X\n", crc);
        Serial.printf("calc CRC %08X\n", getCrc());
    };
    bool isCrcCorrect()
    {
        return crc == getCrc();
    }
};

struct pidsSettings
{
    pidParam param;
    uint32_t crc;
    uint32_t getCrc()
    {
        return calcCRC32(param);
    };
    void print()
    {
        Serial.printf("Kd: %f\n", param.Kd);
        Serial.printf("Ki: %f\n", param.Ki);
        Serial.printf("Kp: %f\n", param.Kp);
        Serial.printf("CRC %08X\n", crc);
        Serial.printf("calc CRC %08X\n", getCrc());
    };
    bool isCrcCorrect()
    {
        return crc == getCrc();
    }
};


struct temperatureSettings
{
    temperatureParam param;
    uint32_t crc;
    uint32_t getCrc() {
        return calcCRC32(param);
    };
    void print() {
        Serial.printf("Box Temperature: %f\n", param.boxTemperature);
        Serial.printf("Heater Temperature %f\n", param.heaterTemperature);
        Serial.printf("CRC %08X\n", crc);
        Serial.printf("calc CRC %08X\n", getCrc());
    };
    bool isCrcCorrect() {
        return crc == getCrc();
    }
};

enum typeOfFilament
{
    _abs = 0,
    _pla = 1,
    _nylon = 2,
    _petg = 3,
    _tpu = 4,
    _pva = 5,
    _custom1 = 6,
    _custom2 = 7,
    _custom3 = 8,
    _custom4 = 9,
    _test = 10
};


struct dryerSetting {
    std::array<filamentSettings, COUNT_TYPE_OF_FILAMENT> filamentParams;
    pidsSettings pidParam;
    filamentSettings manualParam;
    temperatureSettings temperatureParam;
    uint8_t  lastChoosenMaterial;
};


void writeFilamentSettings(const std::array<filamentSettings, COUNT_TYPE_OF_FILAMENT> &list);
int writeFilamentSetting(typeOfFilament typeOf, const filamentSettings &value);
void writeManualSettings(filamentSettings setting);
void writePidSettings(pidsSettings setting);
void writeTemperatureSettings(temperatureSettings setting);
void writeLastChosenMaterial(uint8_t value);
void writeDefaultSettings();

void writeInitByte();
uint8_t readInitByte();

void readFilamentSettings(const std::array<filamentSettings, COUNT_TYPE_OF_FILAMENT> &list);
void readFilamentSetting(typeOfFilament TypeOf, filamentSettings &value);
void readManualSettings(filamentSettings &setting);
void readPidSettings(pidsSettings &setting);
void readTemperatureSettings(temperatureSettings &settings);
void readLastChosenMaterial(uint8_t &value);
bool readSettings(dryerSetting &settings);

int writeFilamentParam(int adrr, const filamentParam &value);
int writeFilamentSetting(int adrr, const filamentSettings &value);
int writePidSetting(int adrr, const pidsSettings &value);
int writeTemperatureSetting(int adrr, const temperatureSettings &value);

template <class T> int EEPROM_writeAnything(int ee, const T &value);
template <class T> int EEPROM_readAnything(int ee, T &value);

void printFilamentSettingsCRC(std::array<filamentSettings, COUNT_TYPE_OF_FILAMENT> list, filamentSettings manualSetting, pidsSettings pidSetting);

void printEeprom(int addr, int cnt);

#endif
