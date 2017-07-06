#include <stdio.h>
#include "dataStructs.h"

// Refer to p.14
void consoleDisplay(void *consoleStruct) {
        consoleData *cData = (consoleData*)consoleStruct;
	unsigned short *pGeneratePtr = pData->pGeneratePtr;
	unsigned short pGenerateData = *pGeneratePtr;
	printf("From consoleDisplay: %hu\n", pGenerateData);
	--(*pGeneratePtr);
}
