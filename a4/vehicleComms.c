/*
 * vehicleComms.c - This file contains
 * the implementation of vehicleComms
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "dataStructs.h"
#include "TCB.h"
#include "satelliteVehicle.h"
#include "nonBlockingKeys.h"
#define MAX 300
#define MAX_BUF 10
#define DEBUG
typedef enum {wrt, rd} rw;

int fd0;    
char buf[MAX_BUF];    

void satelliteEnd(int fd0, rw coms, char *cmd);
void vehicleEnd(int fd0);

void vehicleComms(void *vehicleStruct) {
	// Only run this function every major cycle
	/*
	static unsigned long start = 0;
	if((GLOBALCOUNTER - start) % MAJOR_CYCLE != 0) {
      return;
	}
    start = GLOBALCOUNTER;
	*/
     //printf("INSIDE vehicleComs\n");	  
    // 1.1 Assign the data of vehicleStruct into local variables
    vehicleData *vData = (vehicleData*)vehicleStruct;
    char *command = vData->commandPtr;
    char *response = vData->responsePtr;
    char *request = vData->requestPtr;
    
	if (*request != '\0') {
        char buf[MAX_BUF];
		
		char * myfifo0 = "/tmp/myfifo0";	
		mkfifo(myfifo0, 0666);

		/* open the FIFO */	
		fd0 = open(myfifo0, O_RDWR);		
	    write(fd0, request, sizeof(request));
        vehicleEnd(fd0);	         //  read then write to the fifo
	    read(fd0, buf, MAX_BUF);
	    response = &buf[0];		
	    close(fd0);

        /* remove the FIFO */
        unlink(myfifo0);
		return;
	}
	
    //printf("\033[20;20H");		
    char c = getchar();
    //char c = 'B';	
    if (!satVehicleCmd(c)) {
        if (consoleModeCmd(c) || motorSpeedCmd(c) || warningCmd(c)) ungetc(c, stdin);
        *command = '\0';
		*response = '\0';
        return;
    } else {
        *command = c;
    }
	

    /* create the FIFO (named pipe) */
    char * myfifo0 = "/tmp/myfifo0";	
    mkfifo(myfifo0, 0666);

    /* open the FIFO */	
    fd0 = open(myfifo0, O_RDWR);
    char buf[MAX_BUF];
    write(fd0, command, sizeof(command));
    vehicleEnd(fd0);	         //  read then write to the fifo
	read(fd0, buf, MAX_BUF);
	response = &buf[0];
	
	printf("RESPONSE = %s\n", response);
	close(fd0);

    /* remove the FIFO */
    unlink(myfifo0);
    return;
}

void vehicleEnd(int fd0) {
     char buf[MAX_BUF];
     read(fd0, buf, MAX_BUF);
     printf("VEHICLE: Received command: %s\n", buf);
     char responseBuf[10];	 
	 if (strchr(buf, REQUEST_LIFTOFF)) {
		snprintf(responseBuf, 10, "K %s", buf);
	 } else if (strchr(buf, REQUEST_DOCK)) {
		snprintf(responseBuf, 10, "C %s", buf);
	 } else {
		snprintf(responseBuf, 10, "A %s", buf);		 
		 
	 }
     write(fd0, responseBuf, sizeof(responseBuf));
     return;
}




