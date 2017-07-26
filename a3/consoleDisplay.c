/*
 * consoleDisplay.c - This file contains
 * the implementation of consoleDisplay
 */

#include <stdio.h>
#include <stdbool.h>
#include <termios.h>
#include <unistd.h>
#include<sys/select.h>
#include<sys/time.h>
#include "nonBlockingKeys.h"
#include "dataStructs.h"
#include "TCB.h"
#include <fcntl.h>

#define MAX 1024
#define SATELLITESTATUS 'z'
#define ANNUNCIATION 'x'

//#define OFF
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
	/*
	static unsigned long start = 0;
	if((GLOBALCOUNTER - start) % MAJOR_CYCLE != 0) {
      return;
	}
    start = GLOBALCOUNTER;
	*/
	
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
   
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
	
	char c = getchar();
	printf("CONSOLE: c = %d, %c\n", c, c);
	usleep(1000000);	
	if (c != 255) {
		char output[MAX];	
		if (c == SATELLITESTATUS) {
			sprintf(output, "**Satellite Status\n"    	    
							"Solar Panels: %9s, " 
							"Battery Level: %3hu, "
							"Fuel Level: %3hu, "
							"Power Consumption: %2hu, "
							"Power Generation: %2hu\n", 
							 solarPanelString, *batteryLvl, *fuelLvl, *pConsume, *pGenerate);
			terminalComs(output);								 
		} else if (c == ANNUNCIATION) {
			sprintf(output, "Annunciaton\n"
							"Battery Low: %3s "
							"Fuel Low: %3s",
							 battString, fuelString);
			terminalComs(output);
		} else {
			if(c == 'V') ungetc(c, stdin);
		}
	}
    
	
    
    #ifdef OFF
    static char c;
    static int i; 	
    // 2. Print satellite status and annunciation onto 
    // 	  the satellite terminal.
    //fprintf(stdout, "\033[2J");
    //fprintf(stdout, "\033[1;1H");
    //nonblock(NB_ENABLE);
    i=kbhit();
    if (i!=0)
    {
		char output[MAX]; 
		c=fgetc(stdin);
		printf("character got: %c\n", c);
		if (c == SATELLITESTATUS) {
			sprintf(output, "**Satellite Status\n"    	    
						"Solar Panels: %9s, " 
							"Battery Level: %3hu, "
							"Fuel Level: %3hu, "
							"Power Consumption: %2hu, "
							"Power Generation: %2hu\n", 
							 solarPanelString, *batteryLvl, *fuelLvl, *pConsume, *pGenerate);
		} else if (c == ANNUNCIATION) {	   
			sprintf(output, "Annunciaton\n"
							"Battery Low: %3s "
							"Fuel Low: %3s",
							 battString, fuelString); 
		} else {
			sprintf(output, "%c for Satellite Status\n%c for annunciation",
							 SATELLITESTATUS, ANNUNCIATION);
		}
		terminalComs(output);		
    }
    //fprintf(stdout, "SATELLITE TERMINAL: ----------------\n");

    #endif

}
