/*
 * keyboardConsole.c - This file contains
 * the implementation of keyboardConsole
 */

#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include "dataStructs.h"
#include "TCB.h"
#include "nonBlockingKeys.h"
#define MAX 300

extern bool endOfTravel;

void keyboardConsole(void *keyboardStruct) {
	// Only run this function every major cycle
	static unsigned long start = 0;
	if((GLOBALCOUNTER - start) % MAJOR_CYCLE != 0) {
      return;
	}
    start = GLOBALCOUNTER;
	
    // 1.1 Assign the data of consoleStruct into local variables
    keyboardData *kData = (keyboardData*)keyboardStruct;
    bool *motorInc = kData->motorIncPtr;
    bool *motorDec = kData->motorDecPtr;

   
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
    char c = getchar();
	//char c = 'i';
    if (!motorSpeedCmd(c)) {
        if (consoleModeCmd(c) || satVehicleCmd(c)) ungetc(c, stdin);
        *motorInc = false;
        *motorDec = false;
    } else {
		/*if (endOfTravel) {
			printf("Cannot overdrive solar panel motor...\n");
			return;
		}*/
		
        if (c == SPEEDINC) {
			printf("Increasing motor speed...\n");
            *motorInc = true;
            *motorDec = false;
        } else {
			printf("Decreasing motor speed...\n");			
            *motorDec = true;
            *motorInc = false;
        }

    }
    //printf("motor Inc = %d, motor Dec = %d\n", *motorInc, *motorDec);
    
}
