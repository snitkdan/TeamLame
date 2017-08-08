/*
 * pirateDetection.c - This file contains
 * the implementation of satelliteComs
 * (see satelliteComs.h for specification)
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

void pirateDetection(void *pdStruct) {
    // 1. Assign the data of pData into local variables
    dPirateData *pData = (dPirateData*)pdStruct;
    bool *fuelLow = pData->fuelLowPtr;

}

