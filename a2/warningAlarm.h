/*

  warningAlarm.h - This file contains the
  declaration & specification of warningAlarm.
  (see warningAlarm.c for implementation).

*/

#include <stdio.h>
#include "dataStructs.h"

// Structure for holding LED metadata
typedef struct LED_STRUCT {
  const char *path;  // system file path to LED
  FILE *file;  // file pointer (NULL if not open)
  unsigned int sec;  // blink duration (-1 if continuous)
  bool active;  // true of on, false otherwise
} LED;

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

  @param wData
    Struct containing necessary
    data for the warningAlarm
  @modifies
    *warnStruct.fuelLowPtr
    && *warnStruct.batteryLowPtr
    && leds[1-3]
  @effects
    updates fuelLowPtr && batteryLowPtr
    based on fuelLvlPtr && batteryLvlPtr,
    respectively. The appropriate
    LED structure in leds will have its
    metadata updated to reflect these updates.
    (see "warningAlarm" spec for specific cases)

*/
void update(warnData *wData, LED *leds[]);


/*
  @param led
    struct containing metadata about the
    beaglebone LED to be displayed.
  @modifies
    beaglebone LED specified in led
  @effects
    blinks beaglebone LED specified in
    led every led.sec seconds.
*/
void display(LED *led);

/*
  @param led
    struct containing metadata about the
    beaglebone LED to be displayed.
  @modifies
    beaglebone LED specified in led
  @effects
    turns off the beaglebone LED
    specified in led, and updates
    the relevant fields in led
    to indicate deactivation.
*/
void deactivate(LED *led);
