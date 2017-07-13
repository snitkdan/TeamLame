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
#define ON "1"
#define OFF "0"

// labels the LEDs
#define LED1 "1"
#define LED2 "2"
#define LED3 "3"

// declares how many times GLOBALCOUNTER would increment to be considered
// one second (or two seconds)
#define SEC_1 1
#define SEC_2 2

void warningAlarm(void *warnStruct) {
  
  // 1. Store warning data in local variables
  warnData *wData = (warnData*)warnStruct;
  bool *fuelLowPtr = wData->fuelLowPtr;
  bool *batteryLowPtr = wData->batteryLowPtr;
  unsigned short *batteryLvlPtr = wData->batteryLvlPtr;
  unsigned short *fuelLvlPtr = wData->fuelLvlPtr;
  
  // 1.1 Sets static ints to prevent startBlink functions from 
  //     being set repeatedly for LED2 and LED1
  static int setOnlyOnce = 1;
  static int setOnlyOnce2 = 1;

  // 2. Determine in what region the battery/fuel level is (high, med, low)
  int battRegion = checkRegion(batteryLvlPtr);
  int fuelRegion = checkRegion(fuelLvlPtr);
  
  // 3. Set batteryLow and fuelLow depending on region
  *batteryLowPtr = checkLow(battRegion);
  *fuelLowPtr = checkLow(fuelRegion);
  
  
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

/*
void LEDState(char *led, char *changeState) {
	char command[100];
	snprintf(command, 100, "echo %s > %s%s/brightness", changeState, LEDPATH, led);
	system(command);
}

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
