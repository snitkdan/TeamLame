// Structs associated with each task hold the shared data
#ifndef DATASTRUCTS
#define DATASTRUCTS

struct powerSubsystemData
{
  bool *solarPanelStatePtr;
  unsigned short *batteryLvlPtr;
  unsigned short *pConsumePtr;
  unsigned short *pGeneratePtr;
};
typedef struct powerSubsystemData powerData;


struct thrusterSubsystemData
{
  unsigned int *thrusterCommandPtr;
  unsigned short *fuelLvlPtr;
};
typedef struct thrusterSubsystemData thrustData;

struct satelliteComsData
{
  bool *fuelLowPtr;
  bool *batteryLowPtr;
  bool *solarPanelStatePtr;
  unsigned short *batteryLvlPtr;
  unsigned short *fuelLvlPtr;
  unsigned short *pConsumePtr;
  unsigned short *pGeneratePtr;
  unsigned int *thrusterCommandPtr;
};
typedef struct satelliteComsData satData;

struct consoleDisplayData
{
  bool *fuelLowPtr;
  bool *batteryLowPtr;
  bool *solarPanelStatePtr;
  unsigned short *batteryLvlPtr;
  unsigned short *fuelLvlPtr;
  unsigned short *pConsumePtr;
  unsigned short *pGeneratePtr;
};
typedef struct consoleDisplayData consoleData;

struct warningAlarmData
{
  bool *fuelLowPtr;
  bool *batteryLowPtr;
  unsigned short *batteryLvlPtr;
  unsigned short *fuelLvlPtr;
};
typedef struct warningAlarmData warnData;

#endif
