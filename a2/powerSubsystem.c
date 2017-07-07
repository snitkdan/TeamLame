#include <stdio.h>
#include "dataStructs.h"
#include "Bool.h"

/*
  @param pData
    Struct containing necessary
    data for the powerSubsystem.
  @modifies
    *pData.batteryLvlPtr &&
    *pData.solarPanelStatePtr &&
    *pData.pConsumePtr &&
    *pData.pGeneratePtr
  @effects
    changes batteryLvlPtr based
    on solarPanelStatePtr, pConsumePtr,
    and pGeneratePtr.
    (see sub-methods for details.)
*/
void powerSubsystem(void *pData);

/*
  @param solarPanelState
    whether solar panel is ON or OFF
  @param pGenerate
    current value of power generation
  @param batteryLvl
    current value of the battery level
  @modifies
    pGenerate && solarPanelState
  @effects
    changes pGenerate based on solarPanelState
  @returns
    TRUE if the exit state of the solar panel is ON.
*/
bool useSolarPanels(bool *solarPanelState, unsigned short *pGenerate);

/*
  @param pConsume
    current value of power consumption
  @modifies pConsume
  @effects
    increments pConsume by 2
    every even numbered function call, and
    decriments it by 1 otherwise. If pConsume
    exceeds 10, these conditions are reversed;
    after which, if pConsume falls below 5,
    the original conditions are reinstated.
*/
void powerConsumption(unsigned short *pConsume);

/*
  @param pGenerate
    current value of power generation
  @modifies pGenerate
  @effects
    If batteryLvl <= 50%, increments pGenerate by 2
    every even numbered function call, and by 1 every odd.
    If 50% < batteryLvl <= 95%, increments pGenerate by 2
    every even numbered function call.
    Otherwise, do nothing.
*/
void powerGeneration(unsigned short *pGenerate, unsigned short *batteryLvl);



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
