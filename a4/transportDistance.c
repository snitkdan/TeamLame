/*
 * transportDistance.c - This file contains
 * the implementation of transportDistance
 */

#include <stdio.h>
#include <assert.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include "dataStructs.h"
#include "TCB.h"

// GPIO PIN NUMBERS
#define IN "66"
#define RESET "48"
#define BIT6 "67"
#define BIT5 "68"
#define BIT4 "44"
#define BIT3 "26"
#define BIT2 "46"
#define BIT1 "65"
#define BIT0 "61"

#define BUF_SIZE 8
#define PATH "/sys/class/gpio"
#define MICROSEC 1000000
#define DELAY 10000

FILE *gpioIN = NULL;
FILE *gpioReset = NULL;	
FILE *gpio0 = NULL;
FILE *gpio1 = NULL;
FILE *gpio2 = NULL;
FILE *gpio3 = NULL;
FILE *gpio4 = NULL;
FILE *gpio5 = NULL;
FILE *gpio6 = NULL;

extern unsigned int distanceBuff[BUF_SIZE];
extern bool fromTransport;

void initPins();

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
    unsigned int *distance = tData->distancePtr;
	
	// The Satellite Management and Control System must support an interface to a sensor that
    // detects a signal from an inbound transport vehicle. The frequency of the incoming signal
    // shall be proportional to the distance between the satellite and an inbound transport vehicle.
	static int firstTime = 0;
    //#define DEBUG	
    #ifndef DEBUG	
	if (firstTime == 0) {
        initPins();
	    firstTime++;
	}
	gpioIN = fopen("/sys/class/gpio/gpio"IN"/value", "w");
	gpioReset = fopen("/sys/class/gpio/gpio"RESET"/value", "w");	
	gpio0 = fopen("/sys/class/gpio/gpio"BIT0"/value", "r");
	gpio1 = fopen("/sys/class/gpio/gpio"BIT1"/value", "r");
	gpio2 = fopen("/sys/class/gpio/gpio"BIT2"/value", "r");
	gpio3 = fopen("/sys/class/gpio/gpio"BIT3"/value", "r");
	gpio4 = fopen("/sys/class/gpio/gpio"BIT4"/value", "r");
	gpio5 = fopen("/sys/class/gpio/gpio"BIT5"/value", "r");
	gpio6 = fopen("/sys/class/gpio/gpio"BIT6"/value", "r");	
	
	assert(gpioIN != NULL);
	assert(gpioReset != NULL);
	assert(gpio0 != NULL);
	assert(gpio1 != NULL);
	assert(gpio2 != NULL);
	assert(gpio3 != NULL);
	assert(gpio4 != NULL);
	assert(gpio5 != NULL);
	assert(gpio6 != NULL);

	//char bit0;
	unsigned int bit0, bit1, bit2, bit3, bit4, bit5, bit6;
	system("echo 1 > /sys/class/gpio/gpio69/value");
	//int ret = fprintf(gpioReset, "%d", 1); 
	//fflush(gpioReset);	
	sleep(1);
	system("echo 0 > /sys/class/gpio/gpio69/value");
	//ret = fprintf(gpioReset, "%d", 0); 
	//fflush(gpioReset);
	//fscanf(gpio0, "%d", &bit0);
	//printf("START BIT 0 : %d\n", bit0);	
	fprintf(gpioIN, "%d", 1); fflush(gpioIN);
	usleep(DELAY);
	fprintf(gpioIN, "%d", 0); fflush(gpioIN);
	
	if (gpio0) fscanf(gpio0, "%d", &bit0); fflush(gpio0);
	//fread(&bit0, sizeof(bit0), 1, gpio0); fflush(gpio0);
	if (gpio1) fscanf(gpio1, "%d", &bit1); fflush(gpio1);
	if (gpio2) fscanf(gpio2, "%d", &bit2); fflush(gpio2);
	if (gpio3) fscanf(gpio3, "%d", &bit3); fflush(gpio3);
	if (gpio4) fscanf(gpio4, "%d", &bit4); fflush(gpio4);
	if (gpio5) fscanf(gpio5, "%d", &bit5); fflush(gpio5);
	if (gpio6) fscanf(gpio6, "%d", &bit6); fflush(gpio6);
    #endif 	
	
	// --------------------------------------------
	// *
	#ifdef DEBUG
	// distance in meters
	if (firstTime == 0) {
		system("echo 1 > file0.txt");
		system("echo 0 > file1.txt");
		system("echo 0 > file2.txt");
		system("echo 0 > file3.txt");
		system("echo 0 > file4.txt");
		system("echo 0 > file5.txt");
		system("echo 0 > file6.txt");
		firstTime++;
	}
	
	FILE *fp0 = fopen("file0.txt", "r");
	FILE *fp1 = fopen("file1.txt", "r");
	FILE *fp2 = fopen("file2.txt", "r");
	FILE *fp3 = fopen("file3.txt", "r");
	FILE *fp4 = fopen("file4.txt", "r");
	FILE *fp5 = fopen("file5.txt", "r");
	FILE *fp6 = fopen("file6.txt", "r");
	
	char bit0;
	unsigned int bit1, bit2, bit3, bit4, bit5, bit6;
	
    fscanf(fp0, "%d", &bit0);
	fscanf(fp1, "%d", &bit1);
	fscanf(fp2, "%d", &bit2);
	fscanf(fp3, "%d", &bit3);
	fscanf(fp4, "%d", &bit4);
	fscanf(fp5, "%d", &bit5);
	fscanf(fp6, "%d", &bit6);
	#endif
	// ----------------------------------------------
	
	unsigned int gpioBinary = 0;
	gpioBinary = gpioBinary | (bit0);
	gpioBinary = gpioBinary | (bit1 << 1);
	gpioBinary = gpioBinary | (bit2 << 2);
	gpioBinary = gpioBinary | (bit3 << 3);
	gpioBinary = gpioBinary | (bit4 << 4);
	gpioBinary = gpioBinary | (bit5 << 5);
	gpioBinary = gpioBinary | (bit6 << 6);

	
	printf("%d ", bit6);
	printf("%d ", bit5);
	printf("%d ", bit4);
	printf("%d ", bit3);
	printf("%d ", bit2);
	printf("%d ", bit1);
	printf("%d\n", bit0);
	
	
	printf ("HardwareCounter:  %d\n\n\n", gpioBinary);
	usleep(1000000);
	double time = MICROSEC / DELAY;
	unsigned int frequency = gpioBinary * time;
	//printf("frequency = %d\n", frequency);
	
	// some distance equation?
	//double calcDistance = (frequency == 0)? 0:100 / frequency;
	double calcDistance = 1500;
	
	if (calcDistance > 2000) {
		calcDistance = 2000;
	} else if (calcDistance <= 100) {
		calcDistance = 100;
	}
	//printf("current distance = %d\n", calcDistance);

	fromTransport = true;
	if (calcDistance <= 100) {
		raise(SIGUSR1);
		
	}
	fromTransport = false;

	
	static int callNum = 0;
	static int currIndex = 0;
	if (callNum == 0) {
	    distanceBuff[currIndex] = calcDistance;	
        *distance = distanceBuff[currIndex];		
     	currIndex = (currIndex + 1) % 8;
	} else {
		double diff_high = 1.1 * *distance;
		double diff_low  = 0.9 * *distance;
        if (calcDistance > diff_high || calcDistance < diff_low) { // different by more than 10% of prev measurement
	        distanceBuff[currIndex] = calcDistance;	
            *distance = distanceBuff[currIndex];		
     	    currIndex = (currIndex + 1) % 8;			
		}
		
	}

	#ifdef DEBUG
	fclose(fp0);
	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	fclose(fp4);
	fclose(fp5);
	fclose(fp6);
	#endif
	
	#ifndef DEBUG
	fclose(gpio0);
	fclose(gpio1);
	fclose(gpio2);
	fclose(gpio3);
	fclose(gpio4);
	fclose(gpio5);
	fclose(gpio6);
	#endif
    return;
}

