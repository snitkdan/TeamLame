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
#define LOW 10
#define LED_PATH "sys/class/leds/beaglebone:green:usr"



void warningAlarm(void *warnStruct) {
  // 1. Store warning data in local variables
  warnData *wData = (warnData*)warnStruct;
  bool *fuelLowPtr = wData->fuelLowPtr;
  bool *batteryLowPtr = wData->batteryLowPtr;
  unsigned short *batteryLvlPtr = wData->batteryLvlPtr;
  unsigned short *fuelLvlPtr = wData->fuelLvlPtr;

  // check alert
  *batteryLowPtr = checkLow(batteryLvlPtr);
  *fuelLowPtr = checkLow(fuelLvlPtr);	
}
  


bool checkLow(unsigned short *lvlPtr) {
	return (*lvlPtr <= LOW);
	
}

    /* if(*batteryLvlPtr > 10 && *batteryLvlPtr <= 50) {
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
	*/
