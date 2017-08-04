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
#include "dataStructs.h"
#include "powerSubsystem.h"
#include "adc_utils.h"

#define ACH1 "AIN1"
#define ACH2 "AIN2"
#define HNUM1 16
#define HNUM2 16

extern unsigned int batteryTempBuff1[BUF_SIZE];
extern unsigned int batteryTempBuff2[BUF_SIZE];

void batteryTemp(void *temperatureStruct) {
	// Only run this function every major cycle
	/*
	static unsigned long start = 0;
	if((GLOBALCOUNTER - start) % MAJOR_CYCLE != 0) {
      return;
	}
    start = GLOBALCOUNTER;
	*/
	// Make buffers circular with fifo?

	// 0. Recast the data
	tempData *temData = (tempData *)temperatureStruct;
	bool *batteryOverTempPtr = temData->batteryOverTempPtr;
	unsigned int *batteryTmp1 = temData->batteryTmp1;
	unsigned int *batteryTmp2 = temData->batteryTmp2;

	// 1. Read from the 2 sensors
	#ifdef ADC
	int battTemp1 = readADC(ACH1, HNUM1);
	int battTemp2 = readADC(ACH2, HNUM2);
	#endif
	#ifdef debug
	static int currTmp = 1;
	int battTemp1 = batteryTempBuff1[currTmp];
	int battTemp2 = batteryTempBuff2[currTmp];
	currTmp = (currTmp + 1) % 16;
	#endif
	// 2. Convert sensor readings to temperatures
	int temp1 = 32 * battTemp1 + 33;
	int temp2 = 32 * battTemp2 + 33;
	// 3. Check the overtemp
	if(*batteryTmp1 != 0) {
		double t1_diff = 1.20 * *batteryTmp1;
		double t2_diff = 1.20 * *batteryTmp2;
		*batteryOverTempPtr = (temp1 > t1_diff || temp2 > t2_diff) ? true : false;
	}
	*batteryTmp1 = temp1;
	*batteryTmp2 = temp2;
	batteryTempBuff1[currTmp] = temp1;
	batteryTempBuff2[currTmp] = temp2;
	printf("batteryTmp1: %d, batteryTmp2: %d, batteryOverTmp %d\n",
					*batteryTmp1, *batteryTmp2, *batteryOverTempPtr);
  return;
}
