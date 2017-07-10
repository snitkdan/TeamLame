#include <stdio.h>
#include <stdbool.h>
#include "dataStructs.h"


void warningAlarm(void *warnStruct) {
  // 0. Store warning data in local variables
  warnData *wData = (warnData *)warnStruct;
  bool *fuelLowPtr = wData->fuelLowPtr;
  bool *batteryLowPtr = wData->batteryLowPtr;
  unsigned short *batteryLvlPtr = wData->batteryLvlPtr;
  unsigned short *fuelLvlPtr = wData->fuelLvlPtr;

  // 1. Interrogate batteryLvl && fuelLvl
  switch(*batteryLvl) {
      case
  }

/*
     if (batt Lvl > 50 && fuelLvl > 50) {
         turn on LED 3
         fuelLow = false;
         battLow = false;
    } else {
         turn off LED3
        // batt
        if (10 < battLvl <= 50) {
            blink LED2 at 2 sec

        } else if ( battLvl <= 10) {
            blink LED2 at 1 sec
            battLow = true;
        } else {// batt lvl is good, but we know fuel lvl is low
            do nothing
        }

        // fuel
        if (10 < fuelLvl <= 50) {
            blink LED1 at 2 sec
        } else if ( fuelLvl <= 10) {
            blink LED1 at 1 sec
            fuelLow = true;
        } else {// fuel lvl is good, but we know batt lvl waslow
            do nothing
        }
    }*/

}
