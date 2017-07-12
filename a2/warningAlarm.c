/*

  warningAlarm.c - This file contains
  the implementation of warningAlarm.
  (see warningAlarm.h for specification).

*/

#include <stdbool.h>
#include <stdio.h>
#include "dataStructs.h"
#include "warningAlarm.h"

void warningAlarm(void *warnStruct) {
  // Update warnStruct && LED state.
  warnData *wData = (warnData*) warnStruct;
  LED *leds[3];
  leds[0] = wData->leds[0];
  leds[1] = wData->leds[1];
  leds[2] = wData->leds[2];

  // 1. Store warning data in local variables
  bool *fuelLowPtr = wData->fuelLowPtr;
  bool *batteryLowPtr = wData->batteryLowPtr;
  unsigned short *batteryLvlPtr = wData->batteryLvlPtr;
  unsigned short *fuelLvlPtr = wData->fuelLvlPtr;

  // 2: If no alert.
  if(*batteryLvlPtr > 50 && *fuelLvlPtr > 50) {
    leds[2]->active = true;
    leds[2]->sec = 0;
    *fuelLowPtr = false;
    *batteryLowPtr = false;
  }
  // 3. If alert
  else {
    if(*batteryLvlPtr <= 50) {
      // 3.1: Battery level low
      leds[1]->active = true;
      leds[1]->sec = (*batteryLvlPtr <= 10) ? 1 : 2;
      *batteryLowPtr = (*batteryLvlPtr <= 10) ? true : false;
    }
    if(*fuelLvlPtr <= 50) {
      // 3.2: Fuel level low
      leds[0]->active = true;
      leds[0]->sec = (*fuelLvlPtr <= 10) ? 1 : 2;
      *fuelLowPtr = (*fuelLvlPtr <= 10) ? true : false;
    }
  }
}
