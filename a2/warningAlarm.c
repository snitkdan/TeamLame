/*

  warningAlarm.c - This file contains
  the implementation of warningAlarm.
  (see warningAlarm.h for specification).

*/

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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

#define GC_ONE 100
#define GC_TWO 200

FILE *led1 = NULL;
FILE *led2 = NULL;
FILE *led3 = NULL;

void warningAlarm(void *warnStruct) {
  
  if(!led1) {
    led1 = fopen("/sys/class/leds/beaglebone:green:usr1/brightness", "w");
  }
  if(!led2) {
    led2 = fopen("/sys/class/leds/beaglebone:green:usr2/brightness", "w");
  }
  if(!led3) {
    led3 = fopen("/sys/class/leds/beaglebone:green:usr3/brightness", "w");
  }
  checkOpened(led1);
  checkOpened(led2);
  checkOpened(led3);

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
    static int prev = 0;
    static int firstTime = 1;
    if (firstTime == 1) {
        prev = GLOBALCOUNTER;
        firstTime--;
    } 
    if (battRegion == MED) {
        if ((GLOBALCOUNTER - prev) % GC_TWO == 0) {
            // flip led state
            flipLED2();
            prev = GLOBALCOUNTER;
        }
    } else if (battRegion == LOW) {
        if ((GLOBALCOUNTER - prev) % GC_ONE == 0) 
            // flip led state
            flipLED2();
            prev = GLOBALCOUNTER;
    }

    if (fuelRegion == MED) {
        if ((GLOBALCOUNTER - prev) % GC_TWO == 0) {
            // flip led state
            flipLED1();
            prev = GLOBALCOUNTER;
        }
    } else if(fuelRegion == LOW){
        if ((GLOBALCOUNTER - prev) % GC_ONE == 0) 
            // flip led state
            flipLED1();
            prev = GLOBALCOUNTER;
    }
  }   
}

void flipLED2() {
    static int led2 = 0;
    led2 = 1 - led2;
    if (led2 == 1) {
        ledState(led2, ON);
    } else {
        ledState(led2, OFF); 
    }   
}

void flipLED1() {
    static int led1 = 0;
    led1 = 1 - led1;
    if (led1 == 1) {
        ledState(led1, ON);
    } else {
        ledState(led1, OFF); 
    }   
}

void ledState(FILE *led, int state) {
    fprintf(led, "%d", state);
    fflush(led);
}

void checkOpened(FILE *led) {
    if (!led) {
        printf("Could not open file\n");  
        exit(EXIT_FAILURE);
    }
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
