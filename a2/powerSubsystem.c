#include <stdio.h>
#include "dataStructs.h"
#include "Bool.h"

// Refer to p.12
// prototype
powerconsumption shit

void powerSubsystem(void *powerStruct) 
{
// TEST CASE
/*	powerData *pData = (powerData*)powerStruct;
	unsigned short *pGeneratePtr = pData->pGeneratePtr;
	unsigned short pGenerateData = *pGeneratePtr;
	printf("From powerSubsystem: %hu\n", pGenerateData);
	++(*pGeneratePtr);
*/
    
    powerConsumption();
}

void powerConsumption() 
{
    static bool flag = false; 
    static int calls = 0;
    if ((pConsume <= 10 && !flag) || (pConsume < 5 && flag)) {
                  
        if (calls == 0) { // function has been called even number of times
             pConsume = pConsume + 2;
        
         } else {  // call is odd
             pConsume = pConsume - 1;
         } 

    } else if (pConsume > 10){
        flag = true; 
        if (calls == 0) { // function has been called even number of times
            pConsume = pConsume - 2;
        
        } else {  // call is odd 
             pConsume = pConsume + 1;

        } 
  
    }
    calls++;
}

normal if even add 2 
       else odd minus 1

reverse if even minus 2
         else odd add 1

