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
int fd = 0;

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
    char *myfifo = "/tmp/myfifo0";
	
    char buf[MAX_BUF];
    /* create the FIFO (named pipe) */
    mkfifo(myfifo, 0666);
    fd = open(myfifo, O_RDWR);
	
    // set pipe's read end to non blocking
    fcntl(fd, F_SETFL, O_NONBLOCK);
    if (read (fd, buf, MAX_BUF) > 0) {
        printf("Response from Vehicle: %s\n", buf);
    }
	int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
	
	char c = getchar();
	printf("SATCOMS: c = %d, %c\n", c, c);
	usleep(1000000);
	if (c != 255) {
		if (c == 'V') {
			write(fd, &c, 5);
		}
		else {
			if(c == 'z' || c == 'x') ungetc(c, stdin);
		}
	}
	#ifdef FKTHIS
    char c;
    int i;
    i = kbhit(); 
    if (i != 0) {
        c = fgetc(stdin);
		if (c == 'V') {
			printf("inside satComs V\n");
			write(fd, "V", 10);
		} else if (c == 'B') {
			printf("inside satComs B\n");
			write(fd, "B", 10);
		}		
    }
	#endif
}

void maskBit(unsigned int *thrusterCommand) {
    // 0. Define a mask 1111111111110011
    uint16_t MASK = 0xFFF3;

    // 1. Mask the bit 2 and 3 to 0
    *thrusterCommand &= MASK;
}
