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
    int battRegion = checkRegion(batteryLvlPtr, batteryLowPtr);
    int fuelRegion = checkRegion(fuelLvlPtr, fuelLowPtr);

    if (battRegion == HIGH && fuelRegion == HIGH) {
        ledState(led3, ON);
        ledState(led2, OFF);
        ledState(led1, OFF);
    } else {
        ledState(led3, OFF);
	
		static unsigned long prevBatt = 0;
		static unsigned long prevFuel = 0;
		static int newMedBatt = 0;
		static int newLowBatt = 0;
		static int newMedFuel = 0;
		static int newLowFuel = 0;
		
		if (battRegion == MED) {
			if (newMedBatt == 1) {
					prevBatt = GLOBALCOUNTER;
					newMedBatt--;
					newLowBatt++;				
			}
			
			if ((GLOBALCOUNTER - prevBatt) % GC_TWO == 0) {
			//printf("prev: %lu, GLOBALCOUNTER %lu\n", *prev, GLOBALCOUNTER);
				
				// flip led state
				flipLED2();
				prevBatt = GLOBALCOUNTER;
			}
		} else if (battRegion == LOW) {
			if (newLowBatt == 1) {
					prevBatt = GLOBALCOUNTER;
					newLowBatt--;
					newMedBatt++;				
			}		
			// reset prev = GLOBALCOUNTER
			if ((GLOBALCOUNTER - prevBatt) % GC_ONE == 0) { 
				// flip led state
				flipLED2();
				prevBatt = GLOBALCOUNTER;
			}	
		}

		if (fuelRegion == MED) {
			if (newMedFuel == 1) {
					prevFuel = GLOBALCOUNTER;
					newMedFuel--;
					newLowFuel++;				
			}	
			if ((GLOBALCOUNTER - prevFuel) % GC_TWO == 0) {
				// flip led state
				flipLED1();
				prevFuel = GLOBALCOUNTER;
			}
		} else if(fuelRegion == LOW){
			if (newLowFuel == 1) {
					prevFuel = GLOBALCOUNTER;
					newLowFuel--;
					newMedFuel++;				
			}		
			if ((GLOBALCOUNTER - prevFuel) % GC_ONE == 0) {
				// flip led state
				flipLED1();
				prevFuel = GLOBALCOUNTER;
			}	
		}
    } 
}


void flipLED2() {
    static int flipLed2 = 0;
    flipLed2 = 1 - flipLed2;
    if (flipLed2 == 1) {
        ledState(led2, ON);
    } else {
        ledState(led2, OFF); 
    }   
}

void flipLED1() {
    static int flipLed1 = 0;
    flipLed1 = 1 - flipLed1;
    if (flipLed1 == 1) {
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

int checkRegion(unsigned short *lvlPtr, bool *lowPtr) {
    if (*lvlPtr > 50) {
	    *lowPtr = false;		
	    return HIGH;
    } else if (*lvlPtr <= 10) {
	    *lowPtr = true;				
	    return LOW;
    } else {
	    *lowPtr = false;				
	    return MED;
    }
}
