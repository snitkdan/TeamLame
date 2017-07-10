#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "dataStructs.h"
#include "thrusterSubsystem.h"



void thrusterSubsystem(void *thrustStruct) {
  // 1. Assign the data of thrustStruct into local variables
  thrustData *tData = (thrustData *) thrustStruct;
  unsigned int *thrusterCommand = tData->thrusterCommandPtr;
  unsigned short *fuelLvl = tData->fuelLvlPtr;

  // 2. Parse the thrusterCommand
  cleanCommands cc;
  parseCommands(thrusterCommand, &cc);

  // 3. Calculate the fuel cost
  unsigned short fuelCost = getFuelCost(&cc);

  // 4. Update the fuelLvl
  *fuelLvl -= fuelCost;
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

unsigned short getFuelCost(cleanCommands *cc) {
  return 1;
}
