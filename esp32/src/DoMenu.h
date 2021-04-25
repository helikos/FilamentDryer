
#ifndef _DO_MENU_H
#define _DO_MENU_H

#include <menu.h>

result menuExecute(eventMask e,navNode& nav,prompt& item);

result menuSaveExecute(eventMask e,navNode& nav,prompt& item);

result manualDryingMenuStartExecute(eventMask e,navNode& nav,prompt& item);

result autoDryingMenuStartExecute(eventMask e,navNode& nav,prompt& item);

result pidMenuAutoTuneStartExecute(eventMask e,navNode& nav,prompt& item);

result settingsMenuRestoreDefaultExecute(eventMask e,navNode& nav,prompt& item);

result showEvent(eventMask e,navNode& nav,prompt& item);

void printDisplayMessage(const String &message);


#endif