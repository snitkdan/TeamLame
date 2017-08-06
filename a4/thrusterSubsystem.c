/*

  thrusterSubsystem.c - This file contains
  the implementation of thrusterSubsystem.
  (see thrusterSubsystem.h for specification).

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdlib.h>
#include "dataStructs.h"
#include "thrusterSubsystem.h"
#include "pwm_utils.h"

#define PWM_PIN "P9_14"
#define HNUM_14 17

#define ON 1
#define OFF 0

//#define ERR_STATEMENT

static bool initThrusters();

void thrusterSubsystem(void *thrustStruct) {
  // Only run this function every major cycle
  static unsigned long start = 0;
  if((GLOBALCOUNTER - start) % MAJOR_CYCLE != 0) {
      return;
  }
  start = GLOBALCOUNTER;
  //printf("INSIDE thrusterSubsy\n");

  // 1. Assign the data of thrustStruct into local variables
  thrustData *tData = (thrustData *) thrustStruct;
  unsigned int *thrusterCommand = tData->thrusterCommandPtr;
  unsigned short *fuelLvl = tData->fuelLvlPtr;

  // 2. Parse the thrusterCommand
  cleanCommands cc;
  parseCommands(thrusterCommand, &cc);

  // 3. Calculate the fuel cost
  static double fuelCost = 0;
  fuelCost += getFuelCost(&cc);


  // 4. Update the fuelLvl
  if (*fuelLvl == 0) {
      *fuelLvl = 0;
	  fprintf(stderr, "\n\nOut of fuel, you dead\n");
      exit(0);
  } else {
      *fuelLvl -= (unsigned short)fuelCost;
  }
  // 5. Recalibrate fuelCost
  fuelCost -= (unsigned short)fuelCost;

  // 5. PWM things
  // 5.1: Initialization checks
  /*
  static bool thrusterInit = false;
  if(!thrusterInit) {
		thrusterInit = initThrusters();
    if(!thrusterInit) {
      //fprintf(stderr, "PWM_19 Malfunction\n");
      return;
    }
	}*/
  // 5.2: Set duty / period
  double duty = (cc.duration > 0) ? (cc.magnitude / cc.duration) : 0;
  double period = cc.magnitude;
  //printf("THRUSTERS: Duty: %f, Period: %f\n", duty, period);
  /*setPWMProperty(PWM_PIN, "period", period, HNUM_14);
  setPWMProperty(PWM_PIN, "duty", duty, HNUM_14);*/
}

static bool initThrusters() {
	if(!initPWM(PWM_PIN)) {
      return false;
  }
  setPWMProperty(PWM_PIN, "period", 1, HNUM_14);
	setPWMProperty(PWM_PIN, "duty", 1, HNUM_14);
  // 4. Turn on the output
  setPWMProperty(PWM_PIN, "run", ON, HNUM_14);
	return true;
}

void parseCommands(unsigned int *thrusterCommand, cleanCommands *cc) {
  // 0. Define a MASK (1111)
  uint16_t MASK = 0xF;

  // 1. Extract duration, magnitude, and thruster direction
  uint8_t duration = *thrusterCommand >> 8;
  uint8_t magnitude = (*thrusterCommand >> 4) & MASK;
  uint8_t thruster_dir = *thrusterCommand & MASK;

  // 2. Assign the fields of the struct.
  cc->duration = duration;
  cc->magnitude = magnitude;
  cc->thruster_dir = thruster_dir;
}

double getFuelCost(cleanCommands *cc) {
  double cost = (double)0.0001284522 * (double)(cc->magnitude * cc->duration);
  return cost;
}
