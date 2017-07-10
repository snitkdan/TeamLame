#include <stdint.h>

// For parsed thruster command data
typedef struct parsedThrusterCommands {
  uint8_t duration, magnitude, thruster_dir;
} cleanCommands;

/*
  @param thrustStruct
    Struct containing necessary
    data for the powerSubsystem
  @modifies
    *thrustStruct.fuelLvl
  @effects
    changes fuelLvl based on
    *thrustStruct.thrusterCommand
*/
void thrusterSubsystem(void *thrustStruct);

/*
  @param thrusterCommand
    16-bit instruction of the form:
    [X X X X X X X] [Y Y Y Y] [Z Z Z Z]
       duration     magnitude direction
  @param cc
    Struct to contain parsed thruster commands
  @modifies
    cc
  @effects
    Stores the parsed thruster command in cc
*/
void parseCommands(unsigned int *thrusterCommand, cleanCommands *cc);

/*
  @param cc
    Struct that contains parsed thruster commands
  @returns
    Fuel cost given the commands in cc
*/
double getFuelCost(cleanCommands *cc);
