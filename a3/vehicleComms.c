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
#include <unistd.h>
#include "dataStructs.h"
#include "TCB.h"
#include "satelliteVehicle.h"
#define MAX 300

#define DEBUG

void vehicleComms(void *vehicleStruct) {
	// Only run this function every major cycle
	static unsigned long start = 0;
	if((GLOBALCOUNTER - start) % MAJOR_CYCLE != 0) {
      return;
	}
    start = GLOBALCOUNTER;
	
    // 1.1 Assign the data of consoleStruct into local variables
    vehicleData *vData = (vehicleData*)vehicleStruct;
    char *command = vData->commandPtr;
    char *response = vData->responsePtr;

}
#ifdef DEBUG
//  test read then write back
void funct1(int fd0)
{
	char buf[MAX_BUF];
	int size = sizeof("Roger That\n");
	
	printf("in funct1\n");
		
	/* read and display the message from the FIFO */
    read(fd0, buf, MAX_BUF);
	
    printf("Received command: %s\n", buf);
	
	/* write message to the FIFO */	
	printf("ack command\n");	
	write(fd0, "Roger That\n", size);
	
	return;
}
#endif