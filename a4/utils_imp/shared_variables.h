/*
  shared_variables.h - This file contains
  the declarations for the
  global shared variables.
*/

// Thrusters
unsigned int thrusterCommand;
unsigned short fuelLvl;
// Power Subsystem
unsigned int *batteryLvl;
unsigned int batteryBuff[BUF_SIZE] = {100};
unsigned short pConsume;
unsigned short pGenerate;
// Battery Temp
unsigned int batteryTempBuff1[BUF_SIZE] = {100};
unsigned int batteryTempBuff2[BUF_SIZE] = {100};
unsigned int *batteryTmp1;
unsigned int *batteryTmp2;
bool batteryOverTemp;
// Transport distance
unsigned short distance;
// Solar Panel
bool solarPanelState;
bool solarPanelDeploy;
bool solarPanelRetract;
bool motorInc;
bool motorDec;
// Warning Alarm
bool fuelLow;
bool batteryLow;
// Sat Comms
char command;
char response;

// Declare TCBs
TCB powerSubsystemTCB;
TCB solarPanelControlTCB;
TCB keyboardConsoleTCB;
TCB vehicleCommsTCB;
TCB thrusterSubsystemTCB;
TCB satelliteComsTCB;
TCB consoleDisplayTCB;
TCB warningAlarmTCB;
TCB transportDistanceTCB;
TCB imageCaptureTCB;
TCB batteryTempTCB;

// Declare data structs
powerData pData;
solarData solData;
keyboardData kData;
vehicleData vData;
thrustData tData;
satData sData;
consoleData cData;
warnData wData;
transportData tranData;
imageData iData;
tempData temData;

// Declare the Task Queue
TQ q;
TaskQueue queue;

// Declare Global Counter
unsigned long GLOBALCOUNTER;
