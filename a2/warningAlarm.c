#include <stdio.h>
#include <stdbool.h>
#include "dataStructs.h"


// Refer to p.15
void warningAlarm(void *warnStruct)
{
  //  turn LED3 on;
/*
    PECKOLS ORIGINAL SPEC
    // all good case
    if (batt lvl > 50 && fuel lvl > 50) {
        print and hold LED3
    } else if (batt lvl <= 50 && fuel lvl > 50) {
          if (10 < batt lvl <= 50) {
              blink LED2 every 1 sec
          } else { //(batt lvl <= 10) {
              blink LED1 every 1 sec
              set battlow = true;
          }
    } else if (batt lvl > 50 && fuel lvl <= 50) {
         if (10 < fuel lvl <= 50) {
              blink LED2 every 2 sec
         } else { //fuel lvl <= 10
              blink LED1 every 2 sec
              set fuellow = true;
         }
    } else  { //if (batt lvl <= 50 && fuel lv <= 50) {
         if (10 < fuel lvl <= 50 && 10 < batt lvl <= 50) {
              blink LED2 every 1 sec
         } else if (10 < fuel lvl <= 50 && batt lvl <= 10) {
              blink LED2 every 2 sec
              blink LED1 every 1 sec
              set battlow = true;

         } else if ( 10 < batt lvl <= 50 && fuel lvl <= 10 )
              blink LED2 every 1 sec
              blink LED1 every 2 sec
              set fuelLow = true;
         } else { // both are less than 10
              blink LED1 every 1 sec
              set battlow = true;
              set fuelLow = true;
         }
    }
*/
//   DOUGLAS' EDITED SPEC (PENDING)
/*
     if (batt Lvl > 50 && fuelLvl > 50) {
         turn on LED 3
         fuelLow = false;
         battLow = false;
    } else {
         turn off LED3
        // batt
        if (10 < battLvl <= 50) {
            blink LED2 at 2 sec

        } else if ( battLvl <= 10) {
            blink LED2 at 1 sec
            battLow = true;
        } else {// batt lvl is good, but we know fuel lvl is low
            do nothing
        }

        // fuel
        if (10 < fuelLvl <= 50) {
            blink LED1 at 2 sec
        } else if ( fuelLvl <= 10) {
            blink LED1 at 1 sec
            fuelLow = true;
        } else {// fuel lvl is good, but we know batt lvl waslow
            do nothing
        }
    }*/

}
