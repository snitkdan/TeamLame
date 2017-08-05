/*
  pwm_test.c - This file tests the functionality
  of the PWM on the BBB
*/

#include "pwm_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ACH "AIN0"

int main() {
  if(!initADC()) {
    fprintf(stderr, "ADC Malfunction\n");
    return EXIT_FAILURE;
  }
  int val = readADC(ACH);
  printf("ADC val: %d\n", val);
}