void initPins() {	
	
	system("echo "IN" > /sys/class/gpio/export");
	system("echo 69 > /sys/class/gpio/export");
	system("echo "BIT0" > /sys/class/gpio/export");
	system("echo "BIT1" > /sys/class/gpio/export");
	system("echo "BIT2" > /sys/class/gpio/export");
	system("echo "BIT3" > /sys/class/gpio/export");
	system("echo "BIT4" > /sys/class/gpio/export");
	system("echo "BIT5" > /sys/class/gpio/export");
	system("echo "BIT6" > /sys/class/gpio/export");
	
	system("echo out > /sys/class/gpio/gpio"IN"/direction");
	//system("echo falling > /sys/class/gpio/gpio"IN"/edge");
	
	system("echo out > /sys/class/gpio/gpio69/direction");
	system("echo in > /sys/class/gpio/gpio"BIT0"/direction");
	system("echo in > /sys/class/gpio/gpio"BIT1"/direction");
	system("echo in > /sys/class/gpio/gpio"BIT2"/direction");
	system("echo in > /sys/class/gpio/gpio"BIT3"/direction");
	system("echo in > /sys/class/gpio/gpio"BIT4"/direction");
	system("echo in > /sys/class/gpio/gpio"BIT5"/direction");
	system("echo in > /sys/class/gpio/gpio"BIT6"/direction");


}

