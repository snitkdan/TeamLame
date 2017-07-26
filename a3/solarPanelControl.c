/*
 * solarPanelControl.c - This file contains
 * the implementation of solarPanelControl
 */

#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include "dataStructs.h"
#include "TCB.h"

#define MAX 300
#define pwm_path "/sys/devices/bone_capemgr.9/slots"

void solarPanelControl(void *solarStruct) {
	// Only run this function every major cycle
	static unsigned long start = 0;
	if((GLOBALCOUNTER - start) % MAJOR_CYCLE != 0) {
      return;
	}
  start = GLOBALCOUNTER;

  // 1.1 Assign the data of consoleStruct into local variables
  solarData *solData = (solarData*)solarStruct;
  bool *solarPanelState = solData->solarPanelStatePtr;
  bool *solarPanelDeploy = solData->solarPanelDeployPtr;
  bool *solarPanelRetract = solData->solarPanelRetractPtr;
  bool *motorInc = solData->motorIncPtr;
  bool *motorDec = solData->motorDecPtr;

	// 1.2: Declare variables
	double PWM, duty, period;

  // 1.3: Check if the solor panel state with what it is requested to do
	if ((*solarPanelState == 1 && *solarPanelDeploy == 1) || (*solarPanelState == 0 && *solarPanelRetract == 1)){
		PWM = 0;
	} else {
		//if need speed to increase then duty (run time ) should decrease
		if(*motorInc == 1) {
			duty -= ((5 * duty) / 100);
			duty = (duty > 100) ? 100 : duty;
		} else if (*motorDec == 1) {
			//if need speed to decrease then duty (run time ) should increase
			duty += ((5*duty) / 100);
			duty = (duty < 0) ? 0 : duty;
		}
	}
	// 1.4: Generate the new PWM for the new duty
	PWM = duty * period;
}

bool setPWM(double PWM) {
	#ifdef BEAGLEBONE
	FILE *f = fopen(pwm_path, "w");
	if (!f) {
		perror("");
		return false;
	}
	fprintf(f, PWM);
	fflush(f);
	fclose(f);
	return true;
	#endif
	return false;
}
