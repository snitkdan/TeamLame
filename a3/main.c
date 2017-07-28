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

// TODO:
/*
  1. Update public interface for PWM utils
  to include doubles and update existing code
  accordingly. (DONE)

  2. Get ADC queue working with a
  "next" method and declaring
  the right local variables
  for it in startup.c

  3. Get thrusterSubsystem &
  solarPanelControl to communicate
  on separate PWM channels
  at the same time.

  4. Update the main Makefile
  to integrate the whole
  project.
*/

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

	// make a pipe that can be seen by vehicleComs and vehicle
	// fork the two processes
		int i = 0;
		while (true) {
		  if ((solarPanelState == 1 && solarPanelDeploy == 1) || (solarPanelState == 0 && solarPanelRetract == 1)) {
		      RemoveTCB(queue, &solarPanelControlTCB);
			  RemoveTCB(queue, &keyboardConsoleTCB);
		 else {
		      AppendTCB(queue, &solarPanelControlTCB);
			  AppendTCB(queue, &keyboardConsoleTCB);
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
