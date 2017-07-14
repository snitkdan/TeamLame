/*

  warningAlarm.h - This file contains the
  declaration & specification of warningAlarm.
  (see warningAlarm.c for implementation).

*/
#ifndef WARNING
#define WARNING
#include <stdio.h>

/*
  @param warnStruct
    Struct containing necessary
    data for the warningAlarm
  @modifies
    *warnStruct.fuelLowPtr
    && *warnStruct.batteryLowPtr
    && beaglebone LEDs 1-3
  @effects
    updates fuelLowPtr && batteryLowPtr
    based on fuelLvlPtr && batteryLvlPtr,
    respectively. There are several cases
    involving these variables that affect
    the beaglebone LEDs:

    "Alert Cases":
      1. 10 < fuelLvl <= 50: blink LED1 every 2 sec.
      2. fuelLvl <= 10: blink LED1 every 1 sec.
      3. 10 < batteryLvl <= 50: blink LED2 every 2 sec.
      4. batteryLvl <= 10: blink LED2 every 1 sec.

    "Safe Cases":
      1. batteryLvl && fuelLvl > 50: Illuminate LED3 continuously
*/
void warningAlarm(void *warnStruct);

/*
  @param lvlPtr
    Unsigned short pointer containing either
	the batterylvl or fuelLvl values
  @modifies
    nothing
  @effects
    returns either HIGH, MED, or LOW, indicating
	how much battery/fuel the lvlPtr contains
*/
int checkRegion(unsigned short *lvlPtr, bool *lowPtr);

/*
  @param region
    int displaying in what region the fuel/battery
	level is
  @modifies
    nothing
  @effects
    returns true if the region is LOW, false otherwise
*/

/*
  @param changeState
    string containing either a 1 or a 0
  @modifies
    beaglebone USER LED3
  @effects
    turns on or off the beaglebone LED3 
	depending on changeState
*/
//void LED3State(char *changeState);
void ledState(FILE *led, int state);

bool checkTimeLED2(int interval);

void flipLED2();
void flipLED1();

#endif
