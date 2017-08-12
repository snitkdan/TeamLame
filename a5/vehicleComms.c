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
#define CMD_SIZE 20

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
	
    //printf("\033[20;20H");		
    //char c = getchar();
	char pString[CMD_SIZE];
    pString[0] = '\0';	
	if(fgets(pString, CMD_SIZE, stdin) != NULL) {
       // remove newline
       pString[strcspn(pString, "\n")] = 0;
	}
    //char c = 'B';	
    if (!satVehicleCmd(pString[0])) {
        //if (consoleModeCmd(pString[0]) || motorSpeedCmd(pString[0]) || warningCmd(pString[0])) {
		if (checkAll(pString[0])) {
			int i = 0;
			for (i = strlen(pString); i >= 0; i--) {
			   ungetc(pString[i], stdin);
			}		
		}
        *command = '\0';
		*response = '\0';
        return;
		
    } else {
        *command = pString[0];
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
	 snprintf(responseBuf, 10, "A %s", buf);
     write(fd0, responseBuf, sizeof(responseBuf));
     return;
}




