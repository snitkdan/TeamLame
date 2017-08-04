#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "TCB.h"
#include "dataStructs.h"
#include "warningAlarm.h"
#include "startup.h"
#include "scheduler.h"


#define TEST

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

    extern TCB solarPanelControlTCB;
    extern TCB keyboardConsoleTCB;
    extern bool solarPanelState;

    extern bool solarPanelDeploy;
    extern bool solarPanelRetract;

    static int app = 0;
    static int rem = 1;

    int test_task = 8;

		int i = 0;
		while (true) {
		  if ((solarPanelState && !solarPanelDeploy) ||
          (!solarPanelState && !solarPanelRetract)) {
        if (append==1) {
          printf("Appending solar and keyboard\n");
          AppendTCB(queue, &solarPanelControlTCB);
          AppendTCB(queue, &keyboardConsoleTCB);
          app = 0;
          rem = 1;
          printf("%u\n", NumTasksInTaskQueue(queue));
        }
      } else if (remove) {
        printf("Removing solar and keyboard\n");
        RemoveTCB(queue, &solarPanelControlTCB);
        RemoveTCB(queue, &keyboardConsoleTCB);
        app = 1;
        rem = 0;
		  }
		  aTCBPtr = PopTCB(queue);
		  aTCBPtr->myTask((aTCBPtr->taskDataPtr));
		  /*if(i == 5) {
  			if(GLOBALCOUNTER % MAJOR_CYCLE == 0) {
  			  usleep(70495.4);
  			} else {
  			  usleep(79804);
  			}
			  GLOBALCOUNTER++;
		  }*/
		  AppendTCB(queue, aTCBPtr);
			i = (i + 1) % 9;
		}
		return;
}
