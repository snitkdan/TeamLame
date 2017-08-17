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
#include <math.h>
#include "dataStructs.h"
#include "TCB.h"
#include "powerSubsystem.h"
#include "adc_utils.h"
#include "optfft.h"
#include "fft.h"
#define ACH "AIN3"
#define BUF_SIZE 16

extern signed int presentationBuffer[16];

void imageCapture(void *imageStruct) {
    imageData *iData = (imageData*)imageStruct;
    int *processImage = iData->processImagePtr;
	COMPLEX samples[256];
	COMPLEX w[127];
	signed int realBuff[256];	
	int voltReading;
	double scaledReading;
	static int currIndex = 0;
	
    #ifdef DEBUG
    presentationBuffer[currIndex] = 200 + currIndex;
    #endif
    #ifndef DEBUG
	int i;
	int j;
	printf(BLU_BG BOLD"Running Image Capture...\n"RST);
	fputs("\e[?25l", stdout); // hides cursor	
    char adc_val_path[50];
    //sprintf(adc_val_path, "%s/%s%i/%s", "/sys/devices", "ocp.3/helper.", 15, "AIN3");
    FILE *adc_val = fopen("/sys/devices/ocp.3/helper.15/AIN3", "r");
	
	//float max = 0;
	for (j = 0; j < 16; j++) {
		for (i = 0; i < 256; i++) {
			//voltReading = readADC(ACH, HNUM);
            fseek(adc_val,0,SEEK_SET);			
			fscanf(adc_val, "%d", &voltReading);
			fflush(adc_val);			
			samples[i].real = voltReading - 847;
			samples[i].imag = 0;
		}
        FILE *fp = fopen("measurement.txt", "w+"); 
		for (i = 0; i < 256; i++) {
			fprintf(fp, "%f\n", samples[i].real);
		}
		fclose(fp);
    		
		fft(samples, w, 8);		
		int m_index = 0;
		float mag = sqrt(samples[0].real * samples[0].real + samples[0].imag * samples[0].imag);
		float curr_mag = 0;
		for (i = 0; i < 128; i++) {
            curr_mag = sqrt(samples[i].real * samples[i].real + samples[i].imag * samples[i].imag);
			if (curr_mag > mag) {
				m_index = i;
				mag = curr_mag;
			}
		}

		// Bin Wang's Algorithm
		double fs = 950;
		double N = 256;
		double f;
		f = fs * m_index / N;
		presentationBuffer[currIndex] = f;
		
        printf("\rFrequencies Capture = %d / 16", j + 1); fflush(stdout);
		
		#endif
		
		//printf("presentationBuffer[%d] = %d processImage = %d\n", currIndex, presentationBuffer[currIndex], *processImage);
		currIndex = (currIndex + 1) % 16;
	}
    fclose(adc_val);
	

	// send W back
    processImage = presentationBuffer;	
	printf("\nW\n");
    fputs("\e[?25h", stdout);	
	
    return;
}