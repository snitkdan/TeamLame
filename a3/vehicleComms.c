/*
 * vehicleComms.c - This file contains
 * the implementation of vehicleComms
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>

#include <unistd.h>
#include <string.h>
#include "dataStructs.h"
#include "TCB.h"
#include "satelliteVehicle.h"
#define MAX 300

#define DEBUG
#define MAJOR

void vehicleComms(void *vehicleStruct) {
	#ifdef MAJOR
	// Only run this function every major cycle
	static unsigned long start = 0;
	if((GLOBALCOUNTER - start) % MAJOR_CYCLE != 0) {
      return;
	}
    start = GLOBALCOUNTER;
	#endif
	
    // 1.1 Assign the data of vehicleStruct into local variables
    vehicleData *vData = (vehicleData*)vehicleStruct;
    char *command = vData->commandPtr;
    char *response = vData->responsePtr;

	response = "A";
    /* open, read, and display the message from the FIFO */
	char buf[MAX_BUF];
	if(read(fd_Vehicle, command, MAX_BUF) != -1) {
		printf("Received: %c\n", *command);		
		fflush(stdout);
		write(fd_Vehicle, response, 10);
	}


	

}
