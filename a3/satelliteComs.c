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
#include <errno.h>
#include <unistd.h>
#include <string.h>
#define _XOPEN_SOURCE_EXTENDED 1
#include <stdlib.h> // for exit

#include "TCB.h"
#include "dataStructs.h"
#include "satelliteComs.h"
#include "nonBlockingKeys.h"
#include "satelliteVehicle.h"

#define MAX 65536 // upper bound for 16 bit

#define DEBUG
#define MAJOR
int fd_Vehicle = 0;
int fd_Earth = 0;






void satelliteComs(void *satStruct) {
	// Only runs this function every global cycle
	#ifdef MAJOR
	static unsigned long start = 0;
	if((GLOBALCOUNTER - start) % MAJOR_CYCLE != 0) {
      return;
	}
    start = GLOBALCOUNTER;
	#endif
	
    // 1. Assign the data of sData into local variables
    satData *sData = (satData*)satStruct;
    bool *fuelLow = sData->fuelLowPtr;
    bool *batteryLow = sData->batteryLowPtr;
    bool *solarPanelState = sData->solarPanelStatePtr;
    unsigned short *batteryLvl = sData->batteryLvlPtr;
    unsigned short *fuelLvl = sData->fuelLvlPtr;
    unsigned short *pConsume = sData->pConsumePtr;
    unsigned short *pGenerate = sData->pGeneratePtr;
    unsigned int *thrusterCommand = sData->thrusterCommandPtr;

    // 2. Retrieve random number, mask and assign thrusterCommand to it
    *thrusterCommand = randomInteger(0, MAX) % MAX;
    maskBit(thrusterCommand);
	
    char *solarPanelString = (*solarPanelState) ? "Deployed":"Retracted";
    char *fuelString = (*fuelLow)? "YES":"NO";
    char *battString = (*batteryLow)? "YES":"NO";
    char output[MAX];
    
    char *vehicleFifo = "/tmp/myfifo0";
    char *earthFifo = "/tmp/myfifo1";
	
	
	char buf[MAX_BUF];
    /* create the FIFO (named pipe) */
    mkfifo(vehicleFifo, 0666);
    mkfifo(earthFifo, 0666);
	
	fd_Vehicle = open(vehicleFifo, O_RDWR);
	
	// set pipe's read end to non blocking
	fcntl(fd_Vehicle, F_SETFL, O_NONBLOCK);
	read (fd_Vehicle, buf, MAX_BUF);
	printf("SATELLITECOMS: %s\n", buf);
	
	
	char c;
	int i;
	nonblock(NB_ENABLE);
	i=kbhit(0);

    if (i!=0)
    {
        c=fgetc(stdin);
    }
	//if (c == 'A' || c == 'B' ) 
		
	//	write(fd_Vehicle, &c, 10);
	// FIGURE OUT HOW TO PASS IN THE CHAR WITH A NULL BYTE

	if (c == 'A') 
		write(fd_Vehicle, "A", 10);
	else if (c == 'B')
		write(fd_Vehicle, "B", 10);
	
	//close(fd_Vehicle);

	/* remove the FIFO */
	//unlink(myfifo);
	
    // 3. Store print statements for satellite status and annunciation into output
    snprintf(output, MAX,
		   "EARTH: -----------\n"	
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

    // 4. Pass in the output to terminalComs, which will
    //    display the information on the earth terminal 
    terminalComs(output);
	
	
	
}

void maskBit(unsigned int *thrusterCommand) {
    // 0. Define a mask 1111111111110011
    uint16_t MASK = 0xFFF3;

    // 1. Mask the bit 2 and 3 to 0
    *thrusterCommand &= MASK;
}

/*
 * Filename: TerminalComs.c (adapted)
 * Author: James K. Peckol
 * Contact: jkp@uw.washington.edu
 */

// http://man7.org/linux/man-pages/man2/open.2.html


//#define MAX 1000
#define TEMP 
FILE *fp = NULL; // declare file here

int terminalComs(char *output) {
	int fdm = open("/dev/pts/1", O_RDWR);




    dprintf(fdm, "How can we send input here???\n");

}

