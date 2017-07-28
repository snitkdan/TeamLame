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

static void initSolarPanel();

#define PIN "P8_13"
#define PERIOD 500000
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
		initSolarPanel();
	}

	// 1.3: Declare variables
	double PWM, duty, period;

  // 1.4: Check if the solor panel state with what it is requested to do
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
	// 1.5: Generate the new PWM for the new duty
	PWM = duty * period;

	// 1.6: Duty cycle and period are in ms.
	setPWMProperty(PIN, "duty", duty);
}

static void initSolarPanel() {
	if(!initPWM(PIN)) {
    fprintf(stderr, "PWM Malfunction\n");
    return;
  }
  // 2. Set the period to 500 ms
  setPWMProperty(PIN, "period", PERIOD);
  // 3. Set the duty cycle to 250 ms
	setPWMProperty(PIN, "duty", 250000);
  // 4. Turn on the output
  setPWMProperty(PIN, "run", ON);
}
