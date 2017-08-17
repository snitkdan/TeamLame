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
#include <string.h>
#include "nonBlockingKeys.h"
#include "dataStructs.h"
#include "TCB.h"
#include <fcntl.h>
#include "adc_utils.h"

#define MAX 1024
#define BUF_SIZE 16
#define SCALE 20
#define CMD_SIZE 20

#define STATE_SAT 1
#define STATE_ANN 2
#define ONLY_ANN 3

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
extern bool warningBattTemp;
extern bool isPaused;
void returnToBuffer(char *pString);
void sendToPrint(char mode, void *consoleStruct);
static int state;

void consoleDisplay(void *consoleStruct) {
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
	
	char pString[CMD_SIZE];
    pString[0] = '\0';	
	if(fgets(pString, CMD_SIZE, stdin) != NULL) {
       // remove newline
       pString[strcspn(pString, "\n")] = 0;
	}
	if (isPaused) {
		state = ONLY_ANN;
		if (pString[0] == SATELLITESTATUS || pString[0] == ANNUNCIATION) {
			printf(YEL"ConsoleDisplay: Cannot switch modes, measurements inactive...\n"RST);
		} else {
		    returnToBuffer(pString);
		}
	} else if (pString[0] == SATELLITESTATUS) {
		printf("ConsoleDisplay: Showing Satellite Status...\n");
        state = STATE_SAT;
	} else if (pString[0] == ANNUNCIATION) {
		printf("ConsoleDisplay: Showing Annunciation Mode...\n");
        state = STATE_ANN;
	} else {
		returnToBuffer(pString);	
	}
	sendToPrint(pString[0], consoleStruct);
}

void returnToBuffer(char *pString) {
	if(checkAll(pString[0])) {
		int i = 0;
		for (i = strlen(pString); i >= 0; i--) {
			ungetc(pString[i], stdin);
		}
	}		
}

void sendToPrint(char mode, void *consoleStruct) {
    // 1.1 Assign the data of consoleStruct into local variables	
    consoleData *cData = (consoleData*)consoleStruct;	
    bool *fuelLow = cData->fuelLowPtr;
    bool *batteryLow = cData->batteryLowPtr;
    bool *solarPanelState = cData->solarPanelStatePtr;
    unsigned int *batteryLvl = cData->batteryLvlPtr;
    unsigned short *fuelLvl = cData->fuelLvlPtr;
    unsigned short *pConsume = cData->pConsumePtr;
    unsigned short *pGenerate = cData->pGeneratePtr;
    unsigned int *distance = cData->distancePtr;	
    unsigned int *batteryTmp1 = cData->batteryTmp1;
    unsigned int *batteryTmp2 = cData->batteryTmp2;
	unsigned int *pirateDistance = cData->pirateDistancePtr;	
	
    // 1.2 Define necessary string storage	
    char *solarPanelString = (*solarPanelState) ? "Deployed":"Retracted";
    char *fuelString = (*fuelLow)?         RED"YES" RST:GRN"NO"RST;
    char *battString = (*batteryLow)?      RED"YES" RST:GRN"NO"RST;
    char *tempString = (warningBattTemp)?  RED"HOT!"RST:GRN"OK"RST;	
	char output[MAX];	
	if (state == STATE_SAT) {

	sprintf(output, WHT_BG BLK BOLD"Satellite Status\n"RST
			  WHT_BG BLK"Solar Panels:      %s     \n"
					"Battery Level:     %u     \n"
					"Fuel Level:        %hu    \n"
					"Power Consumption: %hu    \n"
					"Power Generation:  %hu    \n"
					"Vehicle Distance:  %d     \n"
					"Battery Temp 1:    %d     \n"
					"Battery Temp 2:    %d     \n"
					"Pirate Proximity:  %hu    \n",
					 solarPanelString, *batteryLvl, *fuelLvl, *pConsume, *pGenerate,
					 *distance, *batteryTmp1, *batteryTmp2, *pirateDistance);
	} else if (state == STATE_ANN) {
		sprintf(output, WHT_BG BLK BOLD"Annunciaton\n"RST
				  WHT_BG BLK"Battery Low:         %3s"WHT_BG"    \n"
				  WHT_BG BLK"Fuel Low:            %3s"WHT_BG"    \n"
				  WHT_BG BLK"Battery Temperature: %4s"WHT_BG"    \n",
						 battString, fuelString, tempString);
	} else if (state == ONLY_ANN) {	
		sprintf(output, WHT_BG BOLD BLK"Annunciaton\n"RST
				  WHT_BG BLK"Battery Low:         %3s"WHT_BG"    \n"
				  WHT_BG BLK"Fuel Low:            %3s"WHT_BG"    \n"
				  WHT_BG BLK"Battery Temperature: %4s"WHT_BG"    \n"
				  WHT_BG MAG"NOTE: Measurement tasks are inactive.\n"
						 "Start measurements to show satellite status\n"RST,
						 battString, fuelString, tempString);	
	} else {
		sprintf(output, WHT_BG BLK"Press %c for Satellite Status\n"
                        "Press %c for Annunciation Mode\n", 
						SATELLITESTATUS, ANNUNCIATION);				
	}
	terminalComs(mode, output);
	
}


