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
#include <time.h>


#include "TCB.h"
#include "dataStructs.h"
#include "satelliteComs.h"
#include "nonBlockingKeys.h"
#include "satelliteVehicle.h"
#include "adc_utils.h"

#define MAX 65536 // upper bound for 16 bit
#define BUF_SIZE 16
#define CMD_SIZE 20
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
    int *processImage = sData->processImagePtr;
    unsigned int *batteryLvl = sData->batteryLvlPtr;
    unsigned short *fuelLvl = sData->fuelLvlPtr;
    unsigned short *pConsume = sData->pConsumePtr;
    unsigned short *pGenerate = sData->pGeneratePtr;
    unsigned int *thrusterCommand = sData->thrusterCommandPtr;
    char *command = sData->commandPtr;
    char *response = sData->responsePtr;
    unsigned int *distance = sData->distancePtr;	
    unsigned int *batteryTmp1 = sData->batteryTmp1;
    unsigned int *batteryTmp2 = sData->batteryTmp2;
    char *received = sData->received;
    char *transmit = sData->transmit;
	bool *commandOn = sData->commandOnPtr;

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
	
	int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
	char pString[CMD_SIZE];
    pString[0] = '\0';	
    if(fgets(pString, CMD_SIZE, stdin) != NULL) {
       // remove newline
       pString[strcspn(pString, "\n")] = 0;
       if (pString[0] == 'T' || pString[0] == 'M') {
		  strcpy(received, pString);
		  *commandOn = true;
       } else {
		   *received = '\0';
           int i;
           for (i = strlen(pString); i >= 0; i--) {
               ungetc(pString[i], stdin);
           }
       }  
    }
	
	
	
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    dprintf(fd1, "\033[2J");	
    dprintf(fd1, "\033[1;1H");
    dprintf(fd1, "EARTH REMOTE TERMINAL\n");
    dprintf(fd1, "%d-%d-%d %d:%d:%d\n", tm.tm_year + 1950, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    dprintf(fd1, "Operator:          Xxpu$$y$layer69xX\n--------------------------------------\n\n");	
	/*	
    dprintf(fd1, "Battery Low:       %s\n"
	             "Fuel Low:          %s\n"
    	         "Battery Over Temp: %s\n"			 
	             "Solar Panels:      %s\n"
                 "Battery Level:     %u\n"
                 "Fuel Level:        %hu\n"
                 "Power Consumption: %hu\n"
                 "Power Generation:  %hu\n"
                 "Transport Dist:    %d\n"
                 "Battery Temp 1:    %d\n"
                 "Battery Temp 2:    %d\n"				 
                 "Image Data:        %d\n",
              battString, fuelString, warnBattString, solarPanelString,
             *batteryLvl, *fuelLvl, *pConsume, *pGenerate,			  
              *distance, *batteryTmp1, *batteryTmp2, 
                *processImage);			  

	 if (strstr(response, "A")) {  
		 dprintf(fd1, "\nVehicle Response: %c %c\n", *response, *command);
     }
	*/ 
}

void maskBit(unsigned int *thrusterCommand) {
    // 0. Define a mask 1111111111110011
    uint16_t MASK = 0xFFF3;

    // 1. Mask the bit 2 and 3 to 0
    *thrusterCommand &= MASK;
}
