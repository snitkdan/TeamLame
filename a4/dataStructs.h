// Structs associated with each task hold the shared data
#ifndef DATASTRUCTS
#define DATASTRUCTS

#include <stdbool.h>

extern unsigned long GLOBALCOUNTER;
extern int fd0;
#define MAJOR_CYCLE 50

//#define BEAGLEBONE //define this if connected to beaglebone

void checkOpened(FILE *led); // used in warningAlarm and Main

typedef struct powerSubsystemData {
  bool *solarPanelStatePtr;
  bool *solarPanelDeployPtr;
  bool *solarPanelRetractPtr;
  unsigned int *batteryLvlPtr;
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
  unsigned int *batteryLvlPtr;
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
  bool *batteryOverTempPtr;
  unsigned int *batteryLvlPtr;
  unsigned short *fuelLvlPtr;
  unsigned short *pConsumePtr;
  unsigned short *pGeneratePtr;
  unsigned short *distancePtr;
} consoleData;

typedef struct warningAlarmData {
  bool *fuelLowPtr;
  bool *batteryLowPtr;
  unsigned int *batteryLvlPtr;
  unsigned short *fuelLvlPtr;
} warnData;

typedef struct transportDistanceData {
  unsigned short *distancePtr;
} transportData;

typedef struct imageCaptureData {

} imageData;

typedef struct batteryTempData {
  bool *batteryOverTempPtr;
  unsigned int *batteryTmp1;
  unsigned int *batteryTmp2;
} tempData;
#endif
