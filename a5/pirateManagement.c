/*
 * pirateManagement.c - This file contains
 * the implementation of pirateManagement
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
#include "nonBlockingKeys.h"
#define CMD_SIZE 20

#define PIN0 "20"
#define PIN1 "115"

FILE *phasor = NULL;
FILE *photon = NULL;

void initiatePins();

void pirateManagement(void *pmStruct) {
	static int firstTime = 0;
	if (firstTime == 0) {
        initiatePins();
	    firstTime++;
	}
    // 1. Assign the data of pData into local variables
    mPirateData *pData = (mPirateData*)pmStruct;
    bool *pirateDetected = pData->pirateDetectedPtr;
	unsigned int *pirateDistance = pData->pirateDistancePtr;
    // Note: Shared variables are not added. Placeholders are added.
	//printf("inside pirate management\n");
    phasor = fopen("/sys/class/gpio/gpio"PIN0"/value", "w");
	photon = fopen("/sys/class/gpio/gpio"PIN1"/value", "w");	
    if (*pirateDistance <= 30) {
		char pString[CMD_SIZE];
		pString[0] = '\0';	
		if(fgets(pString, CMD_SIZE, stdin) != NULL) {
		   // remove newline
		   pString[strcspn(pString, "\n")] = 0;
		}
		
		if(checkAll(pString[0]) && !piratesCmd(pString[0])) {
			int i = 0;
			for (i = strlen(pString); i >= 0; i--) {
			   ungetc(pString[i], stdin);
			}
			return;
		}
		if (*pirateDistance <= 5 && pString[0] == 'o') {
			dprintf(fd0, "\e[H\e[2J");
			int i = 0;
			printf(RED BOLD"Photons initiating...\n"RST);			
            for (i = 0; i < 5; i++) { 			
			    printf(BOLD"EAT "RED"P"YEL"H"GRN"O"CYN"T"MAG"O"WHT"N"RED"S "WHT "BITCHES \n"RST);
                fflush(stdout);				
				fprintf(photon, "%d", 1); fflush(photon);
				usleep(200000);
				fprintf(photon, "%d", 0); fflush(photon);
				usleep(200000);				
            }		
			printf("\n\n");
				
			
		    
		} else if (*pirateDistance > 5 && pString[0] == 'p') {
			dprintf(fd0, "\e[H\e[2J");			
			printf(YEL BOLD"Phasors initiated.\n\n"RST);
			fprintf(phasor, "%d", 1); fflush(phasor);
			usleep(1000000);
			fprintf(phasor, "%d", 0); fflush(phasor);
			
		}
	}
	fclose(phasor);
	fclose(photon);

	
}

void initiatePins() {
	system("echo "PIN0" > /sys/class/gpio/export");
	system("echo "PIN1" > /sys/class/gpio/export");
	system("echo out > /sys/class/gpio/gpio"PIN0"/direction");
	system("echo out > /sys/class/gpio/gpio"PIN1"/direction");



}
