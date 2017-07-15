/*
 * satelliteComs.c - This file contains
 * the implementation of satelliteComs
 * (see satelliteComs.h for specification)
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "TCB.h"
#include "dataStructs.h"
#include "satelliteComs.h"
#define MAX 65536 // upper bound for 16 bit

void satelliteComs(void *satStruct) {
	static unsigned long start = 0;
	//printf("start: %lu GC - start: %lu GC:%lu\n", start, GLOBALCOUNTER - start, GLOBALCOUNTER);
	if((GLOBALCOUNTER - start) % MAJOR_CYCLE != 0) {
      return;
	}
    start = GLOBALCOUNTER;
    // 1. Assign the data of sData into local variables
    satData *sData = (satData*)satStruct;
    bool *fuelLow = sData->fuelLowPtr;
    bool *batteryLow = sData->batteryLowPtr;
    bool *solarPanelState = sData->solarPanelStatePtr;
    unsigned short *batteryLvl = sData->batteryLvlPtr;
    unsigned short *fuelLvl = sData->fuelLvlPtr;
    unsigned short *pConsume = sData->pConsumePtr;
    unsigned short *pGenerate = sData->pGeneratePtr;
    unsigned int *thrusterCommand = sData->thrusterCommandPtr;

    // 2. Retrieve random number, mask and assign thrusterCommand to it
    *thrusterCommand = randomInteger(0, MAX) % MAX;
    maskBit(thrusterCommand);
	
    char *solarPanelString = (*solarPanelState) ? "Deployed":"Retracted";
    char *fuelString = (*fuelLow)? "YES":"NO";
    char *battString = (*batteryLow)? "YES":"NO";
    char output[MAX];
    
    // 2. Store print statements for satellite status into variable
    snprintf(output, MAX,
		   "EARTH: -----------\n"	
           "Solar Panels: %9s, " 
           "Battery Level: %3hu, "
           "Fuel Level: %3hu, "
           "Power Consumption: %2hu, "
		   "Power Generation: %2hu\n"
		   "ANNUNCIATION: "
		   "Battery Low: %3s "
		   "Fuel Low: %3s\n",		   
           solarPanelString, *batteryLvl, *fuelLvl, *pConsume, *pGenerate,
		   battString, fuelString); 

    // 4. Pass in the string to terminalComs, which will
    //    be displayed on this terminal and another terminal. 
    terminalComs(output);	
}

void maskBit(unsigned int *thrusterCommand) {
    // 0. Define a mask 1111111111110011
    uint16_t MASK = 0xFFF3;

    // 1. Mask the bit 2 and 3 to 0
    *thrusterCommand &= MASK;
}
