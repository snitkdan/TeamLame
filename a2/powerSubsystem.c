/*

  powerSubsystem.c - This file contains
  the implementation of powerSubsystem.
  (see powerSubsystem.h for specification).

*/

#include <stdio.h>
#include <stdbool.h>
#include "dataStructs.h"
#include "Bool.h"
#include "powerSubsystem.h"

void powerSubsystem(void *pData) {
  // 1. Assign the data of pData into local variables
  (powerData*) powerStruct = (powerData*)pData;
  bool *solarPanelState = powerStruct->solarPanelStatePtr;
  unsigned short *batteryLvl = powerStruct->batteryLvlPtr;
  unsigned short *pConsume = powerStruct->pConsumePtr;
  unsigned short *pGenerate = powerStruct->pGeneratePtr;
  // 2. Update powerConsumption
  powerConsumption(pConsume);
  // 3. Check the solar panels & update batteryLvl accordingly
  if (useSolarPanels(solarPanelState, pGenerate, batteryLvl)) {
    // 3,1: batteryLvl = batteryLvl - pConsume + pGenerate
    *batteryLvl += *pGenerate - *pConsume;
  } else {
    // 3.2: batteryLvl = batteryLvl - pConsume
    *batteryLvl -= *pConsume;
  }
}

bool useSolarPanels(bool *solarPanelState, unsigned short *pGenerate) {
  // 1. If solarPanelState == ON
  if(*solarPanelState == TRUE) {
    // 1.1: If  batteryLvl > 95%
    if(*batteryLvl > 95) {
      // 1.1.1: Retract solar panels
      *solarPanelState = FALSE;
    } else {
      // 1.2: Update powerGeneration
      powerGeneration(pGenerate, batteryLvl);
    }
  }
  // 2. If solarPanelState == OFF
  else {
      // 2.1: If batteryLvl <= 10%
      if(batteryLvl <= 10) {
        // 2.1.1: Deploy solar panels
        *solarPanelState = TRUE;
      }
  }
  // 3. Return the current state of the solar panel
  return *solarPanelState;
}

void powerGeneration(unsigned short *pGenerate, unsigned short *batteryLvl) {
  // 1. Define static variables to track function state
  static bool halfCapacity = false;
  static short numCalls = 0;
  // 2. If battery level <= 95%
  if(*batteryLvl <= 95) {
    // 2.1: If battery level <= 50%
    if(*batteryLvl <= 50) {
      *pGenerate += numCalls % 2 ? 2 : 1;
       // if even call -> +2; else -> +1
    }
    // 2.2: If battery level > 50%
    else {
      *pGenerate += numCalls % 2 ? 2 : 0;
        // if even call -> +2; else -> +0
    }
  }
  // 3. Update the function call number
  numCalls++;
}

void powerConsumption(unsigned short *pConsume) {
    // 1. Define static variables to track function state
    static bool isReversed = true;
    static short numCalls = 0;
    // 2. If in reversed condition
    if(isReversed) {
      // 2.1: Reinstate original condition.
      if(*pConsume < 5) {
        isReversed = !isReversed;
      }
    }
    // 3. If not in reversed condition
    else {
      if(*pConsume > 10) {
        // 3.1: Transition to reversed condition
        isReversed = !isReversed;
      }
    }
    // 4. If calls is even
    if(calls % 2 == 0) {
      *pConsume += isReversed ? -1 : 2;
        // if reversed -> -1; else -> +2
    }
    // 5. If calls is odd
    else {
      *pConsume += isReversed ? 2 : -1;
        // if reversed -> +2; else -> -1
    }
    // 6. Update the function call number
    numCalls++;
}
