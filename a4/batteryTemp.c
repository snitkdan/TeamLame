/*
 * batteryTemp.c - This file contains
 * the implementation of batteryTemp
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

void batteryTemp(void *temperatureStruct) {
	// Only run this function every major cycle
	/*
	static unsigned long start = 0;
	if((GLOBALCOUNTER - start) % MAJOR_CYCLE != 0) {
      return;
	}
    start = GLOBALCOUNTER;
	*/
    tempData *tData = (tempData*)temperatureStruct;
    bool *batteryOverTemp = tData->batteryOverTempPtr;
    
    #ifdef THIS_IS_PSUEDOCODE
	int sampleBuffer1[16];
	int sampleBuffer2[16];
	// Make buffers circular with fifo?
	battTemp1 = measure from sensor1;
	battTemp2 = measure from sensor2;
	temp1 = 32 * battTemp1 + 33;
	temp2 = 32 *battTemp2 + 33;
	if (temp1 > 20 * previousTemp1 /100 || temp2 > 20 * previousTemp2/100) {
				*batteryOverTemp = true;
	}  else {
				*batteryOverTemp = false;
	}
    #endif	
    return;
}

