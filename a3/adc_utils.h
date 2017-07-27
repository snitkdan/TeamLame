/*
 * adc_utils.h - This file contains
 * the specification for ADC utility functions.
 * (see pwm_utils.c for implementation)
 *
 *
 * Inspired by Gavin Strunk:
 *  https://class.ee.washington.edu/474/peckol/code/BeagleBone/ForestExamples/Example_5_Analog/libBBB.c
 *
 */

#ifndef ADC_UTILS
#define ADC_UTILS

#include <stdbool.h>
/*
    @modifies
      BeagleBone
    @effects
      initializes the ADC at
      channel AIN0 on BeagleBone.
    @returns
      true on success, and
      false otherwise.
*/
bool initADC();

/*
  @returns
    ADC reading from channel
    AIN0 on Beaglebone on success,
    and -1 on failure.
*/
int readADC();
#endif
