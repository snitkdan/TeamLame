/*

  warningAlarm.c - This file contains
  the implementation of warningAlarm.
  (see warningAlarm.h for specification).

*/

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // for system()
#include "dataStructs.h"
#include "warningAlarm.h"

// declares the regions for batteryLvl and fuelLvl
#define HIGH 2
#define MED 1
#define LOW 0

// declares short hand for the shared path of all LEDs
#define LEDPATH "/sys/class/leds/beaglebone:green:usr"

// declares on or off state, used to trigger brightness for LEDs
#define ON 1
#define OFF 0

FILE *led1 = NULL;
FILE *led2 = NULL;
FILE *led3 = NULL;

void warningAlarm(void *warnStruct) {
  static unsigned long start = 0;
  
  if(!led1) {
    led1 = fopen("/sys/class/leds/beaglebone:green:usr1/brightness", "w");
  }
  if(!led2) {
    led2 = fopen("/sys/class/leds/beaglebone:green:usr2/brightness", "w");
  }
  if(!led3) {
    led2 = fopen("/sys/class/leds/beaglebone:green:usr3/brightness", "w");
  }

  // 1. Store warning data in local variables
  warnData *wData = (warnData*)warnStruct;
  bool *fuelLowPtr = wData->fuelLowPtr;
  bool *batteryLowPtr = wData->batteryLowPtr;
  unsigned short *batteryLvlPtr = wData->batteryLvlPtr;
  unsigned short *fuelLvlPtr = wData->fuelLvlPtr;
  
  // 2. Determine in what region the battery/fuel level is (high, med, low)
  int battRegion = checkRegion(batteryLvlPtr);
  int fuelRegion = checkRegion(fuelLvlPtr);
  
  // 3. Set batteryLow and fuelLow depending on region
  *batteryLowPtr = checkLow(battRegion);
  *fuelLowPtr = checkLow(fuelRegion);

  if (battRegion == HIGH && fuelRegion == HIGH) {
    ledState(led3, ON);
    ledState(led2, OFF);
    ledState(led1, OFF);
  } else {
    ledState(led3, OFF);
  
  }   
  
  
  /*
  // 4. If battLvl and fuelLvl are okay
  if (battRegion == HIGH && fuelRegion == HIGH) {
	  // 4.1 When battLvl and fuelLvl > 50
	  LEDState(LED3, ON);
	  LEDState(LED2, OFF);
	  LEDState(LED1, OFF);
  }
  // 5. At least battLvl or fuelLvl <= 50
  else {
	  LEDState(LED3, OFF);
	  bool timesUp = false;
	  if (battRegion == MED) {
		  if ((timesUp = checkTimeLED2(SEC_2)))
			  stateLED2 = 1 - stateLED2;
	  }	else {
		  if ((timesUp = checkTimeLED2(SEC_1)))
	  		  stateLED2 = 1 - stateLED2;
	  }
	  printf("\nWARNING: %d GLOBAL COUNTER: %lu\n", timesUp, GLOBALCOUNTER);
  }
  if (stateLED2 == 1) {
	  LEDState(LED2, ON);
  } else {
	  LEDState(LED2, OFF);
  }
  */
}

void ledState(FILE *led, int state) {
    fprintf(led, "%d", state);
    fflush(led);
}
/*
bool checkTimeLED2(int interval) {
	static unsigned long startInterval = 0;
	printf("\nstartInterval: %lu\n switch at %lu", startInterval, startInterval + interval);
	
	static int firstTimeCalled = 1;
	if (firstTimeCalled == 1) {
		startInterval = GLOBALCOUNTER; // need to account for waste of one cycle
		firstTimeCalled--;
	}
	if (startInterval + interval == GLOBALCOUNTER) {
		startInterval = GLOBALCOUNTER; // need to account for next cycle
		return true;
	} 
	return false;
}
*/

/*
void stopBlink(char *led) {
	char command[100];
	snprintf(command, 100, "echo none > %s%s/trigger", LEDPATH, led);
	system(command);
}

void startBlink(char *led, char *delay) {
    char command[100];
	snprintf(command, 100, "echo timer > %s%s/trigger", LEDPATH, led);
	system(command);
	snprintf(command, 100, "echo %s > %s%s/delay_on",  delay, LEDPATH, led);
	system(command);	
	snprintf(command, 100, "echo %s > %s%s/delay_off", delay, LEDPATH, led);
	system(command);	
}
*/

int checkRegion(unsigned short *lvlPtr) {
	if (*lvlPtr > 50) {
		return HIGH;
	} else if (*lvlPtr <= 10) {
		return LOW;
	} else {
		return MED;
	}
}

bool checkLow(int region) {
	return region == LOW;
}
