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
	// Only run this function every major cycle
	static unsigned long start = 0;
	if((GLOBALCOUNTER - start) % MAJOR_CYCLE != 0) {
      return;
	}
    start = GLOBALCOUNTER;
	
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
   
    char ch = getch();
    printf("Input Char is %c", ch);
    // 2. Print satellite status and annunciation onto 
	// 	  the satellite terminal.
    /*fprintf(stdout, "\033[2J");
    fprintf(stdout, "\033[10;10H");
    fprintf(stdout, "SATELLITE TERMINAL: ----------------\n");
    fprintf(stdout, "**Satellite Status/n"    	    
	       "Solar Panels: %9s, " 
           "Battery Level: %3hu, "
           "Fuel Level: %3hu, "
           "Power Consumption: %2hu, "
		   "Power Generation: %2hu\n", 
		   solarPanelString, *batteryLvl, *fuelLvl, *pConsume, *pGenerate);
		   
	fprintf(stdout, "Annunciaton"
		   "Battery Low: %3s "
		   "Fuel Low: %3s",		   
		   battString, fuelString); 
	fflush(stdout);
    */	
}
