/*
 * consoleDisplay.c - This file contains
 * the implementation of consoleDisplay
 */

#include <stdio.h>
#include <stdbool.h>
#include "dataStructs.h"
#include "TCB.h"

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
    // unsigned short *pGenerate = cData->pGeneratePtr; // FOR FUTURE PROJECTS

    // DEBUG. Temporary flipping of 1 and 0 (won't be part of the final code)
    static int numCalls = 0;
    numCalls = 1 - numCalls;

    // 1.2 Define necessary string storage 
    char *solarPanelString = (*solarPanelState) ? "Deployed":"Retracted";
    char *fuelString = (*fuelLow)? "YES":"NO";
    char *battString = (*batteryLow)? "YES":"NO";
    //char *fuelString = (numCalls == 0)? "YES":"NO";
    //char *battString = (numCalls == 1)? "YES":"NO";
    char satelliteStatus[100];
    char annunciation[100];
    
    // 2. Store print statements for satellite status into variable
    snprintf(satelliteStatus, 100, 
           "Solar Panels: %9s, " 
           "Battery Level: %3hu, "
           "Fuel Level: %3hu, "
           "Power Consumption: %2hu\r", 
           solarPanelString, *batteryLvl, *fuelLvl, *pConsume); 

    // 3. Store print statement for annunciation mode into variable
    snprintf(annunciation, 100,
           "ANNUNCIATION: "
           "Battery Low: %3s " 
           "Fuel Low: %3s\r",
           battString, fuelString);
    
    // 4. Pass in the strings to terminalComs, which will
    //    be displayed on this terminal and another terminal. 
    terminalComs(satelliteStatus, annunciation);
    //terminalComs(annunciation, satelliteStatus);
}
