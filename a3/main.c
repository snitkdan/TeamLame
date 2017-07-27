#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include "TCB.h"
#include "dataStructs.h"
#include "warningAlarm.h"
#include "startup.h"
#include "scheduler.h"

extern unsigned long GLOBALCOUNTER;

void main(void) {
    // Extern for the queue
    extern TaskQueue queue;
    // Startup task
    Initialize();
    ActivateTimeBase();
    // Defines a TCB pointer
    TCB_Ptr aTCBPtr;
    // Queue index
    int i = 0;
    while (true) {
      aTCBPtr = PopTCB(queue);
      aTCBPtr->myTask((aTCBPtr->taskDataPtr));
      if(i == 5) {
        if(GLOBALCOUNTER % MAJOR_CYCLE == 0) {
          usleep(9441.8);
        } else {
          usleep(9465);
        }
        GLOBALCOUNTER++;
      }
      AppendTCB(queue, aTCBPtr);
	    i = (i + 1) % 6;
    }
    return;
}
