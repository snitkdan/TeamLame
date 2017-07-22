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


#include "TCB.h"
#include "dataStructs.h"
#include "satelliteComs.h"
#include "nonBlockingKeys.h"
#include "satelliteVehicle.h"

#define MAX 65536 // upper bound for 16 bit

#define DEBUG

typedef enum {wrt, rd} rw;

int fd0;    
char buf[MAX_BUF];

#ifdef DEBUG
void funct0(int fd0, rw coms);
void funct1(int fd0);
#endif
void satelliteComs(void *satStruct) {
	// Only runs this function every global cycle
	static unsigned long start = 0;
	if((GLOBALCOUNTER - start) % MAJOR_CYCLE != 0) {
      return;
	}
    start = GLOBALCOUNTER;
	
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
    
	#ifdef DEBUG
	char * myfifo0 = "/tmp/myfifo0";
	    
	/* create the FIFO (named pipe) */
    mkfifo(myfifo0, 0666);

    /* open the FIFO */
	
    fd0 = open(myfifo0, O_RDWR);
	
	funct0(fd0, wrt);		//  write to fifo
	funct1(fd0);			//  read then write to the fifo
	funct0(fd0, rd);		//  read fromn the fifo
	
    close(fd0);

    /* remove the FIFO */
    unlink(myfifo0);
	#endif
	
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
#ifdef DEBUG

/*
 *  Test function0
 */
 
 //  test write then read back
 
void funct0(int fd0, rw coms)
{	char buf[MAX_BUF];
	int size = 0;
	
	//  testing
	printf("in funct0\n");
	
	// check for write or read
	switch(coms)
	{
		case wrt: 
		{
			printf("send command\n");
			size = sizeof("Launch Rocket");
			write(fd0, "Launch Rocket", size);
			break;
		}
		case rd:
		{
			printf("receive response\n");
			size = sizeof("Roger That\n");
			read(fd0, buf, MAX_BUF);
			printf("Received: %s\n", buf);
			break;
		}
	}

	return;
}
#endif

void maskBit(unsigned int *thrusterCommand) {
    // 0. Define a mask 1111111111110011
    uint16_t MASK = 0xFFF3;

    // 1. Mask the bit 2 and 3 to 0
    *thrusterCommand &= MASK;
}
