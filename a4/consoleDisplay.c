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
#include "adc_utils.h"

#define MAX 1024
#define BUF_SIZE 16
#define SCALE 20

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
    //printf("INSIDE consoleDisplay\n");
    // 1.1 Assign the data of consoleStruct into local variables
    consoleData *cData = (consoleData*)consoleStruct;
    bool *fuelLow = cData->fuelLowPtr;
    bool *batteryLow = cData->batteryLowPtr;
    bool *solarPanelState = cData->solarPanelStatePtr;
    bool *batteryOverTemp = cData->batteryOverTempPtr;	
    unsigned int *batteryLvl = cData->batteryLvlPtr;
    unsigned short *fuelLvl = cData->fuelLvlPtr;
    unsigned short *pConsume = cData->pConsumePtr;
    unsigned short *pGenerate = cData->pGeneratePtr;
    unsigned int *distance = cData->distancePtr;	

    // 1.2 Define necessary string storage
    char *solarPanelString = (*solarPanelState) ? "Deployed":"Retracted";
    char *fuelString = (*fuelLow)? "YES":"NO";
    char *battString = (*batteryLow)? "YES":"NO";


    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
	//#define DEBUG
	#ifndef DEBUG
	char c = getchar();
	#endif
	
	#ifdef DEBUG
	char c = 'z';
	//printf("CONSOLE: c = %d, %c\n", c, c);	
	#endif
	
	char output[MAX];	
	//if (c != 255) {
		if (c == SATELLITESTATUS) {
			printf("ConsoleDisplay: Showing Satellite Status...\n");
			sprintf(output, "**Satellite Status\n"
							"Solar Panels: %9s, "
							"Battery Level: %3u, "
							"Fuel Level: %3hu, "
							"Power Consumption: %2hu, "
							"Power Generation: %2hu\n",
							 solarPanelString, *batteryLvl, *fuelLvl, *pConsume, *pGenerate);
			//terminalComs(output);
		} else if (c == ANNUNCIATION) {
			printf("ConsoleDisplay: Showing Annunciation Mode...\n");
			sprintf(output, "Annunciaton\n"
							"Battery Low: %3s "
							"Fuel Low: %3s",
							 battString, fuelString);
			//terminalComs(output);
		} else {
			if(satVehicleCmd(c) || motorSpeedCmd(c) || warningCmd(c)) ungetc(c, stdin);
		}
	//}
	terminalComs(output);

}
