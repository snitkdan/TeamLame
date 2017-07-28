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
#define HNUM 18

#define ON 1
#define OFF 0

static bool initThrusters();

void thrusterSubsystem(void *thrustStruct) {
  // Only run this function every major cycle
  static unsigned long start = 0;
  if((GLOBALCOUNTER - start) % MAJOR_CYCLE != 0) {
      return;
  }
  start = GLOBALCOUNTER;
  printf("INSIDE thrusterSubsy\n");

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
  static bool pwm_init = false;
  if(!pwm_init) {
		pwm_init = initThrusters();
	}
  // 5.2: Set duty / period
  int duty = (cc.magnitude > 0) ? (cc.duration / cc.magnitude) : 0;
  int period = cc.magnitude;
  printf("Duty: %d, Period: %d\n", duty, period);
  setPWMProperty(P8_19, "duty", duty, HNUM);
  setPWMProperty(P8_19, "period", period, HNUM);
}

static bool initThrusters() {
	if(!initPWM(P8_19)) {
    fprintf(stderr, "PWM Malfunction\n");
    return false;
  }
  // 2. Set the period to 500 ms
  setPWMProperty(P8_19, "period", 0, HNUM);
  // 3. Set the duty cycle to 250 ms
	setPWMProperty(P8_19, "duty", 0, HNUM);
  // 4. Turn on the output
  setPWMProperty(P8_19, "run", ON, HNUM);
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
