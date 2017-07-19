/*
 * vehicleComms.c - This file contains
 * the implementation of vehicleComms
 */

#include <stdio.h>
#include <stdbool.h>
#include "dataStructs.h"
#include "TCB.h"
#define MAX 300

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
