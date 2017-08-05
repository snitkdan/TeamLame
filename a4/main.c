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
    extern TCB imageCaptureTCB;


    extern bool solarPanelState;
    extern bool solarPanelDeploy;
    extern bool solarPanelRetract;

    extern bool snapshot;

    int test_task = 8;

		int i = 0;
		while (true) {

      #ifndef TEST
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
        }
      }
      if(snapshot) {
        if(!ContainsTCB(queue, &transportDistanceTCB)) {
          //AppendTCB(queue, &imageCaptureTCB);
          AppendTCB(queue, &transportDistanceTCB);
        }
      }
      #endif

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
			i = (i + 1) % 9;
		}
		return;
}
