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

#define BUF_SIZE 16

#define ADC

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

	// 0. Recast the data
	tempData *temData = (tempData *)temperatureStruct;
	bool *batteryOverTempPtr = temData->batteryOverTempPtr;
	unsigned int *batteryTmp1 = temData->batteryTmp1;
	unsigned int *batteryTmp2 = temData->batteryTmp2;

	// 1. Read from the 2 sensors
	#ifdef ADC
	double battTemp1 = readADC(ACH1, HNUM1) / 1000;
	double battTemp2 = readADC(ACH2, HNUM2) / 1000;
	static int callNum = 0;
	static int currTmp = 0;
	#endif
	#ifdef debug

	static unsigned int tempBuff[] = {100, 100, 100, 100, 100, 100,
			100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100};
	unsigned int battTemp1 = tempBuff[currTmp];
	unsigned int battTemp2 = tempBuff[currTmp];
	battTemp1 *= (callNum % 3) ? 2 : (callNum % 2) ? 1.2 : 1.5;
	battTemp2 *= (callNum % 3) ? 1.3 : (callNum % 2) ? 1.2 : 1.1;
	#endif
	// 2. Convert sensor readings to temperatures
	unsigned int temp1 = 32 * battTemp1 + 33;
	unsigned int temp2 = 32 * battTemp2 + 33;
	// 3. Check the overtemp
	if(callNum != 0) {
		double t1_diff = 1.20 * *batteryTmp1;
		double t2_diff = 1.20 * *batteryTmp2;
		*batteryOverTempPtr = (temp1 > t1_diff || temp2 > t2_diff) ? true : false;
	}
	*batteryTmp1 = temp1;
	*batteryTmp2 = temp2;
	batteryTempBuff1[currTmp] = temp1;
	batteryTempBuff2[currTmp] = temp2;
	printf("callNum: %d, batteryTmp1: %u, batteryTmp2: %u, batteryOverTmp %d\n",
					callNum, *batteryTmp1, *batteryTmp2, *batteryOverTempPtr);
	callNum++;
	currTmp = (currTmp + 1) % 16;
  return;
}
