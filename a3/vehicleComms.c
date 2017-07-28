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
    
    
     
}




