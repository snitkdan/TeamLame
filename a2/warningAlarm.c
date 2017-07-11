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
  // 1. Declare structures to store LED metadata
  static LED led1 = {"sys/class/leds/beaglebone:green:usr1/brightness", NULL, 0, false};
  static LED led2 = {"sys/class/leds/beaglebone:green:usr2/brightness", NULL, 0, false};
  static LED led3 = {"sys/class/leds/beaglebone:green:usr3/brightness", NULL, 0, false};
  static LED *leds[3] = {&led1, &led2, &led3};

  // 3. Update warnStruct && LED state.
  update((warnData*)warnStruct, leds);

  // 4. Display/Deactive appropriate LEDs.
  int i = 0;
  while(i < 3) {
    if(leds[i]->active) {
      display(leds[i]);
    } else {
      deactivate(leds[i]);
    }
  }
}

void update(warnData *wData, LED *leds[]) {
  // 1. Store warning data in local variables
  bool *fuelLowPtr = wData->fuelLowPtr;
  bool *batteryLowPtr = wData->batteryLowPtr;
  unsigned short *batteryLvlPtr = wData->batteryLvlPtr;
  unsigned short *fuelLvlPtr = wData->fuelLvlPtr;

  // 2: If no alert.
  if(*batteryLvlPtr > 50 && *fuelLvlPtr > 50) {
    //led3.file = fopen(led3.path);
    leds[2]->active = true;
    leds[2]->sec = -1; // "infinity"
    *fuelLowPtr = false;
    *batteryLowPtr = false;
  }
  // 3. If alert
  else {
    if(*batteryLvlPtr <= 50) {
      // 3.1: Battery level low
      //led2.file = fopen(led2.path);
      leds[1]->active = true;
      leds[1]->sec = (*batteryLvlPtr <= 10) ? 1 : 2;
      *batteryLowPtr = (*batteryLvlPtr <= 10) ? true : false;
    }
    if(*fuelLvlPtr <= 50) {
      // 3.2: Fuel level low
      //led1.file = fopen(led1.path);
      leds[0]->active = true;
      leds[0]->sec = (*fuelLvlPtr <= 10) ? 1 : 2;
      *fuelLowPtr = (*fuelLvlPtr <= 10) ? true : false;
    }
  }
}

void display(LED *led) {

}

void deactivate(LED *led) {
  // 0. Close the connection (turn off LED)
  fclose(led->file);
  // 1. Reassign struct fields
  led->file = NULL;
  led->active = false;
}
