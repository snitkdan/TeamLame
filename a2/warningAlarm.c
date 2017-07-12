/*

  warningAlarm.c - This file contains
  the implementation of warningAlarm.
  (see warningAlarm.h for specification).

*/

#include <stdbool.h>
#include <stdio.h>
#include "dataStructs.h"
#include "warningAlarm.h"
#define PATH "sys/class/leds/beaglebone:green:usr"

void warningAlarm(void *warnStruct) {
  // 1. Declare structures to store LED metadata
  /*
  static LED led1 = {"sys/class/leds/beaglebone:green:usr1/brightness", NULL, 0, false};
  static LED led2 = {"sys/class/leds/beaglebone:green:usr2/brightness", NULL, 0, false};
  static LED led3 = {"sys/class/leds/beaglebone:green:usr3/brightness", NULL, 0, false};
  static LED *leds[3] = {&led1, &led2, &led3};
  */
  //static FILE *leds[4] = {&led0, &led1, &led2, &led3};
  
  
	// 3. Update warnStruct && LED state.
	update(warnStruct);
}

void update(void *warnStruct) {
  // 1. Store warning data in local variables
  warnData *wData = (warnData*)warnStruct;
  bool *fuelLowPtr = wData->fuelLowPtr;
  bool *batteryLowPtr = wData->batteryLowPtr;
  unsigned short *batteryLvlPtr = wData->batteryLvlPtr;
  unsigned short *fuelLvlPtr = wData->fuelLvlPtr;

  // 2: If no alert.
  if(*batteryLvlPtr > 50 && *fuelLvlPtr > 50) {
    *fuelLowPtr = false;
    *batteryLowPtr = false;
  }
  // 3. If alert
  else {
    if(*batteryLvlPtr <= 50) {
      // 3.1: Battery level low
      *batteryLowPtr = (*batteryLvlPtr <= 10) ? true : false;
    }
    if(*fuelLvlPtr <= 50) {
      // 3.2: Fuel level low
      *fuelLowPtr = (*fuelLvlPtr <= 10) ? true : false;
    }
  }
}

void display(FILE *led) {

}

void deactivate(FILE *led) {
  // 0. Close the connection (turn off LED)
  // fclose(led->file);
  // 1. Reassign struct fields
  // led->file = NULL;
  // led->active = false;
}
