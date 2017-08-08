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
#include "nonBlockingKeys.h"


// declares the regions for batteryLvl and fuelLvl
#define HIGH 2
#define MED 1
#define LOW 0

// declares short hand for the shared path of all LEDs
#define LEDPATH "/sys/class/leds/beaglebone:green:usr"

// declares on or off state, used to trigger brightness for LEDs
#define ON 1
#define OFF 0
#define FLIP 2

// declares how many ticks for the GLOBALCOUNTER before 1 or 2 secs have passed
#define GC_ONE 10
#define GC_TWO 20
#define GC_FIVE 50
#define GC_TEN 100
#define GC_FIFTEEN 150
#define GC_10HZ 2
#define FULL 36

#define BEAGLEBONE
// globally defines the led files here
FILE *led1 = NULL;
FILE *led2 = NULL;
FILE *led3 = NULL;

static int tempFlag = 0; // 0: normal, 1: battery was over temperature waiting for acknowledge print onto terminal, 2: no acknowledge after 15 sec, flash lights 
static int timer_batt = 0;
static int timer_fuel = 0;
static int timer_15 = 0;
static int timer_10 = 0;

int stateled2 = 0;
int stateled1 = 0;

bool warningBattTemp = false;

void warningAlarm(void *warnStruct) {
    // 1.1 Opens the led files and checks they were opened successfully
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

    // 1.2 Store warning data in local variables
    warnData *wData = (warnData*)warnStruct;
    bool *fuelLowPtr = wData->fuelLowPtr;
    bool *batteryLowPtr = wData->batteryLowPtr;
    unsigned int *batteryLvl = wData->batteryLvlPtr;
    unsigned int *fuelLvl = (unsigned int*) wData->fuelLvlPtr;
    bool *batteryOverTemp = wData->batteryOverTempPtr;

	
    // 2. Determine in what region the battery/fuel level is (high, med, low)
    int battRegion = checkRegion(batteryLvl, batteryLowPtr);
    int fuelRegion = checkRegion(fuelLvl, fuelLowPtr);
        #ifdef BEAGLEBONE	
	// 3. Section for controlling the LEDS
	if (*batteryOverTemp && tempFlag == 0) { // if tempFlag == 2, don't change it to 1
		tempFlag = 1;
		//tempFlag = 0;
	}
	
	if (tempFlag != 2) {
		if (tempFlag == 1) {
			warningBattTemp = true;
			if (timer_15 == GC_FIFTEEN) {
				tempFlag = 2;
				timer_15 = 0;
			} else {
				timer_15++;	
			}
			readAck();			
		}
	
		if (battRegion == HIGH && fuelRegion == HIGH) {
			// 3.1 both battery and fuel level are high
			ledState(led3, ON);
			ledState(led2, OFF);
			ledState(led1, OFF);
		} else {
			ledState(led3, OFF);


			// 3.2.1 Declare statics to track the states
			//       for LED2 and its relationship with Battery
			if (battRegion == MED) {
				// 3.2.2 Battery is in MEDIUM region
				if (timer_batt % GC_TWO == 0) {
					// flip led2 state every 2 second
					flipLED(FLIP, led2, &stateled2);
					timer_batt = 0;
				}
			} else if (battRegion == LOW) {
				// 3.2.3 Battery is in LOW region
				if (timer_batt % GC_ONE == 0) {
					// flip led2 state every 1 second
					flipLED(FLIP, led2, &stateled2);
					timer_batt = 0;					
				}
			    timer_batt++;
			} else {
				// 3.2.4 Battery is in HIGH region, LED should turn off
				//       If in this state, FUEL must be MED or LOW
				flipLED(OFF, led2, &stateled2);
			}
		    timer_batt++;
			

			// NOTE: the logic here is the same as the battery's
			if (fuelRegion == MED) {
				if (timer_fuel % GC_TWO == 0) {
					flipLED(FLIP, led1, &stateled1);
					timer_fuel = 0;
				}
			} else if(fuelRegion == LOW){
				if (timer_fuel % GC_ONE == 0) {
					// flip led state
					flipLED(FLIP, led1, &stateled1);
					timer_fuel = 0;					
				}
			} else {
				flipLED(OFF, led1, &stateled1);
			}
            timer_fuel++;
		}
	} else {
		readAck();
		ledState(led3, OFF);
		
		if (timer_10 < GC_FIVE) {
			if (timer_10 % GC_10HZ == 0) {
			    flipLED(FLIP, led2, &stateled2);
			    flipLED(FLIP, led1, &stateled1);
			}
			timer_10++;
		} else if (timer_10 < GC_TEN) {
			flipLED(ON, led2, &stateled2);
			flipLED(ON, led1, &stateled1);
			timer_10++;			
	    } else {
			timer_10 = 0;
		}
	}	
        #endif
}


void flipLED(int force, FILE *ledFile, int *state) {
	if (force == OFF) {
		*state = 0;
	} else if (force == ON){
		*state = 1;
	} else {	
		*state = 1 - *state;
	}
	if (*state == 1) {
		ledState(ledFile, ON);
	} else {
		ledState(ledFile, OFF);
	}
}

void readAck() {
	char c = getchar();
	//char c = 'a';	
	if (!warningCmd(c)) {
		if (consoleModeCmd(c) || motorSpeedCmd(c) || satVehicleCmd(c)) ungetc(c, stdin);
	} else {
		//printf("Warning Alarm: Acknowledge Received\n");
		warningBattTemp = false;		
		tempFlag = 0;
		timer_15 = 0;
	}	
}

// Prints the state to the led and flushes the buffer
// @param led
//		the FILE containing the led
// @param state
//		instructions on whether to turn on or off led
void ledState(FILE *led, int state) {
    fprintf(led, "%d", state);
    fflush(led);
}

// checks if file was opened successfully
// NOTE: function prototype inside dataStructs.h
void checkOpened(FILE *led) {
    if (!led) {
        //printf("Could not open file\n");
        //exit(EXIT_FAILURE);
        return;
    }
}

int checkRegion(unsigned int *lvlPtr, bool *lowPtr) {
    if (*lvlPtr > FULL / 2) {
	    *lowPtr = false;
	    return HIGH;
    } else if (*lvlPtr <= 4) { // 3.6 is 10 percent of full, but rounded up to 4 due to error 
	    *lowPtr = true;
	    return LOW;
    } else {
	    *lowPtr = false;
	    return MED;
    }
}
