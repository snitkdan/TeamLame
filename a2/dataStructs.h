// Structs associated with each task hold the shared data
#ifndef DATASTRUCTS
#define DATASTRUCTS

#include <stdbool.h>

extern unsigned long GLOBALCOUNTER;
#define MAJOR_CYCLE 500

void checkOpened(FILE *led); // used in warningAlarm and Main

typedef struct powerSubsystemData {
  bool *solarPanelStatePtr;
  unsigned short *batteryLvlPtr;
  unsigned short *pConsumePtr;
  unsigned short *pGeneratePtr;
} powerData;

typedef struct thrusterSubsystemData {
  unsigned int *thrusterCommandPtr;
  unsigned short *fuelLvlPtr;
} thrustData;

typedef struct satelliteComsData {
  bool *fuelLowPtr;
  bool *batteryLowPtr;
  bool *solarPanelStatePtr;
  unsigned short *batteryLvlPtr;
  unsigned short *fuelLvlPtr;
  unsigned short *pConsumePtr;
  unsigned short *pGeneratePtr;
  unsigned int *thrusterCommandPtr;
} satData;

typedef struct consoleDisplayData {
  bool *fuelLowPtr;
  bool *batteryLowPtr;
  bool *solarPanelStatePtr;
  unsigned short *batteryLvlPtr;
  unsigned short *fuelLvlPtr;
  unsigned short *pConsumePtr;
  unsigned short *pGeneratePtr;
} consoleData;

typedef struct warningAlarmData {
  bool *fuelLowPtr;
  bool *batteryLowPtr;
  unsigned short *batteryLvlPtr;
  unsigned short *fuelLvlPtr;
  //LED *leds[3];
} warnData;

#endif
