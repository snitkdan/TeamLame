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


//#define TEST

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
    extern TCB batteryTempTCB;
    extern TCB transportDistanceTCB;
  	extern TCB pirateManagementTCB;
  	extern TCB commandParserTCB;

    extern bool solarPanelState;
    extern bool solarPanelDeploy;
    extern bool solarPanelRetract;
    extern bool snapshot;
  	extern bool pirateDetected;
  	extern bool commandOn;
	extern bool isPaused;

	int i = 0;
	while (true) {
	if (!isPaused) {
	//printf("IN PAUSED\n");
	//usleep(10000000);
    if(solarPanelState) {
      // SolarPanelState ON
      if(solarPanelDeploy) {
        // Charging
        if(!ContainsTCB(queue, &batteryTempTCB)) {
          AppendTCB(queue, &batteryTempTCB);
        }
      } else {
        // Solar Panel switch state
        if(!ContainsTCB(queue, &solarPanelControlTCB)) {
          AppendTCB(queue, &solarPanelControlTCB);
          AppendTCB(queue, &keyboardConsoleTCB);
        }
      }
    } else {
      // SolarPanelState OFF
      if(!solarPanelRetract) {
        // Solar Panel switch state
        if(!ContainsTCB(queue, &solarPanelControlTCB)) {
          AppendTCB(queue, &solarPanelControlTCB);
          AppendTCB(queue, &keyboardConsoleTCB);
        }
      } else {
        // SolarPanelState OFF && retracted (may need to remove tasks)
        if(ContainsTCB(queue, &batteryTempTCB)) {
          RemoveTCB(queue, &batteryTempTCB);
        }
        if (ContainsTCB(queue, &solarPanelControlTCB)) {
          RemoveTCB(queue, &solarPanelControlTCB);
          RemoveTCB(queue, &keyboardConsoleTCB);
        }
      }
    }
	  // Scheduling Transport Distance and Image Capture
    if (snapshot) {
      if(!ContainsTCB(queue, &transportDistanceTCB))  {
		    printf("Calculating transport distance...\n");
            AppendTCB(queue, &transportDistanceTCB);
      }
    } else {
  		if(ContainsTCB(queue, &transportDistanceTCB))  {
            RemoveTCB(queue, &transportDistanceTCB);
  	    }
	  }
	  // Scheduling pirates
	  if (pirateDetected) {
  		if(!ContainsTCB(queue, &pirateManagementTCB)) {
  			//printf("Pirate Detected!!\n");
  			AppendTCB(queue, &pirateManagementTCB);
  		}
  	 } else {
  		if(ContainsTCB(queue, &pirateManagementTCB)) {
  			RemoveTCB(queue, &pirateManagementTCB);
  			//printf("Pirate ran away. (Like a lil bitch lmao)\n");
  		}
  	}
	}
	  // Scheduling CommandParser
	  if (commandOn) {
  		if (!ContainsTCB(queue, &commandParserTCB)) {
  			PushTCB(queue, &commandParserTCB);
  	  }
  	} else {
  		if(ContainsTCB(queue, &commandParserTCB)) {
  			RemoveTCB(queue, &commandParserTCB);
  		}
	  }
	  aTCBPtr = PopTCB(queue);
	  aTCBPtr->myTask((aTCBPtr->taskDataPtr));
	  if(i == 5) {
			if(GLOBALCOUNTER % MAJOR_CYCLE == 0) {
			  usleep(70495.4);
			} else {
			  usleep(79804);
			}
		  GLOBALCOUNTER++;
	  }
	  AppendTCB(queue, aTCBPtr);
	  i = (i + 1) % 11;
	}
	return;
}
