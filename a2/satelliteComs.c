#include <stdio.h>
#include "dataStructs.h"

// Refer to p.13
void satelliteComs(void *satStruct) {
    satData *pData = (satData*)satStruct;
    unsigned short *pGeneratePtr = pData->pGeneratePtr;
    unsigned short pGenerateData = *pGeneratePtr;
    printf("From satCom: %hu\n", pGenerateData);
    ++(*pGeneratePtr);
}
