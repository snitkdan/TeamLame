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
#include "scheduler.h"
#define MAX 300
#define MAX_BUF 10
#define CMD_SIZE 20

typedef enum {wrt, rd} rw;

char buf[MAX_BUF];    
extern TaskQueue queue;
extern TCB imageCaptureTCB;


void satelliteEnd(int pipeCom, rw coms, char *cmd);
void vehicleEnd(int pipeCom);

void vehicleComms(void *vehicleStruct) {
    int pipeCom;
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
    int *processImage = vData->processImagePtr;    
	if (*request != '\0') {
        if (*request == 'T') {
			*response = 'K';
	        printf("RESPONSE = %s\n", response);
			
		} else if (*request == 'D'){
		    *response = 'C';
	        printf("RESPONSE = %s\n", response);
			
		} else {
			printf("Invalid Request in vehicle coms\n");
		}
		return;
	}
	
	char pString[CMD_SIZE];
    pString[0] = '\0';	
	if(fgets(pString, CMD_SIZE, stdin) != NULL) {
       // remove newline
       pString[strcspn(pString, "\n")] = 0;
	}
	bool isValidCommand = satVehicleCmd(pString[0]);
    if (!isValidCommand) {
		if (checkAll(pString[0])) {
			int i = 0;
			for (i = strlen(pString); i >= 0; i--) {
			   ungetc(pString[i], stdin);
			}
		}
        *command = '\0';
		*response = '\0';
    } else {
        *command = pString[0];
    }
	if (*command == START_IMAGE) {
	    if (!ContainsTCB(queue, &imageCaptureTCB)) {
			AppendTCB(queue, &imageCaptureTCB);
		}
	} else if (*command == SEND_IMAGE) {
		// get buffer pointer and print 
		int i;
		for (i = 0; i < 16; i++) {
			printf("processImage[%d] = %d\n", i, processImage[i]); 
		}
	} else {
	    if (ContainsTCB(queue, &imageCaptureTCB)) {
			RemoveTCB(queue, &imageCaptureTCB);
		}
	}
	
	if (isValidCommand) {
		/* create the FIFO (named pipe) */
		char * myfifo0 = "/tmp/myfifo0";	
		mkfifo(myfifo0, 0666);

		/* open the FIFO */	
		pipeCom = open(myfifo0, O_RDWR);
		char buf[MAX_BUF];
		write(pipeCom, command, sizeof(command));
		vehicleEnd(pipeCom);	         //  read then write to the fifo
		read(pipeCom, buf, MAX_BUF);
		response = &buf[0];
		
		printf("RESPONSE = %s\n", response);
		close(pipeCom);

		/* remove the FIFO */
		unlink(myfifo0);
		return;	
	}
}

void vehicleEnd(int pipeCom) {
     char buf[MAX_BUF];
     read(pipeCom, buf, MAX_BUF);
     printf("VEHICLE: Received command: %s\n", buf);
     char responseBuf[10];
	 snprintf(responseBuf, 10, "A %s", buf);
     write(pipeCom, responseBuf, sizeof(responseBuf));
     return;
}




