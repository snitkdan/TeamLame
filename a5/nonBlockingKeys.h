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
#define START_IMAGE 'S'
#define SEND_IMAGE 'I'

// commands for warningAlarm
#define OVERHEAT_ACK 'a'

// commands for keyboardConsole
#define SPEEDINC 'i'
#define SPEEDDEC 'd'

// commands for Earth to Satellite
#define THRUSTER 'T'
#define MEASURE  'M'

// commands for pirates
#define PHASORS_BITCH 'p'
#define PHOTONS_BITCH 'o'


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

bool consoleModeCmd(char c);
bool satVehicleCmd(char c);
bool motorSpeedCmd(char c);
bool warningCmd(char c);
bool payloadCmd(char c);
bool piratesCmd(char c);
bool checkAll(char c);


#endif
