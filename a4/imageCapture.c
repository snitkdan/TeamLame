/*
 * imageCapture.c - This file contains
 * the implementation of imageCapture
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
#include "powerSubsystem.h"
#include "adc_utils.h"
#include "optfft.h"
#define ACH "AIN3"
#define HNUM 15

#define BUF_SIZE 16

void imageCapture(void *imageStruct) {
	// Only run this function every major cycle
	/*
	static unsigned long start = 0;
	if((GLOBALCOUNTER - start) % MAJOR_CYCLE != 0) {
      return;
	}
    start = GLOBALCOUNTER;
	*/
	signed int presentationBuffer[16];
	
	signed int realBuff[256];
	
	double voltReading;
	double scaledReading;
	int i;
	int j;
	
	for (i = 0; i < 16; i++) {
		for (j = 0; j < 256; j++) {
		   voltReading = readADC(ACH, HNUM);
		   scaledReading = (voltReading - 900) / 29.0322518;
		   realBuff[j] = scaledReading;
		   //printf("VoltReading: %f Scaledreading: %d \n", voltReading, realBuff[i]);
		   usleep(1000); // sampling freq of 1000 Hz
		}
		// Brent's FFT
    	signed int imgBuff[256] = {0};	
		int m_index = optfft(realBuff, imgBuff);
		printf("m_index %d\n", m_index);
		// Bin Wang's Algorithm
		double fs = 1000;
		double N = 256;
		double f;
		f = fs * m_index / N;
		printf("f = %f\n", f);
		presentationBuffer[i] = f;
	}
	
	for (i = 0; i < 16; i++) {
		printf("presentationbuffer = %d\n", presentationBuffer[i]);
	}
	
	printf("-----------------------\n\n\n\n");

    return;
}

