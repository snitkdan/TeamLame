/*
  pwm_test.c - This file tests the functionality
  of the PWM on the BBB
*/

#include "pwm_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PIN "P8_13"
#define PERIOD 500000
#define ON 1
#define OFF 0

int main() {
  // 1. Initialize the PWM for P8_13
	if(!initPWM(PIN)) {
    fprintf(stderr, "PWM Malfunction\n");
    return EXIT_FAILURE;
  }
  // 2. Set the period to 500 ms
  setPWMProperty(PIN, "period", PERIOD);
  // 3. Set the duty cycle to 250 ms
	setPWMProperty(PIN, "duty", 250000);
  // 4. Turn on the output
  setPWMProperty(PIN, "run", ON);
  // 5. Delay and turn off
  usleep(5000000);
  setPWMProperty(PIN, "run", OFF);
  return EXIT_SUCCESS;
}
