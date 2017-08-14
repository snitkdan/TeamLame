/*
 * pirateManagement.c - This file contains
 * the implementation of pirateManagement
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "TCB.h"
#include "dataStructs.h"


void pirateManagement(void *pmStruct) {
    // 1. Assign the data of pData into local variables
    mPirateData *pData = (mPirateData*)pmStruct;
    bool *pirateDetected = pData->pirateDetectedPtr;
	unsigned int *pirateDistance = pData->pirateDistancePtr;
    // Note: Shared variables are not added. Placeholders are added.
	//printf("inside pirate management\n");
    if (*pirateDistance <= 30) {
		if (*pirateDistance <= 5) {
			printf("PHOTONS BITCH\n");
		} else {
			printf("PHASORS BITCH\n");
		}
	}
	
}

