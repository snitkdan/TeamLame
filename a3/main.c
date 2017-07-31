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
		int i = 0;
    static int app = 0;
    static int rem = 1;
		while (true) {
      if ((solarPanelState && !solarPanelDeploy) || (!solarPanelState && !solarPanelRetract)) {
            if (append==1) {
  		        AppendTCB(queue, &solarPanelControlTCB);
  			      AppendTCB(queue, &keyboardConsoleTCB);
              app = 0;
              rem = 1;
            } else if (rem == 1) {
  		        RemoveTCB(queue, &solarPanelControlTCB);
  			      RemoveTCB(queue, &keyboardConsoleTCB);
              app = 1;
              rem = 0;
		      }
      }
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
