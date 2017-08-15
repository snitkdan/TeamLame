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
#include "satComsParse.h"

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
	unsigned int *pirateDistance = sData->pirateDistancePtr;	

    // 2. Retrieve random number, mask and assign thrusterCommand to it
    *thrusterCommand = randomInteger(0, MAX) % MAX;
    //maskBit(thrusterCommand);



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
        if (validCmd(pString[0])) {
		  strcpy(received, pString);
		  *commandOn = true; // Appends command parser next cycle
        } else if (strstr(pString, "clear")) {
		    printf("\033[2J");
		    printf("\033[1;1H");
		} else {
		    *received = '\0';
			if (checkAll(pString[0])) {
				int i;
				for (i = strlen(pString); i >= 0; i--) {
				  ungetc(pString[i], stdin);
				}
			} else {
				*transmit = 'E';
				printf("Transmit: %c\n", *transmit);
			}
        }  
    }
	
    char *fuelString = (*fuelLow)? "YES":"NO";
    char *battString = (*batteryLow)? "YES":"NO";
    char *solarPanelString = (*solarPanelState) ? "Deployed":"Retracted";	
	char *warnBattString = warningBattTemp? "OVERHEATING!":"OK";	
	
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    dprintf(fd1, "\033[2J");	
    dprintf(fd1, "\033[1;1H");
    dprintf(fd1, "EARTH REMOTE TERMINAL\n");
    dprintf(fd1, "%d-%d-%d %d:%d:%d\n", 
	             tm.tm_year + 1950, 
				 tm.tm_mon + 1, 
				 tm.tm_mday, 
				 tm.tm_hour, 
				 tm.tm_min, 
				 tm.tm_sec);
    dprintf(fd1, "Operator:          Xxpu$$y$layer69xX\n"
	             "--------------------------------------\n\n");	
    dprintf(fd1, "Battery Low:       %s\n"
	             "Fuel Low:          %s\n"
    	         "Battery Over Temp: %s\n", 
				 battString, fuelString, warnBattString);				 
	
	switch(*transmit) {
		case SHOW_FUEL:
		    dprintf(fd1, "Fuel Level: %hu\n", *fuelLvl);
			break;
		case SHOW_BATT:
		    dprintf(fd1, "Battery Level: %hu\n", *batteryLvl);
			break;
        case SHOW_PCON:
		    dprintf(fd1, "Power Consumed: %hu\n", *pConsume);
            break;			
		case SHOW_TEMP:		
		    dprintf(fd1, "Battery Temp 1: %d\n"
			             "Battery Temp 2: %d\n", 
						 *batteryTmp1, *batteryTmp2);
			break;					
		case SHOW_PANEL:
		    dprintf(fd1, "Solar Panels: %s\n", solarPanelString);
			break;			
		case SHOW_DIST:
		    dprintf(fd1, "Transport Distance: %d\n", *distance);
			break;			
		case SHOW_IMAG:
		    dprintf(fd1, "Image Frequency: %hu\n", *processImage);
			break;			
		case SHOW_PIRATE:
		    dprintf(fd1, "Pirate Distance: %hu\n", *pirateDistance);
			break;			
	}
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

bool validCmd(char c) {
	return c == START ||
	       c == STOP ||
		   c == DISPLAY ||
		   c == THRUSTER ||
		   c == MEASURE;
}

/*
void maskBit(unsigned int *thrusterCommand) {
    // 0. Define a mask 1111111111110011
    uint16_t MASK = 0xFFF3;

    // 1. Mask the bit 2 and 3 to 0
    *thrusterCommand &= MASK;
}
*/