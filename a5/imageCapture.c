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
//#define HNUM 15

#define BUF_SIZE 16
//#define DEBUG
extern signed int presentationBuffer[16];

void imageCapture(void *imageStruct) {
	// Only run this function every major cycle
	/*static unsigned long start = 0;
	if((GLOBALCOUNTER - start) % MAJOR_CYCLE != 0) {
      return;
	}
    start = GLOBALCOUNTER;*/
    imageData *iData = (imageData*)imageStruct;
    int *processImage = iData->processImagePtr;
	signed int captureBuff[256];
	signed int realBuff[256];	
	double voltReading;
	double scaledReading;
	static int currIndex = 0;
	
    #ifdef DEBUG
    presentationBuffer[currIndex] = 200 + currIndex;
    #endif
    #ifndef DEBUG
	int max = 0;
	int i;
	int j;
	printf("Running Image Capture...\n");
	for (j = 0; j < 16; j++) {
		for (i = 0; i < 256; i++) {
			voltReading = readADC(ACH, HNUM);
			captureBuff[i] = voltReading;
			//printf("VoltReading: %f Scaledreading: %d \n", voltReading, realBuff[currIndex]);
			usleep(100); // sampling freq of 10000 Hz
		}
		
		for (i = 0; i < 256; i++) {
			max = (captureBuff[i] > max) ? captureBuff[i] : max;
		}
		
		for ( i = 0; i < 256; i++) {
			realBuff[i] = (((captureBuff[i] - (max/2)) * 63) / 1023);
		}
		
		// Brent's FFT
		signed int imgBuff[256] = {0};	
		int m_index = optfft(realBuff, imgBuff);
		// Bin Wang's Algorithm
		double fs = 10000;
		double N = 256;
		double f;
		f = fs * m_index / N;
		presentationBuffer[currIndex] = f;
		#endif
		
		//printf("presentationBuffer[%d] = %d processImage = %d\n", currIndex, presentationBuffer[currIndex], *processImage);
		currIndex = (currIndex + 1) % 16;
	}
    	

	// send W back
    processImage = presentationBuffer;	
	printf("W\n");
	
    return;
}

