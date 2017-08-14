/*
 * pirateDetection.c - This file contains
 * the implementation of pirateDetection
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
#include "adc_utils.h"

#define ACH "AIN4"
#define HNUM 15

void pirateDetection(void *pdStruct) {
    // 1. Assign the data of pData into local variables
    dPirateData *pData = (dPirateData*)pdStruct;
    bool *pirateDetected = pData->pirateDetectedPtr;
	unsigned int *pirateDistance = pData->pirateDistancePtr;
	
	#ifdef BASICTEST
	static int count = 0;
	printf("count: %d\n", count);
	if (10 < count && count <= 15) {
		*pirateDetected = true;
	} else {
		*pirateDetected = false;
	}
	count++;
	#endif
	
    // Note: Shared variables are not added. Placeholders are added.
	
	unsigned int rawDistance = readADC(ACH, HNUM);
	*pirateDistance = rawDistance / 9; // Scale from raw measurement of 900 to 100   
	
	if (*pirateDistance <= 100) {
		*pirateDetected = true;		
	} else {
		*pirateDetected = false;
	}
	//printf("*pirateProximity = %d  pirateDetected = %d\n", *pirateDistance, *pirateDetected);

}

