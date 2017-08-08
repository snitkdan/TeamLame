/*
  startup.h - This file contains
  the specification for the
  Startup task.
  (see startup.c for implementation)
*/

#ifndef STRT_UP
#define STRT_UP

#include "TCB.h"

/*
  @effects
    initializes all necessary global
    variables, TCBs, etc.
*/
void Initialize(void);

/*
  @effects
    configures and activates
    the system time base.
*/
void ActivateTimeBase(void);

/*
  @effects
    initializes the ADC / PWM
*/
void InitHardware(void);
#endif
