#include <stdio.h>
#include "dataStructs.h"

// Refer to p.12
void powerSubsystem(void *powerStruct) {
	powerData *pData = (powerData*)powerStruct;
	unsigned short *pGeneratePtr = pData->pGeneratePtr;
	unsigned short pGenerateData = *pGeneratePtr;
	printf("From powerSubsystem: %hu\n", pGenerateData);
	++(*pGeneratePtr);
}
