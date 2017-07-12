/*

  warningAlarm.c - This file contains
  the implementation of warningAlarm.
  (see warningAlarm.h for specification).

*/

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // for system
#include "dataStructs.h"
#include "warningAlarm.h"
#define LED0 "sys/class/leds/beaglebone:green:usr0"

FILE *led0 = NULL; // brightness
FILE *led1 = NULL;
FILE *led2 = NULL;
FILE *led3 = NULL;


void warningAlarm(void *warnStruct) {
  // 1. Store warning data in local variables
  warnData *wData = (warnData*)warnStruct;
  bool *fuelLowPtr = wData->fuelLowPtr;
  bool *batteryLowPtr = wData->batteryLowPtr;
  unsigned short *batteryLvlPtr = wData->batteryLvlPtr;
  unsigned short *fuelLvlPtr = wData->fuelLvlPtr;
  static int state1 = 0;
  static int state2 = 0;


  // 2: If no alert.
  if(*batteryLvlPtr > 50 && *fuelLvlPtr > 50) {
    *fuelLowPtr = false;
    *batteryLowPtr = false;
	system("echo 1 > /sys/class/leds/beaglebone:green:usr3/brightness");
	system("echo none > /sys/class/leds/beaglebone:green:usr2/trigger");
	state1 = 0; state2 = 0;
	
  }
  // 3. If alert
  else {
	system("echo 0 > /sys/class/leds/beaglebone:green:usr3/brightness");	  
    if(*batteryLvlPtr > 10 && *batteryLvlPtr <= 50) {
      // 3.1: Battery level low
	  *batteryLowPtr = false;
	  if (state1 == 0) {
		system("echo timer > /sys/class/leds/beaglebone:green:usr2/trigger");
		system("echo 1000 > /sys/class/leds/beaglebone:green:usr2/delay_on");
		system("echo 1000 > /sys/class/leds/beaglebone:green:usr2/delay_off");
		state1++;
	  }
	} else { // if (10 <= *batteryLvlPtr);  
      *batteryLowPtr = true;
	  if (state2 == 0) {
		// system("echo timer > /sys/class/leds/beaglebone:green:usr2/trigger");
		system("echo 100 > /sys/class/leds/beaglebone:green:usr2/delay_on");
		system("echo 100 > /sys/class/leds/beaglebone:green:usr2/delay_off");
		state2++;
		state1--;
	  }		
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
