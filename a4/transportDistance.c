/*
 * transportDistance.c - This file contains
 * the implementation of transportDistance
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

void transportDistance(void *transportStruct) {
	// Only run this function every major cycle
	/*
	static unsigned long start = 0;
	if((GLOBALCOUNTER - start) % MAJOR_CYCLE != 0) {
      return;
	}
    start = GLOBALCOUNTER;
	*/
	transportData *tData = (transportData*)transportStruct;
    unsigned short *distance = tData->distancePtr;
	
	// unsigned int frequency variable?
	
	// The Satellite Management and Control System must support an interface to a sensor that
    // detects a signal from an inbound transport vehicle. The frequency of the incoming signal
    // shall be proportional to the distance between the satellite and an inbound transport vehicle. 
	
	// received interrupt when distance == 1000
	
	
	// distance in meters
	
	
    return;
}

