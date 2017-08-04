#ifndef NONBLOCK
#define NONBLOCK
#define NB_DISABLE 1
#define NB_ENABLE 0

// commands for consoleDisplay
#define SATELLITESTATUS 'z'
#define ANNUNCIATION 'x'

// commands for vehicleComs
#define FORWARD 'F'
#define BACK 'B'
#define LEFT 'L'
#define RIGHT 'R'
#define DRILL_DOWN 'D'
#define DRILL_UP 'H'
#define REQUEST_LIFTOFF 'T'
#define REQUEST_DOCK 'D'

// commands for warningAlarm
#define OVERHEAT_ACK 'a'

// commands for keyboardConsole
#define SPEEDINC 'i'
#define SPEEDDEC 'd'
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
int kbhit();

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
void nonblock(int state);

bool consoleModeCmd(char c);
bool satVehicleCmd(char c);
bool motorSpeedCmd(char c);
bool warningCmd(char c);


#endif
