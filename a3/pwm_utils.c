/*
 * pwm_utils.c - This file contains
 * the implementation for PWM utility functions.
 * (see pwm_utils.h for specification)
 *
 * Inspired by Gavin Strunk:
 *  https://class.ee.washington.edu/474/peckol/code/BeagleBone/ForestExamples/Example_4_PWM/Example_4_PWM/libBBB.c
 *
 */

#include <stdio.h>
#include <stdbool.h>
#include "pwm_utils.h"

bool pwm_initialized = false;

#define DEVICES "/sys/devices"
#define HNUM 15
#define MGRNUM 9

bool initPWM(char *pin) {
	// 1. Declare necessary variables
	FILE *pwm;
	char mgr_path[35];
	char pin_path[15];
	// 2. Construct file paths for the mgr & pin
	sprintf(mgr_path, "%s/%s%i/slots", DEVICES, "bone_capemgr.", MGRNUM);
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

bool setPWMProperty(char *pin, char *prop, int prop_val) {
	// 1. Declare necessary variables
	FILE *pwm;
	char prop_path[45];
	// 2. Construct the file path for desired property
	sprintf(prop_path, "%s/ocp.2/pwm_test_%s.%i/%s", DEVICES, pin, HNUM, prop);
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
