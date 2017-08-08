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
    Unsigned int pointer containing either
	the batterylvl or fuelLvl values
  @param lowPtr
    Unsigned bool pointer containing either batteryLow
    or fuelLow values	
  @modifies
    assigns batteryLow or fuelLow true if region is LOW
  @effects
    returns either HIGH, MED, or LOW, indicating
	how much battery/fuel the lvlPtr contains
*/
// CASES:
// 		If above 50: 				 HIGH
// 		If in between 10 to 50: 	 MED
// 		If less than or equal to 10: LOW
int checkRegion(unsigned int *lvlPtr, bool *lowPtr);


void ledState(FILE *led, int state);
void readAck();
void flipLED(int force, FILE *ledFile, int *state);


#endif
