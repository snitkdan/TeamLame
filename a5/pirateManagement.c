/*
 * pirateManagement.c - This file contains
 * the implementation of pirateManagement
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "TCB.h"
#include "dataStructs.h"
#include "nonBlockingKeys.h"
#define CMD_SIZE 20


void pirateManagement(void *pmStruct) {
    // 1. Assign the data of pData into local variables
    mPirateData *pData = (mPirateData*)pmStruct;
    bool *pirateDetected = pData->pirateDetectedPtr;
	unsigned int *pirateDistance = pData->pirateDistancePtr;
    // Note: Shared variables are not added. Placeholders are added.
	//printf("inside pirate management\n");
	
    if (*pirateDistance <= 30) {
		char pString[CMD_SIZE];
		pString[0] = '\0';	
		if(fgets(pString, CMD_SIZE, stdin) != NULL) {
		   // remove newline
		   pString[strcspn(pString, "\n")] = 0;
		}
		
		if(checkAll(pString[0]) && !piratesCmd(pString[0])) {
			int i = 0;
			for (i = strlen(pString); i >= 0; i--) {
			   ungetc(pString[i], stdin);
			}
			return;
		}
		if (*pirateDistance <= 5 && pString[0] == 'o') {
			printf("PHOTONS BITCH\n");		    
		} else if (*pirateDistance > 5 && pString[0] == 'p') {
			printf("PHASORS BITCH\n");
		}
	}
	
}

