/*
 * satelliteComs.c - This file contains
 * the implementation of satelliteComs
 * (see satelliteComs.h for specification)
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "satelliteComs.h"
#include "dataStructs.h"
#include "TCB.h"
#define MAX 65536 // upper bound for 16 bit



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

    // 2. Retrieve random number, mask and assign thrusterCommand to it
    *thrusterCommand = randomInteger(0, MAX) % MAX;
    maskBit(thrusterCommand);
    // printf("thrusterCommand = %d\n", *thrusterCommand);
}

void maskBit(unsigned int *thrusterCommand) {
    // 0. Define a mask 1111111111110011
    uint16_t MASK = 0xFFF3;
    
    // 1. Mask the bit 2 and 3 to 0
    *thrusterCommand &= MASK;
}
