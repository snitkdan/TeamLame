/*
 * solarPanelControl.c - This file contains
 * the implementation of solarPanelControl
 */

#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include "dataStructs.h"
#include "TCB.h"
#include "pwm_utils.h"

static bool initSolarPanel();

#define PWM_PIN "P9_14"
#define HNUM 16
#define PERIOD 500000
#define DEFAULT_DUTY 250000
#define ON 1
#define OFF 0
#define MAX 300

extern bool fromSolar;
extern bool endOfTravel;

void solarPanelControl(void *solarStruct) {
	fromSolar = true;
	// Only run this function every major cycle
	static unsigned long start = 0;
	if((GLOBALCOUNTER - start) % MAJOR_CYCLE != 0) {
      return;
	}
  start = GLOBALCOUNTER;
  //printf("INSIDE solarPanelControl\n");

  // 1.1 Assign the data of consoleStruct into local variables
  solarData *solData = (solarData*)solarStruct;
  bool *solarPanelState = solData->solarPanelStatePtr;
  bool *solarPanelDeploy = solData->solarPanelDeployPtr;
  bool *solarPanelRetract = solData->solarPanelRetractPtr;
  bool *motorInc = solData->motorIncPtr;
  bool *motorDec = solData->motorDecPtr;

	// 1.2: Track PWM initialization status
	static bool solarPanelInit = false;
  //#define TEST
	#ifndef TEST
	// 1.3: Check PWM initialization
	if(!solarPanelInit) {
		solarPanelInit = initSolarPanel();
		if(!solarPanelInit) {
			//fprintf(stderr, "PWM_13 Malfunction\n");
			return;
		}
	}
	#endif

	// 1.3: Declare variables
	//static double PWM;
	static double duty = DEFAULT_DUTY;
	//static double period = PERIOD;

  //printf("SPS: %d SPD: %d SPR: %d\n", *solarPanelState, *solarPanelDeploy, *solarPanelRetract);
  // 1.4: Check if the solor panel state with what it is requested to do
	*motorInc = true;
	if ((*solarPanelState == 1 && *solarPanelDeploy == 1) || (*solarPanelState == 0 && *solarPanelRetract == 1)){
		if(!endOfTravel) {
			raise(SIGUSR1);
			setPWMProperty(PWM_PIN, "run", OFF, HNUM);
		}
	} else {
		if(endOfTravel) {
			setPWMProperty(PWM_PIN, "run", ON, HNUM);
			endOfTravel = false;
			*solarPanelDeploy = !(*solarPanelDeploy);
			*solarPanelRetract = !(*solarPanelRetract);
		}
		//if need speed to increase then duty (run time ) should decrease
		if(*motorInc == 1) {
			duty += ((5 * duty) / 100);
			duty = (duty > PERIOD) ? PERIOD : duty;
		} else if (*motorDec == 1) {
			//if need speed to decrease then duty (run time ) should increase
			duty -= ((5*duty) / 100);
			duty = (duty < 0) ? 0 : duty;
		}
	}
	// 1.5: Generate the new PWM for the new duty
	//PWM = duty * period;

	// 1.6: Duty cycle and period are in ms.
	//printf("SOLARPANELS: Duty: %f\n", duty);

	setPWMProperty(PWM_PIN, "duty", duty, HNUM);
	fromSolar = false;
}

static bool initSolarPanel() {
  // 2. Set the period to 500 ms
  setPWMProperty(PWM_PIN, "period", PERIOD, HNUM);
  // 3. Set the duty cycle to 250 ms
	setPWMProperty(PWM_PIN, "duty", DEFAULT_DUTY, HNUM);
  // 4. Turn on the output
  setPWMProperty(PWM_PIN, "run", ON, HNUM);
	return true;
}
