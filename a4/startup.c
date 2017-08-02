/*
  startup.h - This file contains
  the implementation for the
  Startup task.
  (see startup.c for implementation)
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "TCB.h"
#include "dataStructs.h"
#include "warningAlarm.h"
#include "scheduler.h"

#define BUF_SIZE 16


//#define COMMENTED
#ifdef COMMENTED
int terminalComs(char* output) {}
// unsigned int randomInteger(unsigned int low, unsigned int high) {}
void powerSubsystem(void *powerStruct) {}
void keyboardConsole(void *keyboardStruct) {}
void vehicleComms(void *vehicleStruct) {}
void satelliteComs(void *satStruct) {}
void consoleDisplay(void *consoleStruct) {}
void warningAlarm(void *warnStruct) {}
//void solarPanelControl(void *solarStruct) {}
//void thrusterSubsystem(void *thrustStruct) {}
#endif

// Define shared variables
unsigned int thrusterCommand;
unsigned int *batteryLvl;
unsigned int batteryBuff[BUF_SIZE] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
//unsigned int current_measurement = 0;
unsigned short fuelLvl;
unsigned short pConsume;
unsigned short pGenerate;
bool solarPanelState;
bool fuelLow;
bool batteryLow;
bool solarPanelDeploy;
bool solarPanelRetract;
bool motorInc;
bool motorDec;
char command;
char response;

// Defines some TCBs
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

// Defines data structures
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

// Define the Task Queue
TQ q;
TaskQueue queue;
// Define the Global Counter
unsigned long GLOBALCOUNTER;

void Initialize(void) {
  // 1. Assign initial values to shared variables
  thrusterCommand = 0;
  batteryLvl = &batteryBuff[0];
  fuelLvl = 100;
  pConsume = 0;
  pGenerate = 0;
  solarPanelState = false;
  fuelLow = false;
  batteryLow = false;
  solarPanelDeploy = false;
  solarPanelRetract = false;
  motorInc = false;
  motorDec = false;
  command = '\0';
  response = '\0';

  // 2. Turn off led0 initially
  #define BEAGLEBONE
  #ifdef BEAGLEBONE
  FILE *led0 = fopen("/sys/class/leds/beaglebone:green:usr0/brightness", "w");
  if (!led0) {
     fprintf(stderr, "MAIN: Couldn't open led0\n");
     return;
  } else {
     fprintf(led0, "%d", 0); fflush(led0); fclose(led0);
  }
  #endif

  // 3. Assign shared variables to pointers
  // 3.1: powerSubsystem
  pData.solarPanelStatePtr = &solarPanelState;
  pData.solarPanelDeployPtr = &solarPanelDeploy;
  pData.solarPanelRetractPtr = &solarPanelRetract;
  pData.batteryLvlPtr = batteryLvl;
  pData.pConsumePtr = &pConsume;
  pData.pGeneratePtr = &pGenerate;
  // 3.2: solarPanelControl
  solData.solarPanelStatePtr = &solarPanelState;
  solData.solarPanelDeployPtr = &solarPanelDeploy;
  solData.solarPanelRetractPtr = &solarPanelRetract;
  solData.motorIncPtr = &motorInc;
  solData.motorDecPtr = &motorDec;
  // 3.3: keyboardConsole
  kData.motorIncPtr = &motorInc;
  kData.motorDecPtr = &motorDec;
  // 3.4: vehicleComms
  vData.commandPtr = &command;
  vData.responsePtr = &response; //address needed for strings and chars?
  // 3.5: thrusterSubsystem
  tData.thrusterCommandPtr = &thrusterCommand;
  tData.fuelLvlPtr = &fuelLvl;
  // 3.6: satelliteComs
  sData.fuelLowPtr = &fuelLow;
  sData.batteryLowPtr = &batteryLow;
  sData.solarPanelStatePtr = &solarPanelState;
  sData.batteryLvlPtr = batteryLvl;
  sData.fuelLvlPtr = &fuelLvl;
  sData.pConsumePtr = &pConsume;
  sData.pGeneratePtr = &pGenerate;
  sData.thrusterCommandPtr = &thrusterCommand;
  sData.commandPtr = &command;
  sData.responsePtr = &response;
  // 3.7: consoleDisplay
  cData.fuelLowPtr = &fuelLow;
  cData.batteryLowPtr = &batteryLow;
  cData.solarPanelStatePtr = &solarPanelState;
  cData.batteryLvlPtr = batteryLvl;
  cData.fuelLvlPtr = &fuelLvl;
  cData.pConsumePtr = &pConsume;
  cData.pGeneratePtr = &pGenerate;
  // 3.8: warningAlarm
  wData.fuelLowPtr = &fuelLow;
  wData.batteryLowPtr = &batteryLow;
  wData.batteryLvlPtr = batteryLvl;
  wData.fuelLvlPtr = &fuelLvl;
  
  #ifdef INSERT_LAB4_DATA_HERE
  tranData.fooPtr = &foo;
  
  iData.fooPtr = &foo;
  temData.fooPtr = &foo;
  #endif
  
  // 4. Initialize the TCBs
  // 4.1: powerSubsystem
  powerSubsystemTCB.taskDataPtr = (void*)&pData;
  powerSubsystemTCB.myTask = powerSubsystem;
  // 4.2: solarPanelControl
  solarPanelControlTCB.taskDataPtr = (void*)&solData;
  solarPanelControlTCB.myTask = solarPanelControl;
  // 4.3: keyboardConsole
  keyboardConsoleTCB.taskDataPtr = (void*)&kData;
  keyboardConsoleTCB.myTask = keyboardConsole;
  // 4.4: vehicleComms
  vehicleCommsTCB.taskDataPtr = (void*)&vData;
  vehicleCommsTCB.myTask = vehicleComms;
  // 4.5: thrusterSubsystem
  thrusterSubsystemTCB.taskDataPtr = (void*)&tData;
  thrusterSubsystemTCB.myTask = thrusterSubsystem;
  // 4.6: satelliteComs
  satelliteComsTCB.taskDataPtr = (void*)&sData;
  satelliteComsTCB.myTask = satelliteComs;
  // 4.7: consoleDisplay
  consoleDisplayTCB.taskDataPtr = (void*)&cData;
  consoleDisplayTCB.myTask = consoleDisplay;
  // 4.8: warningAlarm
  warningAlarmTCB.taskDataPtr = (void*)&wData;
  warningAlarmTCB.myTask = warningAlarm;
   // 4.8: transportDistance
  transportDistanceTCB.taskDataPtr = (void*)&tranData;
  warningAlarmTCB.myTask = transportDistance; 
  // 4.8: imageCapture
  imageCaptureTCB.taskDataPtr = (void*)&iData;
  imageCaptureTCB.myTask = imageCapture;
    // 4.8: batteryTemp
  batteryTempTCB.taskDataPtr = (void*)&temData;
  batteryTempTCB.myTask = batteryTemp;
  
  // 5. Initialize the task queue
  queue = &q;
  InitializeTaskQueue(queue);
  AppendTCB(queue, &warningAlarmTCB);
  AppendTCB(queue, &keyboardConsoleTCB);
  AppendTCB(queue, &solarPanelControlTCB);
  AppendTCB(queue, &satelliteComsTCB);
  AppendTCB(queue, &thrusterSubsystemTCB);
  AppendTCB(queue, &powerSubsystemTCB);
  AppendTCB(queue, &consoleDisplayTCB);
  AppendTCB(queue, &vehicleCommsTCB);
}

void ActivateTimeBase(void) {
  GLOBALCOUNTER = 0;
}
