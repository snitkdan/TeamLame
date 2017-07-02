#ifndef DATASTRUCTS
#define DATASTRUCTS

// Could be wrong...unsure how to declare the shared data

struct powerSubsystemData 
{
  unsigned short *batteryLvl;
  unsigned short *fuelLvl;
  unsigned short *pConsume;
  unsigned short *pGenerate;
};
typedef struct powerSubsystemData powerData;


struct thrusterSubsystemData
{
  unsigned int *thrustCommand;
  unsigned short *fuelLvl;
};
typedef struct thrusterSubsystemData thrustData;

struct satelliteComsData
{
  Bool *fuelLow;
  Bool *batteryLow;
  Bool *solarPanelState;
  unsigned short *batteryLvl;
  unsigned short *fuelLvl;
  unsigned short *pConsume;
  unsigned short *pGenerate;
  unsigned int *thrusterCommand;
};
typedef struct satelliteComsData satData;

struct consoleDisplayData
{
  Bool *fuelLow;
  Bool *batteryLow;
  Bool *solarPanelState;
  unsigned short *batteryLvl;
  unsigned short *fuelLvl;
  unsigned short *pConsume;
  unsigned short *pGenerate;
};
typedef struct consoleDisplayData consoleData;

struct warningAlarmData
{
  Bool *fuelLow;
  Bool *batteryLow;
  unsigned short *batteryLvl;
  unsigned short *fuelLvl;
};
typedef struct warningAlarmData warnData;

#endif
