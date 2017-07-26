/*
 * solarPanelControl.c - This file contains
 * the implementation of solarPanelControl
 */

#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include "dataStructs.h"
#include "TCB.h"
#include "solarPanelControl.h"

#define MAX 300
#define pwm_path "/sys/devices/bone_capemgr.9/slots"
#define duty_path "/sys/devices/ocp.3/pwm_test_P8_13.15/duty"
#define period_path "/sys/devices/ocp.3/pwm_test_P8_13.15/period"
#define run_path "/sys/devices/ocp.3/pwm_test_P8_13.15/run"

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

	duty = 250000;
	period = 500000;

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
	setPWM(duty);
}

bool setPWM(double d) {

	#ifndef BEAGLEBONE
	#define BEAGLEBONE
	FILE *pwm = fopen(pwm_path, "w");
	fseek(pwm,0,SEEK_SET);
	fprintf(pwm,"am33xx_pwm");
	fflush(pwm);
	fprintf(pwm,"bone_pwm_P8_13");
	fflush(pwm);
	FILE *period = fopen(period_path, "w");
	fseek(period,0,SEEK_SET);
	fprintf(period,"%d",500000);
	fflush(period);
	FILE *duty = fopen(duty_path, "w");
	fseek(duty,0,SEEK_SET);
	fprintf(duty,"%d",250000);
	fflush(duty);
	FILE *run = fopen(run_path, "w");
	fseek(run,0,SEEK_SET);
	fprintf(run,"%d",0);
	fflush(run);
	#endif

	// 1. Set the duty
	fseek(duty,0,SEEK_SET);
	fprintf(duty,"%d",d);
	fflush(duty);
	// 2. Run the PMW
	fseek(run,0,SEEK_SET);
	fprintf(run,"%d",1);
	fflush(run);
	fprintf(run,"%d",0);
	fflush(run);
	return true;
}
