#include <stdio.h>
#include "dataStructs.h"
#include "thrusterSubsystem.h"

void thrusterSubsystem(void *thrustStruct) {
  // 1. Assign the data of thrustStruct into local variables
  thrustData *tData = (thrustData *) thrustStruct;
  unsigned int *thrusterCommand = tData->thrusterCommandPtr;
  unsigned short *fuelLvl = tData->fuelLvlPtr;

  // 2. Parse the thrusterCommand
  uint8_t duration = *thrusterCommand & (0xFF << 8);
  uint8_t magnitude = *thrusterCommand & (0xF << 4);

}
