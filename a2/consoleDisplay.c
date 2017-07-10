#include <stdio.h>
#include <stdbool.h>
#include "dataStructs.h"
#include "TCB.h"

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
    static int numCalls = 0;
    numCalls = 1 - numCalls;
    // Satellite Status
    char *solarPanelString = (*solarPanelState) ? "Deployed":"Retracted";
    //char *fuelString = (*fuelLow)? "YES":"NO";
    //char *battString = (*batteryLow)? "YES":"NO";
    char *fuelString = (numCalls == 0)? "YES":"NO";
    char *battString = (numCalls == 1)? "YES":"NO";
    char satelliteStatus[100];
    char annunciation[100];
    snprintf(satelliteStatus, 100, 
           "\rSolar Panels: %9s, " 
           "Battery Level: %3hu, "
           "Fuel Level: %3hu, "
           "Power Consumption: %2hu", 
           solarPanelString, *batteryLvl, *fuelLvl, *pConsume); 
    snprintf(annunciation, 100,
           "\rANNUNCIATION: "
           "Battery Low: %3s " 
           "Fuel Low: %3s",
           fuelString, battString);
     
    terminalComs(satelliteStatus, annunciation);
    //terminalComs(annunciation, satelliteStatus);
    fflush(stdout);
}
