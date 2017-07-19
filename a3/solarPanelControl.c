/*
 * solarPanelControl.c - This file contains
 * the implementation of solarPanelControl
 */

#include <stdio.h>
#include <stdbool.h>
#include "dataStructs.h"
#include "TCB.h"
#define MAX 300

void solarPanelControl(void *solarStruct) {
	// Only run this function every major cycle
	static unsigned long start = 0;
	if((GLOBALCOUNTER - start) % MAJOR_CYCLE != 0) {
      return;
	}
    start = GLOBALCOUNTER;
	
    // 1.1 Assign the data of consoleStruct into local variables
    solarData *solData = (solarData*)solarStruct;
    bool *solarPanelState = solData->solarPanelStatePtr;
    bool *solarPanelDeploy = solData->solarPanelDeployPtr;
    bool *solarPanelRetract = solData->solarPanelRetractPtr;
    bool *motorInc = solData->motorIncPtr;
    bool *motorDec = solData->motorDecPtr;

    // Have fun Abdul - Love, Rad
}
