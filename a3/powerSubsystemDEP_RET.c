/*

  powerSubsystem.c - This file contains
  the implementation of powerSubsystem.
  (see powerSubsystem.h for specification).

*/

#include <stdio.h>
#include <stdbool.h>
#include "dataStructs.h"
#include "powerSubsystem.h"
#include "adc_utils.h"

#define ACH "AIN0"
#define BUF_SIZE 16

int current_measurement = 0;

void powerSubsystem(void *powerStruct) {
  // Only run this function every major cycle
  static unsigned long start = 0;
  if((GLOBALCOUNTER - start) % MAJOR_CYCLE != 0) {
      return;
  }
  start = GLOBALCOUNTER;
  // 1. Assign the data of powerStruct into local variables
  powerData *pData = (powerData*)powerStruct;
  bool *solarPanelState = pData->solarPanelStatePtr;
  bool *solarPanelDeploy = pData->solarPanelDeployPtr;
  bool *solarPanelRetract = pData->solarPanelRetractPtr;
   
  unsigned short *batteryLvl = pData->batteryLvlPtr;
  unsigned short *pConsume = pData->pConsumePtr;
  unsigned short *pGenerate = pData->pGeneratePtr;
  // 2. Update powerConsumption && powerGeneration
  powerConsumption(pConsume);
  useSolarPanels(solarPanelState, pGenerate, batteryLvl, solarPanelDeploy, solarPanelRetract);
  // 3. Get the next measurement
  if(!initADC()) {
    fprintf(stderr, "ADC Malfunction\n");
    return;
  }
  // 4. Update the buffer
  int next = nextMeasurement();
  *batteryLvl[current_measurement] = next;
  current_measurement = (current_measurement + 1) % BUF_SIZE;
}

static int nextMeasurement() {
  return readADC(ACH);
}

bool useSolarPanels(bool *solarPanelState, unsigned short *pGenerate, unsigned short *batteryLvl, bool *solarPanelDeploy, bool *solarPanelRetract) {
  // 1. If solarPanelState == ON
  if(*solarPanelState) {
    *solarPanelDeploy = !(*solarPanelDeploy);
    *solarPanelRetract = !(*solarPanelRetract);
    // 1.1: If  batteryLvl > 95%
    if(*batteryLvl > 95) {
      // 1.1.1: Retract solar panels
      *solarPanelState = false;
      *pGenerate = 0;
    } else {
      // 1.2: Update powerGeneration
      powerGeneration(pGenerate, batteryLvl);
    }
  }
  // 2. If solarPanelState == OFF
  else {
      // 2.1: If batteryLvl <= 10%
      if(*batteryLvl <= 10) {
        // 2.1.1: Deploy solar panels
        *solarPanelState = true;
      }
  }
  // 3. Return the current state of the solar panel
  return *solarPanelState;
}

// Motar Drive -> set motar drive to 100 when solar panel state changes. After it changes, set it back to 0.
void powerGeneration(unsigned short *pGenerate, unsigned short *batteryLvl) {
  // 1. Define static variables to track function state
  static short numCalls = 0;
  // 2. If battery level <= 95%
  if(*batteryLvl <= 95) {
    // 2.1: If battery level <= 50%
    if(*batteryLvl <= 50) {
      *pGenerate += (numCalls % 2 == 0) ? 2 : 1;
       // if even call -> +2; else -> +1
    }
    // 2.2: If battery level > 50%
    else {
      *pGenerate += (numCalls % 2 == 0) ? 2 : 0;
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
    if(numCalls % 2 == 0) {
      *pConsume += isReversed ? -2 : 2;
        // if reversed -> -2; else -> +2
    }
    // 5. If calls is odd
    else {
      *pConsume += isReversed ? 1 : -1;
        // if reversed -> +2; else -> -1
    }
    // 6. Update the function call number
    numCalls++;
}
