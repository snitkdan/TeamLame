// Structs associated with each task hold the shared data
#ifndef DATASTRUCTS
#define DATASTRUCTS

#include <stdbool.h>

extern unsigned long GLOBALCOUNTER;
#define MAJOR_CYCLE 500

void checkOpened(FILE *led); // used in warningAlarm and Main

typedef struct powerSubsystemData {
  bool *solarPanelStatePtr;
  bool *solarPanelDeployPtr;
  bool *solarPanelRetractPtr;
  unsigned short *batteryLvlPtr; // TO BE CHANGED LATER
  unsigned short *pConsumePtr;
  unsigned short *pGeneratePtr;
} powerData;

typedef struct solarPanelControlData {
  bool *solarPanelStatePtr;
  bool *solarPanelDeployPtr;
  bool *solarPanelRetractPtr;
  bool *motorIncPtr;
  bool *motorDecPtr;
} solarData;

typedef struct keyboardConsoleData {
  bool *motorIncPtr;
  bool *motorDecPtr;
} keyboardData;

typedef struct vehicleCommsData {
  char *commandPtr;
  char *responsePtr;
} vehicleData;

typedef struct thrusterSubsystemData {
  unsigned int *thrusterCommandPtr;
  unsigned short *fuelLvlPtr;
} thrustData;

typedef struct satelliteComsData {
  bool *fuelLowPtr;
  bool *batteryLowPtr;
  bool *solarPanelStatePtr;
  unsigned short *batteryLvlPtr; // TO BE CHANGED LATER
  unsigned short *fuelLvlPtr;
  unsigned short *pConsumePtr;
  unsigned short *pGeneratePtr;
  unsigned int *thrusterCommandPtr;
  char *commandPtr;
  char *responsePtr;  
} satData;

typedef struct consoleDisplayData {
  bool *fuelLowPtr;
  bool *batteryLowPtr;
  bool *solarPanelStatePtr;
  unsigned short *batteryLvlPtr; // TO BE CHANGED LATER
  unsigned short *fuelLvlPtr;
  unsigned short *pConsumePtr;
  unsigned short *pGeneratePtr;
} consoleData;

typedef struct warningAlarmData {
  bool *fuelLowPtr;
  bool *batteryLowPtr;
  unsigned short *batteryLvlPtr; // TO BE CHANGED LATER
  unsigned short *fuelLvlPtr;
} warnData;

#endif
