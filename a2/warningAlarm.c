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

// declares short hand for path of all LEDS
#define LEDPATH "/sys/class/leds/beaglebone:green:usr"

// declares on or off state, used to trigger brightness for LED3
#define ON "1"
#define OFF "0"

// adjusts how often the LED flashes (100 = On 100ms then off for 100ms)
#define SEC1 "100"
#define SEC2 "500"

// labels the LEDS
#define LED1 "1"
#define LED2 "2"
#define LED3 "3"

void warningAlarm(void *warnStruct) {
  

  printf("in warningAlarm: GLOBALCOUNTER = %lu", GLOBALCOUNTER); 	
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
  
  // 4. If battLvl and fuelLvl are okay
  if (battRegion == HIGH && fuelRegion == HIGH) {
	  // 4.1 When battLvl and fuelLvl > 50
	  LED3State(ON);
	  stopBlink(LED2);
	  stopBlink(LED1);
  }
  // 5. At least battLvl or fuelLvl <= 50
  else {
	  LED3State(OFF);
	  if (battRegion == MED && setOnlyOnce == 1) {
		    // 5.1 10 < battLvl <= 50
	        startBlink(LED2, SEC2);
			setOnlyOnce--;	
	  } else if (battRegion == LOW && setOnlyOnce == 0) {
		    // 5.2 battLvl < 10
		   	startBlink(LED2, SEC1);      	  
			setOnlyOnce++;
	  }
	  
	  if (fuelRegion == MED && setOnlyOnce2 == 1) {
		    // 5.3 10 < fuelLvl <= 50 
	        startBlink(LED1, SEC2);
			setOnlyOnce2--;	
	  } else if (fuelRegion == LOW && setOnlyOnce2 == 0) {
		    // 5.4 fuelLvl < 10	  
		   	startBlink(LED1, SEC1);      	  
			setOnlyOnce2++;
	  }	  
  }

}

void LED3State(char *changeState) {
	char command[100];
	snprintf(command, 100, "echo %s > %s%s/brightness", changeState, LEDPATH, LED3);
	system(command);
}

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
