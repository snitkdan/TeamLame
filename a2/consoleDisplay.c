/*
 * consoleDisplay.c - This file contains
 * the implementation of consoleDisplay
 */

#include <stdio.h>
#include <stdbool.h>
#include "dataStructs.h"
#include "TCB.h"
#define MAX 300

/*
  @param consoleStruct
    struct containing the shared 
    data for consoleDisplay.
  @modifies
    nothing    
  @effects
    stores the data from the shared variables
    in the form of satelliteStatus and 
    annunciation, passing them to terminalComs.c
*/
void consoleDisplay(void *consoleStruct) {
    // 1.1 Assign the data of consoleStruct into local variables
    consoleData *cData = (consoleData*)consoleStruct;
    bool *fuelLow = cData->fuelLowPtr;
    bool *batteryLow = cData->batteryLowPtr;
    bool *solarPanelState = cData->solarPanelStatePtr;
    unsigned short *batteryLvl = cData->batteryLvlPtr;
    unsigned short *fuelLvl = cData->fuelLvlPtr;
    unsigned short *pConsume = cData->pConsumePtr;
    unsigned short *pGenerate = cData->pGeneratePtr;

    // 1.2 Define necessary string storage 
    char *solarPanelString = (*solarPanelState) ? "Deployed":"Retracted";
    char *fuelString = (*fuelLow)? "YES":"NO";
    char *battString = (*batteryLow)? "YES":"NO";
    //char *fuelString = (numCalls == 0)? "YES":"NO";
    //char *battString = (numCalls == 1)? "YES":"NO";
    char output[MAX];
    
    // 2. Store print statements for satellite status into variable
    snprintf(output, MAX, 
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

    // 4. Pass in the strings to terminalComs, which will
    //    be displayed on this terminal and another terminal. 
    //terminalComs(satelliteStatus, annunciation);
    //terminalComs(annunciation, satelliteStatus);
    terminalComs(output);
}
