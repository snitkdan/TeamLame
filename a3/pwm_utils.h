/*
 * pwm_utils.h - This file contains
 * the specification for PWM utility functions.
 * (see pwm_utils.c for implementation)
 *
 * Inspired by Gavin Strunk:
 *  https://class.ee.washington.edu/474/peckol/code/BeagleBone/ForestExamples/Example_4_PWM/Example_4_PWM/libBBB.c
 *
 */
#ifndef PWM_UTILS
#define PWM_UTILS

#include <stdbool.h>
/*
    @param pin
      desired pin to activate
    @modifies
      BeagleBone
    @effects
      initializes the PWM at
      pin on BeagleBone.
    @returns
      true on success, and
      false otherwise.
*/
bool initPWM(char* pin);

/*
  @param pin
    desired pin to set
  @param prop
    desired property to set
  @param prop_val
    desired value of prop
  @modifies
    BeagleBone
  @effects
    sets prop of pin to prop_val
    on the BeagleBone
  @returns
    true on success, and
    false otherwise.
*/
bool setPWMProperty(char *pin, char *prop, int prop_val, int hnum);
#endif
