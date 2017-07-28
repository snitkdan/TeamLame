/*
 * solarPanelControl.c - This file contains
 * the implementation of solarPanelControl
 */

#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include "dataStructs.h"
#include "TCB.h"
#include "pwm_utils.h"

static bool initSolarPanel();

#define P8_13 "P8_13"
#define HNUM 15
#define PERIOD 500000
#define DEFAULT_DUTY 250000
#define ON 1
#define OFF 0
#define MAX 300

void solarPanelControl(void *solarStruct) {
	// Only run this function every major cycle
	static unsigned long start = 0;
	if((GLOBALCOUNTER - start) % MAJOR_CYCLE != 0) {
      return;
	}
  start = GLOBALCOUNTER;
  printf("INSIDE solarPaenlControl\n");

  // 1.1 Assign the data of consoleStruct into local variables
  solarData *solData = (solarData*)solarStruct;
  bool *solarPanelState = solData->solarPanelStatePtr;
  bool *solarPanelDeploy = solData->solarPanelDeployPtr;
  bool *solarPanelRetract = solData->solarPanelRetractPtr;
  bool *motorInc = solData->motorIncPtr;
  bool *motorDec = solData->motorDecPtr;

	// 1.2: Track PWM initialization status
	static bool pwm_init = false;

	// 1.3: Check PWM initialization
	if(!pwm_init) {
		pwm_init = initSolarPanel();
	}

	// 1.3: Declare variables
	static int PWM;
	static int duty = DEFAULT_DUTY;
	static int period = PERIOD;

  // 1.4: Check if the solor panel state with what it is requested to do
	if ((*solarPanelState == 1 && *solarPanelDeploy == 1) || (*solarPanelState == 0 && *solarPanelRetract == 1)){
		PWM = 0;
	} else {
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
	PWM = duty * period;

	// 1.6: Duty cycle and period are in ms.
	setPWMProperty(P8_13, "duty", duty);
}

static bool initSolarPanel() {
	if(!initPWM(P8_13)) {
    fprintf(stderr, "PWM Malfunction\n");
    return false;
  }
  // 2. Set the period to 500 ms
  setPWMProperty(P8_13, "period", PERIOD, HNUM);
  // 3. Set the duty cycle to 250 ms
	setPWMProperty(P8_13, "duty", 250000, HNUM);
  // 4. Turn on the output
  setPWMProperty(P8_13, "run", ON, HNUM);
	return true;
}
