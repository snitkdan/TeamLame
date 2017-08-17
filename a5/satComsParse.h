#ifndef SATPARSE
#define SATPARSE

/* Valid M<payload> commands?
  MF - Fuel Level
  MB - Battery Level
  MC - Power Consumed          (spec doesn't say power generated)
  MT - Battery Temp            (both of them)
  MS - Solar Panels
  MD - Transport Distance      (if active)
  MI - Image Capture Frequency (if active)
  MP - Pirate Distance       (if active)
  
  S
  P
  S
  W
  MF
  MB
  MC
  MT
  MS
  MD
  MI
  MP
*/

// Acknowledgements
#define OK 'A'
#define ERR 'E'
#define BAD_COMMAND 'X'

// Valid "Received"
#define START 'S'
#define STOP 'P'
#define DISPLAY 'D'
#define THRUSTER 'T'
#define MEASURE 'M'

// Valid "Transmit"

#define SHOW_FUEL 'F'
#define SHOW_BATT 'B'
#define SHOW_PCON 'C'
#define SHOW_TEMP 'T'
#define SHOW_PANEL 'S'
#define SHOW_DIST 'D'
#define SHOW_IMAG 'I'
#define SHOW_PIRATE 'P'
#define SHOW_EMPTY '\0'
#define NO_MEASURE 'Q';

#endif
