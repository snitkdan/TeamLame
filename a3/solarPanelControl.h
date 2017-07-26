#ifndef SP_CTRL
#define SP_CTRL
#include <stdbool.h>
/*
  @param PWM
    target PWM to set
  @modifies
    beaglebone PWM
  @effects
    sets the beaglebone
    pwm to the target PWM
  @returns
    true on success, and
    false otherwise
*/
bool setPWM(double PWM);
#endif
