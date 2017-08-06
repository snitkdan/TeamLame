/*

  powerSubsystem.c - This file contains
  the implementation of powerSubsystem.
  (see powerSubsystem.h for specification).

*/

#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include "dataStructs.h"
#include "powerSubsystem.h"
#include "adc_utils.h"

#define ACH "AIN0"
#define HNUM 16

#define BUF_SIZE 16
//#define DEBUG

extern bool fromPowerSS;

#ifndef DEBUG
extern unsigned int batteryBuff[BUF_SIZE];
#endif

void powerSubsystem(void *powerStruct) {

fromPowerSS = true;
  // Only run this function every major cycle
  static unsigned long start = 0;
  if((GLOBALCOUNTER - start) % MAJOR_CYCLE != 0) {
      return;
  }
  start = GLOBALCOUNTER;
  // 1. Assign the data of powerStruct into local variables
  powerData *pData = (powerData*)powerStruct;
  bool *solarPanelState = pData->solarPanelStatePtr;
  unsigned int *batteryLvl = pData->batteryLvlPtr;
  unsigned short *pConsume = pData->pConsumePtr;
  unsigned short *pGenerate = pData->pGeneratePtr;
  bool *solarPanelDeploy = pData->solarPanelDeployPtr;
  bool *solarPanelRetract = pData->solarPanelRetractPtr;
  // 2. Update the buffer
  static unsigned int current_measurement = 0;
  #ifndef DEBUG
  int next = readADC(ACH, HNUM);
  batteryBuff[current_measurement] = next;
  current_measurement = (current_measurement + 1) % BUF_SIZE;
  *batteryLvl = next;
  #endif
  #ifdef DEBUG
  unsigned int batteryBuff[BUF_SIZE] = {60, 10, 5, 10, 30, 95, 100, 50, 30, 10, 5, 0, 30, 50, 95, 100};
  *batteryLvl = batteryBuff[current_measurement];
  current_measurement = (current_measurement + 1) % BUF_SIZE;
  #endif

  // 2. Update powerConsumption && powerGeneration
  // TODO:
  // 3. rescaling v to mv
  // 4. Change battery level pointer rescaled!!
  powerConsumption(pConsume);
  useSolarPanels(solarPanelState, solarPanelDeploy, solarPanelRetract, pGenerate, batteryLvl);
}

bool useSolarPanels(bool *solarPanelState, bool *solarPanelDeploy, bool *solarPanelRetract, unsigned short *pGenerate, unsigned int *batteryLvl) {
  // 1. If solarPanelState == ON
  if(*solarPanelState) {
    *solarPanelDeploy = true;
    *solarPanelRetract = false;
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
	  *solarPanelDeploy = false;
	  *solarPanelRetract = true;
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
void powerGeneration(unsigned short *pGenerate, unsigned int *batteryLvl) {
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
