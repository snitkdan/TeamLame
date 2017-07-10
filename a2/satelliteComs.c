/*
 * satelliteComs.c - This file contains
 * the implementation of satelliteComs
 * (see satelliteComs.h for specification)
*/

#include <stdio.h>
#include <stdbool.h>
#include "dataStructs.h"
#include "TCB.h"

void satelliteComs(void *sData) {
    // 1. Assign the data of satStruct into local variables
    satData *satStruct = (satData*)sData;
    bool *fuelLow = satStruct->fuelLowPtr;
    bool *batteryLow = satStruct->batteryLowPtr;
    bool *solarPanelState = satStruct->solarPanelStatePtr;
    unsigned short *batteryLvl = satStruct->batteryLvlPtr;
    unsigned short *fuelLvl = satStruct->fuelLvlPtr;
    unsigned short *pConsume = satStruct->pConsumePtr;
    unsigned short *pGenerate = satStruct->pGeneratePtr;
    unsigned int *thrusterCommand = satStruct->thrusterCommandPtr;

    // 2. Retrieve random number and assign thrusterCommand to it
    *thrusterCommand = randomInteger(0,10)%7;
    printf("thrusterCommand = %d\n", *thrusterCommand);
}
