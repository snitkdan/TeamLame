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

void Initialize(void) {
  // Define shared variables
  unsigned int thrusterCommand = 0;
  unsigned short batteryLvl = 100; // TO BE CHANGED LATER
  unsigned short fuelLvl = 100;
  unsigned short pConsume = 0;
  unsigned short pGenerate = 0;
  bool solarPanelState = false;
  bool fuelLow = false;
  bool batteryLow = false;

  // -- New Stuff -- //
  bool solarPanelDeploy = false;
  bool solarPanelRetract = false;
  bool motorInc = false;
  bool motorDec = false;
  char command = '\0';
  char response = '\0';

  // Defines some TCBs
  TCB powerSubsystemTCB;
  TCB solarPanelControlTCB;
  TCB keyboardConsoleTCB;
  TCB vehicleCommsTCB;
  TCB thrusterSubsystemTCB;
  TCB satelliteComsTCB;
  TCB consoleDisplayTCB;
  TCB warningAlarmTCB;

  // Defines data structures
  powerData pData;
  solarData solData;
  keyboardData kData;
  vehicleData vData;
  thrustData tData;
  satData sData;
  consoleData cData;
  warnData wData;

  #ifdef BEAGLEBONE
  // 1. Turn off led0 initially
  FILE *led0 = fopen("/sys/class/leds/beaglebone:green:usr0/brightness", "w");
  if (!led0) {
     fprintf(stderr, "MAIN: Couldn't open led0\n");
     return;
  } else {
     fprintf(led0, "%d", 0); fflush(led0); fclose(led0);
  }
  #endif

  //.....................................
  //  Assign shared variables to pointers
  //.....................................
  // powerSubsystem
  pData.solarPanelStatePtr = &solarPanelState;
  pData.solarPanelDeployPtr = &solarPanelDeploy;
  pData.solarPanelRetractPtr = &solarPanelRetract;
  pData.batteryLvlPtr = &batteryLvl; // TO BE CHANGED LATER
  pData.pConsumePtr = &pConsume;
  pData.pGeneratePtr = &pGenerate;

  //solarPanelControl
  solData.solarPanelStatePtr = &solarPanelState;
  solData.solarPanelDeployPtr = &solarPanelDeploy;
  solData.solarPanelRetractPtr = &solarPanelRetract;
  solData.motorIncPtr = &motorInc;
  solData.motorDecPtr = &motorDec;

  // keyboardConsole
  kData.motorIncPtr = &motorInc;
  kData.motorDecPtr = &motorDec;

  // vehicleComms
  vData.commandPtr = &command;
  vData.responsePtr = &response; //address needed for strings and chars?

  // thrusterSubsystem
  tData.thrusterCommandPtr = &thrusterCommand;
  tData.fuelLvlPtr = &fuelLvl;

  // satelliteComs
  sData.fuelLowPtr = &fuelLow;
  sData.batteryLowPtr = &batteryLow;
  sData.solarPanelStatePtr = &solarPanelState;
  sData.batteryLvlPtr = &batteryLvl; // TO BE CHANGED LATER
  sData.fuelLvlPtr = &fuelLvl;
  sData.pConsumePtr = &pConsume;
  sData.pGeneratePtr = &pGenerate;
  sData.thrusterCommandPtr = &thrusterCommand;
  sData.commandPtr = &command;
  sData.responsePtr = &response;

  // consoleDisplay
  cData.fuelLowPtr = &fuelLow;
  cData.batteryLowPtr = &batteryLow;
  cData.solarPanelStatePtr = &solarPanelState;
  cData.batteryLvlPtr = &batteryLvl; // TO BE CHANGED LATER
  cData.fuelLvlPtr = &fuelLvl;
  cData.pConsumePtr = &pConsume;
  cData.pGeneratePtr = &pGenerate;

  // warningAlarm
  wData.fuelLowPtr = &fuelLow;
  wData.batteryLowPtr = &batteryLow;
  wData.batteryLvlPtr = &batteryLvl; // TO BE CHANGED LATER
  wData.fuelLvlPtr = &fuelLvl;

  // Initialize the TCBs
  powerSubsystemTCB.taskDataPtr = (void*)&pData;
  powerSubsystemTCB.myTask = powerSubsystem;

  solarPanelControlTCB.taskDataPtr = (void*)&solData;
  solarPanelControlTCB.myTask = solarPanelControl;

  keyboardConsoleTCB.taskDataPtr = (void*)&kData;
  keyboardConsoleTCB.myTask = keyboardConsole;

  vehicleCommsTCB.taskDataPtr = (void*)&vData;
  vehicleCommsTCB.myTask = vehicleComms;

  thrusterSubsystemTCB.taskDataPtr = (void*)&tData;
  thrusterSubsystemTCB.myTask = thrusterSubsystem;

  satelliteComsTCB.taskDataPtr = (void*)&sData;
  satelliteComsTCB.myTask = satelliteComs;

  consoleDisplayTCB.taskDataPtr = (void*)&cData;
  consoleDisplayTCB.myTask = consoleDisplay;

  warningAlarmTCB.taskDataPtr = (void*)&wData;
  warningAlarmTCB.myTask = warningAlarm;

  // Initialize the task queue
  // Note: only using 5, (the extra index will be used in future projects)
  TaskQueue queue;
  AppendTCB(queue, &warningAlarmTCB);
  AppendTCB(queue, &satelliteComsTCB);
  AppendTCB(queue, &thrusterSubsystemTCB);
  AppendTCB(queue, &powerSubsystemTCB);
  AppendTCB(queue, &consoleDisplayTCB);
  AppendTCB(queue, &vehicleCommsTCB);
}

void ActivateTimeBase(void) {
  unsigned long GLOBALCOUNTER = 0;
}
