/*
 * keyboardConsole.c - This file contains
 * the implementation of keyboardConsole
 */

#include <stdio.h>
#include <stdbool.h>
#include "dataStructs.h"
#include "TCB.h"
#define MAX 300

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

    // Even more fun, Abdul - Love, Rad
}
