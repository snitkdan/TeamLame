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
#include "solarPanelControl.h"

// PWM Functions (inspired by Gavin Strunk: https://class.ee.washington.edu/474/peckol/code/BeagleBone/ForestExamples/Example_4_PWM/Example_4_PWM/libBBB.c)
static bool initPWM(int mgrnum, char* pin);
static bool setPWMProperty(int helpnum, char *pin, char *prop, int prop_val);

static bool pwm_initialized = false;

#define DEVICES "/sys/devices"
#define MAX 300
#define PIN "P8_13"
#define HNUM 14
#define MGRNUM 8

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
	if(!pwm_initialized) {
		if(!initPWM(MGRNUM, PIN)) fprintf(stderr, "PWM Malfunction\n");
	}
	setPWMProperty(HNUM, PIN, "duty", duty);
}

static bool initPWM(ing mgrnum, char *pin) {
	// 1. Declare necessary variables
	FILE *pwm;
	char mgr_path[50];
	char pin_path[20];
	// 2. Construct file paths for the mgr & pin
	sprintf(mgr_path, "%s/%s/%i/slots", DEVICES, "bone_capemgr.", mgrnum);
	sprintf(pin_path, "bone_pwm_%s", pin);
	// 3. Open the pwm
	pwm = fopen(mgr_path, "w");
	if(!pwm) {
		fprintf(stderr, "Failed to initialize PWM for pin %s\n", pin);
		return false;
	}
	// 4. Configure the PWM
	fprintf(pwm, "am33xx_pwm");
	fprintf(pwm, "%s", pin_path);
	fflush(pwm);
	// 5. Clean up
	fclose(pwm);
	return true;
}

static bool setPWMProperty(int helpnum, char *pin, char *prop, int prop_val) {
	// 1. Declare necessary variables
	FILE *pwm;
	char prop_path[60];
	// 2. Construct the file path for desired property
	sprintf(prop_path, "%s/ocp.2/pwm_test_%s.%i/%s", DEVICES, pin, helpnum, prop);
	// 3, Open the pwm
	pwm = fopen(prop_path, "w");
	if(!pwm) {
		fprintf(stderr, "Failed to update %s\n", prop);
		return false;
	}
	// 4. Set the property
	fprintf(pwm, "%d", prop_val);
	fflush(pwm);
	// 5. Clean up
	fclose(pwm);
	pwm_initialized = true;
	return true;
}
