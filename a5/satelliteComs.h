#ifndef SATCOMS
#define SATCOMS
#include <stdint.h>


/*
  @param satelliteComs
    Struct containing necessary
    data for the satelliteComs
  @modifies
    *sData.thrusterCommand
  @effects
    changes thrusterCommand based
    on random integer from rand2.c
*/
void satelliteComs(void *sData);

/*
  @param thrusterCommand
    16-bit instruction of the form:
    [X X X X X X X] [Y Y Y Y] [Z Z Z Z]
       duration     magnitude direction
  @modifies
    *thrusterCommand
  @effects
    Masks the last 4 bits to be 00XX
*/
//void maskBit(unsigned int *thrusterCommand);
bool validCmd(char c);
void clearScreen(int fd1, char c);
#endif