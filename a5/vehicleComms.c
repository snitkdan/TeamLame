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

// vars for the pipe
typedef enum {wrt, rd} rw;
char buf[MAX_BUF];
// to be able to insert imageCapture into queue on demands
extern TaskQueue queue;
extern TCB imageCaptureTCB;
bool getFrequencies; // tells satelliteComs whether an "I" was received

// function prototypes
void satelliteEnd(int pipeCom, rw coms, char *cmd);
void vehicleEnd(int pipeCom);

void vehicleComms(void *vehicleStruct) {
    int pipeCom;  
    // 1.1 Assign the data of vehicleStruct into local variables
    vehicleData *vData = (vehicleData*)vehicleStruct;
    char *command = vData->commandPtr;
    char *response = vData->responsePtr;
    char *request = vData->requestPtr;
    int *processImage = vData->processImagePtr; 

    // 2. From Transport Distance: 	
	if (*request != '\0') {
        if (*request == 'T') {
			*response = 'K';
	        printf("RESPONSE "CYN"= %s\n"RST, response);
			
		} else if (*request == 'D'){
		    *response = 'C';
	        printf("RESPONSE = "CYN"%s\n"RST, response);
		} else {
			printf("Invalid Request in vehicle coms\n");
		}
		return;
	}
	
	// 3. Reads user input for a vehicle command 
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
	
	// 4. Activates image capture routine
	if (*command == START_IMAGE) {
	    if (!ContainsTCB(queue, &imageCaptureTCB)) {
			AppendTCB(queue, &imageCaptureTCB);
		}
		getFrequencies = false;
	} else if (*command == SEND_IMAGE) {
		// get buffer pointer and print 
		printf("Image Status: "GRN"P\n"RST);
		getFrequencies = true; // tells satelliteComs to acquire images


	} else {
	    if (ContainsTCB(queue, &imageCaptureTCB)) {
			RemoveTCB(queue, &imageCaptureTCB);
		}
	}
	
	// 5. Checks if there is a vehicle command, creates pipe communication
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
		
		printf(MAG"PIPE "RST"- RESPONSE = %s\n", response);
		close(pipeCom);

		/* remove the FIFO */
		unlink(myfifo0);
		return;	
	}
}

void vehicleEnd(int pipeCom) {
     char buf[MAX_BUF];
     read(pipeCom, buf, MAX_BUF);
     printf(MAG"PIPE "RST"- VEHICLE: Received command: %s\n", buf);
     char responseBuf[10];
	 snprintf(responseBuf, 10, "A %s", buf);
     write(pipeCom, responseBuf, sizeof(responseBuf));
     return;
}




