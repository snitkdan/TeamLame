#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include "TCB.h"
#include "dataStructs.h"
#include "warningAlarm.h"

unsigned long GLOBALCOUNTER = 0;

#define BUF_SIZE 16

void main(void)
{
  // Define shared variables
  unsigned int thrusterCommand;
  unsigned int **batteryLvl;
  unsigned int batteryBuff[BUF_SIZE];
  unsigned int current_measurement = 0;
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
  // Defines data structures
  powerData pData;
  solarData solData;
  keyboardData kData;
  vehicleData vData;
  thrustData tData;
  satData sData;
  consoleData cData;
  warnData wData;
  // Define the Task Queue
  TQ q;
  TaskQueue queue;
  // Define the Global Counter
  unsigned long GLOBALCOUNTER;

  // Define pipe
  int fd0;

    // 1. Assign initial values to shared variables
    thrusterCommand = 0;
    batteryLvl = (unsigned int**)&batteryBuff;
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
    // Initialize the task queue
    queue[0] = &warningAlarmTCB;
    queue[1] = &satelliteComsTCB;
    queue[2] = &thrusterSubsystemTCB;
    queue[3] = &powerSubsystemTCB;
    queue[4] = &consoleDisplayTCB;
    queue[5] = &solarPanelControlTCB;
    queue[6] = &keyboardConsoleTCB;
    queue[7] = &vehicleCommsTCB;

    int i = 0;   // queue index
    int timeTask = 0;
    FILE *gpio = fopen("/sys/class/gpio/gpio48/value", "w");
    if (!gpio){
	     printf("Could not open GPIO\n");
	     return;
    }
    while (true) {
	     if(i == timeTask) {
		       fprintf(gpio, "%d", 1);
		       fflush(gpio);
	     }
       aTCBPtr = queue[i];
	     aTCBPtr->myTask((aTCBPtr->taskDataPtr));
	     if(i == timeTask) {
		       fprintf(gpio, "%d", 0);
           fflush(gpio);
	     }
	     i = (i + 1) % 8;
	     usleep(100000);
    }
    fclose(gpio);
    return;
}
