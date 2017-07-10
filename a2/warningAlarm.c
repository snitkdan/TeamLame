#include <stdio.h>
#include <stdbool.h>
#include <stdio.h>
#include "dataStructs.h"

// Structure for holding LED metadata
typedef struct LED_STRUCT {
  char *path;  // system file path to LED
  FILE *file;  // file pointer (NULL if not open)
  unsigned int sec;  // blink duration (-1 if continuous)
  bool active;  // true of on, false otherwise
} LED;

void warningAlarm(void *warnStruct) {
  // 0. Store warning data in local variables
  warnData *wData = (warnData *)warnStruct;
  bool *fuelLowPtr = wData->fuelLowPtr;
  bool *batteryLowPtr = wData->batteryLowPtr;
  unsigned short *batteryLvlPtr = wData->batteryLvlPtr;
  unsigned short *fuelLvlPtr = wData->fuelLvlPtr;

  // 1. Declare file paths for LEDs
  static char *l1 = "sys/class/leds/beaglebone:green:usr1/brightness";
  static char *l2 = "sys/class/leds/beaglebone:green:usr2/brightness";
  static char *l3 = "sys/class/leds/beaglebone:green:usr3/brightness";

  // 2. Declare structures to store LED metadata
  static LED led1 = {l1, NULL, 0, FALSE};
  static LED led2 = {l2, NULL, 0, FALSE};
  static LED led3 = {l3, NULL, 0, FALSE};
  static LED *leds[3] = {&led1, &led2, &led3};

  // 3. Interrogate batteryLvl && fuelLvl
  if(*batteryLvlPtr > 50 && *fuelLvlPtr > 50) {
    // 3.1: No alert.
    //led3.file = fopen(led3.path);
    led3.sec = -1; // "infinity"
    led3.active = true;
    *fuelLowPtr = false;
    *batteryLowPtr = false;
  } else {
    // 3.2: Alert
    if(*batteryLvl <= 50) {
      // 3.2.1: Battery level low
      //led2.file = fopen(led2.path);
      led2.active = true;
      led2.sec = (*batteryLvl <= 10) ? 1 : 2;
      *batteryLowPtr = (*batteryLvl <= 10) ? true : false;
    }
    if(*fuelLvl <= 50) {
      // 3.2.2: Fuel level low
      //led1.file = fopen(led1.path);
      led1.active = true;
      led1.sec = (*fuelLvlPtr <= 10) ? 1 : 2;
      *fuelLowPtr = (*fuelLvlPtr <= 10) ? true : false;
    }
  }

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

void display(LED *led) {

}

void deactivate(LED *led) {
  // 0. Close the connection (turn off LED)
  fclose(led->file);
  // 1. Reassign struct fields
  led->file = NULL;
  led->active = false;
}
