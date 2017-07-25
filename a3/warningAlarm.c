/*

  warningAlarm.c - This file contains
  the implementation of warningAlarm.
  (see warningAlarm.h for specification).

*/

#include <stdbool.h>
#include <stdio.h>
#include <errno.h>
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
#define FLIP 1

// declares how many ticks for the GLOBALCOUNTER before 1 or 2 secs have passed
#define GC_ONE 100
#define GC_TWO 200

// globally defines the led files here
FILE *led1 = NULL;
FILE *led2 = NULL;
FILE *led3 = NULL;

void warningAlarm(void *warnStruct) {
  #ifdef BEAGLEBONE
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
  #endif
  // 1.2 Store warning data in local variables
  warnData *wData = (warnData*)warnStruct;
  bool *fuelLowPtr = wData->fuelLowPtr;
  bool *batteryLowPtr = wData->batteryLowPtr;
  unsigned short *batteryLvlPtr = wData->batteryLvlPtr;
  unsigned short *fuelLvlPtr = wData->fuelLvlPtr;

  // 2. Determine in what region the battery/fuel level is (high, med, low)
  int battRegion = checkRegion(batteryLvlPtr, batteryLowPtr);
  int fuelRegion = checkRegion(fuelLvlPtr, fuelLowPtr);

  #ifdef BEAGLEBONE
  // 3. Section for controlling the LEDS
  if (battRegion == HIGH && fuelRegion == HIGH) {
	    // 3.1 both battery and fuel level are high
      ledState(led3, ON);
      ledState(led2, OFF);
      ledState(led1, OFF);
  } else {
    // 3.2 at least batt or fuel is not HIGH
    ledState(led3, OFF);
    // 3.2.1 Declare statics to track the states for
    // LED2 and its relationship with Battery
		static unsigned long prevBatt = 0;
		static int newMedBatt = 1;
		static int newLowBatt = 0;

  	if (battRegion == MED) {
  		// 3.2.2 Battery is in MEDIUM region
  		if (newMedBatt == 1) {
  			// Only do this step if battery just changed state to MEDIUM
  			prevBatt = GLOBALCOUNTER;
  			newMedBatt--; // decrements to show we've entered MED state
  			newLowBatt++; // increments to show we're not in LOW state
  		}
  		if ((GLOBALCOUNTER - prevBatt) % GC_TWO == 0) {
  			// flip led2 state every 2 second
  			flipLED2(FLIP);
  			prevBatt = GLOBALCOUNTER;
  		}
  	} else if (battRegion == LOW) {
  		// 3.2.3 Battery is in LOW region
  		if (newLowBatt == 1) {
  			// Only do this step if battery just changed
  			// state to LOW
  			prevBatt = GLOBALCOUNTER;
  			newLowBatt--; // decrements to show we've entered LOW state
  			newMedBatt++; // increments to show we're not in MED state
  	  }
  		if ((GLOBALCOUNTER - prevBatt) % GC_ONE == 0) {
  			// flip led2 state every 1 second
  			flipLED2(FLIP);
  			prevBatt = GLOBALCOUNTER;
  	  }
    } else {
         // 3.2.4 Battery is in HIGH region, LED should turn off
         // If in this state, FUEL must be MED or LOW
         flipLED2(OFF);
    }

  	// 3.3.1 Declare statics to track the states
  	// for LED1 and its relationship with Fuel
  	static unsigned long prevFuel = 0;
  	static int newMedFuel = 1;
  	static int newLowFuel = 0;

  	// NOTE: the logic here is the same as the battery's
  	if (fuelRegion == MED) {
  		if (newMedFuel == 1) {
  				prevFuel = GLOBALCOUNTER;
  				newMedFuel--;
  				newLowFuel++;
  		}
  		if ((GLOBALCOUNTER - prevFuel) % GC_TWO == 0) {
  			flipLED1(FLIP);
  			prevFuel = GLOBALCOUNTER;
  		}
  	} else if(fuelRegion == LOW) {
  		if (newLowFuel == 1) {
  				prevFuel = GLOBALCOUNTER;
  				newLowFuel--;
  				newMedFuel++;
  		}
  		if ((GLOBALCOUNTER - prevFuel) % GC_ONE == 0) {
  			// flip led state
  			flipLED1(FLIP);
  			prevFuel = GLOBALCOUNTER;
  		}
  	} else {
  		flipLED1(OFF);
  	}
  }
  #endif
}

// Tracks the state of LED2, whether it's
// on or off.
// @param force
//	  If force is off, then turn LED2 off
//    else just flip the led state
void flipLED2(int force) {
  static int flipLed2 = 0;
  flipLed2 = (force == OFF) ? 0 : (1 - flipLed2);
	if (flipLed2 == 1) {
		ledState(led2, ON);
	} else {
		ledState(led2, OFF);
	}
}

// Tracks the state of LED1, whether it's
// on or off.
// @param force
//	  If force is off, then turn LED1 off
//    else just flip the led state
void flipLED1(int force) {
  static int flipLed1 = 0;
  flipLed1 = (force == OFF) ? 0 : (1 - flipLed1);
  if (flipLed1 == 1) {
      ledState(led1, ON);
  } else {
      ledState(led1, OFF);
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
        perror("");
        exit(EXIT_FAILURE);
    }
}

int checkRegion(unsigned short *lvlPtr, bool *lowPtr) {
    *lowPtr = (*lvlPtr <= 10) ? true : false;
    return (*lvlPtr <= 50) ? ((*lvlPtr <= 10) ? LOW : MED) : HIGH;
}
