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

#define DEVICES "/sys/devices"
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
	int out = fprintf(pwm, "am33xx_pwm");
	if(out < 0) {
		#ifdef ERR_STATEMENT
		fprintf(stderr, "Write error\n");
		#endif
		return false;
	}
	fflush(pwm);
	out = fprintf(pwm, "%s", pin_path);
	if(out < 0) {
		#ifdef ERR_STATEMENT
		fprintf(stderr, "Write error\n");
		#endif
		return false;
	}
	fflush(pwm);
	// 5. Clean up
	fclose(pwm);
	return true;
}

bool setPWMProperty(char *pin, char *prop, double prop_val, int hnum) {
	// 1. Declare necessary variables
	FILE *pwm;
	char prop_path[45];
	// 2. Construct the file path for desired property
	sprintf(prop_path, "%s/ocp.3/pwm_test_%s.%i/%s", DEVICES, pin, hnum, prop);
	// 3, Open the pwm
	pwm = fopen(prop_path, "w");
	if(!pwm) {
		#ifdef ERR_STATEMENT
		fprintf(stderr, "Failed to update %s\n", prop);
		#endif
		return false;
	}
	// 4. Set the property
	fprintf(pwm, "%f", prop_val);
	fflush(pwm);
	// 5. Clean up
	fclose(pwm);
	return true;
}
