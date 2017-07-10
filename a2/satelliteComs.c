/*
 * satelliteComs.c - This file contains
 * the implementation of satelliteComs
 * (see satelliteComs.h for specification)
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
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
    uint16_t MASK = 0xFFF3; // used to set last 4 bits to 00XX 
    *thrusterCommand = randomInteger(0,MAX)%MAX & MASK;
    // printf("thrusterCommand = %d\n", *thrusterCommand);
  printf("value is b%s\n", mask); 
}
