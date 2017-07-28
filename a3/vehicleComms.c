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
#include "nonBlockingKeys.h"
#define MAX 300
#define MAX_BUF 300
#define DEBUG
typedef enum {wrt, rd} rw;

int fd0;    
char buf[MAX_BUF];    

void satelliteEnd(int fd0, rw coms, char cmd);
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
    //*command = 'B';
    char c = getchar();
    if (!satVehicleCmd(c)) {
        if (consoleModeCmd(c) || motorSpeedCmd(c)) ungetc(c, stdin);
        *command = '\0';
        return;
    } else {
        *command = c;
    }
    char * myfifo0 = "/tmp/myfifo0";
	    
    /* create the FIFO (named pipe) */
    mkfifo(myfifo0, 0666);

    /* open the FIFO */
	
    fd0 = open(myfifo0, O_RDWR);
    satelliteEnd(fd0, wrt, *command);     //  write to fifo
    vehicleEnd(fd0);		         //  read then write to the fifo
    satelliteEnd(fd0, rd, *response);     //  read fromn the fifo	
    close(fd0);

    printf("VEHICLE COMS: %c\n", *response);
    /* remove the FIFO */
    unlink(myfifo0);
    return;
}

void satelliteEnd(int fd0, rw coms, char cmd) {
     char buf[MAX_BUF];
     int size = 0;

     switch(coms) {
         case wrt:
         {
             printf("FUNCT0: send cmd %c\n", cmd);
             char toVehicle[2];
             toVehicle[0] = cmd;
             toVehicle[1] = '\0';
             size = sizeof(toVehicle);
             write(fd0, toVehicle, size);
             break;
         }
         case rd:
         {
             printf("FUNCT0: receiving response\n");
             size = sizeof("Roger That\n");
             read(fd0, buf, MAX_BUF);
             printf("Received: %s\n", buf);
             cmd = 'A';
             break;
         }
     }
}

void vehicleEnd(int fd0) {
     char buf[MAX_BUF];
     read(fd0, buf, MAX_BUF);
     printf("FUNCT1: Received command: %s\n", buf);
     char responseBuf[4]; 
     sprintf(responseBuf, "A %s", buf);
     write(fd0, responseBuf, sizeof(responseBuf));
     return;
}




