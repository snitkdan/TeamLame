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

#define P8_19 "P8_19"
#define HNUM_19 15

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
  static bool pwm_19_init = false;
  if(!pwm_19_init) {
		pwm_19_init = initThrusters();
    if(!pwm_19_init) {
      fprintf(stderr, "PWM_19 Malfunction\n");
      return;
    }
	}
  // 5.2: Set duty / period
  double duty = (cc.magnitude > 0) ? (cc.duration / cc.magnitude) : 0;
  double period = cc.magnitude;
  setPWMProperty(P8_19, "duty", duty, HNUM_19);
  setPWMProperty(P8_19, "period", period, HNUM_19);
}

static bool initThrusters() {
	if(!initPWM(P8_19)) {
      return false;
  }
  // 2. Set the period to 500 ms
  setPWMProperty(P8_19, "period", 0, HNUM_19);
  // 3. Set the duty cycle to 250 ms
	setPWMProperty(P8_19, "duty", 0, HNUM_19);
  // 4. Turn on the output
  setPWMProperty(P8_19, "run", ON, HNUM_19);
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
