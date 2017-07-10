#include <stdio.h>
#include <stdbool.h>
#include "dataStructs.h"

// Refer to p.14
void consoleDisplay(void *consoleStruct) {
    // 1. Assign the data of consoleStruct into local variables
    consoleData *cData = (consoleData*)consoleStruct;
    bool *fuelLow = cData->fuelLowPtr;
    bool *batteryLow = cData->batteryLowPtr;
    bool *solarPanelState = cData->solarPanelStatePtr;
    unsigned short *batteryLvl = cData->batteryLvlPtr;
    unsigned short *fuelLvl = cData->fuelLvlPtr;
    unsigned short *pConsume = cData->pConsumePtr;
    unsigned short *pGenerate = cData->pGeneratePtr;

    // Satellite Status
    char *solarPanelString = (*solarPanelState) ? "Deployed":"Retracted";
    printf("Solar Panels: %9s, " 
           "Battery Level: %3hu, "
           "Fuel Level: %3hu, "
           "Power Consumption: %2hu\r", 
           solarPanelString, *batteryLvl, *fuelLvl, *pConsume); 
    fflush(stdout);
}
