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
#define MAX_BUF 300
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
    
    //printf("\033[20;20H");		
    char c = getchar();
    //char c = 'B';	
    if (!satVehicleCmd(c)) {
        if (consoleModeCmd(c) || motorSpeedCmd(c)) ungetc(c, stdin);
        *command = '\0';
		*response = '\0';
        return;
    } else {
        *command = c;
    }
    char * myfifo0 = "/tmp/myfifo0";
	    
    /* create the FIFO (named pipe) */
    mkfifo(myfifo0, 0666);

    /* open the FIFO */
	
    fd0 = open(myfifo0, O_RDWR);
    satelliteEnd(fd0, wrt, command);     //  write to fifo
    vehicleEnd(fd0);	         //  read then write to the fifo
    satelliteEnd(fd0, rd, response);     //  read fromn the fifo	
	printf("RESPONSE = %s\n", response);
	close(fd0);

    //printf("VEHICLE COMS: %c\n", response);
    /* remove the FIFO */
    unlink(myfifo0);
    return;
}

void satelliteEnd(int fd0, rw coms, char *cmd) {
     char buf[MAX_BUF];
     switch(coms) {
         case wrt:
         {
             printf("SATELLITE: send cmd %s\n", cmd);
			 write(fd0, cmd, sizeof(cmd));
			 break;
         }
         case rd:
         {
             printf("SATELLITE: receiving response\n");
             read(fd0, buf, MAX_BUF);
             printf("SATELLITE: received: %s\n", buf);
             int i = 0;
			 while (buf[i] != '\0') {
				 cmd[i] = buf[i];
				 i++;
			 }
			 cmd[i] = '\0';
			 break;
         }
     }
}

void vehicleEnd(int fd0) {
     char buf[MAX_BUF];
     read(fd0, buf, MAX_BUF);
     printf("VEHICLE: Received command: %s\n", buf);
     char responseBuf[10];	 
	 if (strchr(buf, REQUEST_LIFTOFF)) {
		sprintf(responseBuf, "K %s%c", buf, '\0');
	 } else if (strchr(buf, REQUEST_DOCK)) {
		sprintf(responseBuf, "C %s%c", buf, '\0');
	 } else {
		sprintf(responseBuf, "A %s%c", buf, '\0');		 
		 
	 }
     write(fd0, responseBuf, sizeof(responseBuf));
     return;
}




