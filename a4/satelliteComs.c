/*
 * satelliteComs.c - This file contains
 * the implementation of satelliteComs
 * (see satelliteComs.h for specification)
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


#include "TCB.h"
#include "dataStructs.h"
#include "satelliteComs.h"
#include "nonBlockingKeys.h"
#include "satelliteVehicle.h"
#include "adc_utils.h"

#define MAX 65536 // upper bound for 16 bit
#define BUF_SIZE 16
//extern unsigned int current_measurement;
//extern unsigned int batteryBuff[BUF_SIZE];

#define DEBUG
int fd = 0;

extern bool warningBattTemp;

void satelliteComs(void *satStruct) {
	/*
	// Only runs this function every global cycle
	static unsigned long start = 0;
	if((GLOBALCOUNTER - start) % MAJOR_CYCLE != 0) {
      return;
	}
    start = GLOBALCOUNTER;
	*/
    // 1. Assign the data of sData into local variables
    satData *sData = (satData*)satStruct;
    bool *fuelLow = sData->fuelLowPtr;
    bool *batteryLow = sData->batteryLowPtr;
    bool *solarPanelState = sData->solarPanelStatePtr;
    unsigned int *batteryLvl = sData->batteryLvlPtr;
    unsigned short *fuelLvl = sData->fuelLvlPtr;
    unsigned short *pConsume = sData->pConsumePtr;
    unsigned short *pGenerate = sData->pGeneratePtr;
    unsigned int *thrusterCommand = sData->thrusterCommandPtr;
    char *command = sData->commandPtr;
    char *response = sData->responsePtr;
    char *request = sData->requestPtr;
    unsigned int *distance = sData->distancePtr;	
    unsigned int *batteryTmp1 = sData->batteryTmp1;
    unsigned int *batteryTmp2 = sData->batteryTmp2;


    // 2. Retrieve random number, mask and assign thrusterCommand to it
    *thrusterCommand = randomInteger(0, MAX) % MAX;
    maskBit(thrusterCommand);

    char *solarPanelString = (*solarPanelState) ? "Deployed":"Retracted";
    char *fuelString = (*fuelLow)? "YES":"NO";
    char *battString = (*batteryLow)? "YES":"NO";
	char *warnBattString = warningBattTemp? "OVERHEATING!":"OK";

	static int fd1;
	static int firstTime = 1;
	if (firstTime == 1) {
        fd1 = open("/dev/pts/1", O_WRONLY);
	    if (!fd1) {
		    fprintf(stderr, "SATCOMS: couldn't open fd1 earth terminal\n");
		    exit(EXIT_FAILURE);
	    }
		dprintf(fd1, "\033[2J");
		dprintf(fd1, "\033[1;1H");
	    firstTime--;
	}
    dprintf(fd1, "\033[2J");	
    dprintf(fd1, "\033[1;1H");
    dprintf(fd1, "EARTH DISPLAY TERMINAL\n");
    dprintf(fd1, "Solar Panels:      %s\n"
                 "Battery Level:     %u\n"
                 "Fuel Level:        %hu\n"
                 "Power Consumption: %hu\n"
                 "Power Generation:  %hu\n"
                 "Vehicle Distance:  %d\n"
                 "Battery Temp 1:    %d\n"
                 "Battery Temp 2:    %d\n"
	         "Battery Low:       %s\n"
	         "Fuel Low:          %s\n"
	         "Battery Over Temp: %s\n",
              solarPanelString, *batteryLvl, *fuelLvl, *pConsume, *pGenerate,
              *distance, *batteryTmp1, *batteryTmp2, 
              battString, fuelString, warnBattString);

	 if (strstr(response, "A")) {  
		 dprintf(fd1, "\nVehicle Response: %c %c\n", *response, *command);
         }
}

void maskBit(unsigned int *thrusterCommand) {
    // 0. Define a mask 1111111111110011
    uint16_t MASK = 0xFFF3;

    // 1. Mask the bit 2 and 3 to 0
    *thrusterCommand &= MASK;
}
