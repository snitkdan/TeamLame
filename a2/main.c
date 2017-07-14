#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include "TCB.h"
#include "dataStructs.h"

#define MAJOR_DELAY 9441.8
#define MINOR_DELAY 9920.2
unsigned long GLOBALCOUNTER = 0;

void main(void)
{
    // Define shared variables
    unsigned int thrusterCommand = 0;
    //unsigned short batteryLvl = 100;
    unsigned short batteryLvl = 1;	
    unsigned short fuelLvl = 11;
    unsigned short pConsume = 0;
    unsigned short pGenerate = 0;
    bool solarPanelState = false;
    bool fuelLow = false;
    bool batteryLow = false;

    // Defines a task queue
    // Note: only using 5, (the extra index will be used in future projects)
    TCB* queue[6];

    // Defines some TCBs
    TCB powerSubsystemTCB;
    TCB thrusterSubsystemTCB;
    TCB satelliteComsTCB;
    TCB consoleDisplayTCB;
    TCB warningAlarmTCB;

    // Defines a TCB pointer
    TCB* aTCBPtr;

    // Defines data structures
    powerData pData;
    thrustData tData;
    satData sData;
    consoleData cData;
    warnData wData;

    // 1. Turn off led0 initially
    FILE *led0 = fopen("/sys/class/leds/beaglebone:green:usr0/brightness", "w");
    FILE *led1 = fopen("/sys/class/leds/beaglebone:green:usr1/brightness", "w");
    FILE *led2 = fopen("/sys/class/leds/beaglebone:green:usr2/brightness", "w");
    FILE *led3 = fopen("/sys/class/leds/beaglebone:green:usr3/brightness", "w");

	checkOpened(led0); 
	checkOpened(led1);
	checkOpened(led2);
	checkOpened(led3);
	
    fprintf(led0, "%d", 0); fflush(led0); fclose(led0);
    fprintf(led1, "%d", 0); fflush(led1); fclose(led1);
    fprintf(led2, "%d", 0); fflush(led2); fclose(led2);
    fprintf(led3, "%d", 0); fflush(led3); fclose(led3);
	
    //.....................................
    //  Assign shared variables to pointers
    //.....................................
    // powerSubsystem
    pData.batteryLvlPtr = &batteryLvl;
    pData.solarPanelStatePtr = &solarPanelState;
    pData.pConsumePtr = &pConsume;
    pData.pGeneratePtr = &pGenerate;

    // thrusterSubsystem
    tData.thrusterCommandPtr = &thrusterCommand;
    tData.fuelLvlPtr = &fuelLvl;

    // satelliteComs
    sData.fuelLowPtr = &fuelLow;
    sData.batteryLowPtr = &batteryLow;
    sData.solarPanelStatePtr = &solarPanelState;
    sData.batteryLvlPtr = &batteryLvl;
    sData.fuelLvlPtr = &fuelLvl;
    sData.pConsumePtr = &pConsume;
    sData.pGeneratePtr = &pGenerate;
    sData.thrusterCommandPtr = &thrusterCommand;

    // consoleDisplay
    cData.fuelLowPtr = &fuelLow;
    cData.batteryLowPtr = &batteryLow;
    cData.solarPanelStatePtr = &solarPanelState;
    cData.batteryLvlPtr = &batteryLvl;
    cData.fuelLvlPtr = &fuelLvl;
    cData.pConsumePtr = &pConsume;
    cData.pGeneratePtr = &pGenerate;

    // warningAlarm
    wData.fuelLowPtr = &fuelLow;
    wData.batteryLowPtr = &batteryLow;
    wData.batteryLvlPtr = &batteryLvl;
    wData.fuelLvlPtr = &fuelLvl;

    // Initialize the TCBs
    powerSubsystemTCB.taskDataPtr = (void*)&pData;
    powerSubsystemTCB.myTask = powerSubsystem;

    thrusterSubsystemTCB.taskDataPtr = (void*)&tData;
    thrusterSubsystemTCB.myTask = thrusterSubsystem;

    satelliteComsTCB.taskDataPtr = (void*)&sData;
    satelliteComsTCB.myTask = satelliteComs;

    consoleDisplayTCB.taskDataPtr = (void*)&cData;
    consoleDisplayTCB.myTask = consoleDisplay;

    warningAlarmTCB.taskDataPtr = (void*)&wData;
    warningAlarmTCB.myTask = warningAlarm;

    // Initialize the task queue
    queue[0] = &warningAlarmTCB;
    queue[1] = &satelliteComsTCB;
    queue[2] = &thrusterSubsystemTCB;
    queue[3] = &powerSubsystemTCB;
    queue[4] = &consoleDisplayTCB;

    int i = 0;   // queue index
	// static int calls = 0;
    while (true) {
      aTCBPtr = queue[i];
      aTCBPtr->myTask((aTCBPtr->taskDataPtr));
      if(i == 4) {
        if(GLOBALCOUNTER % MAJOR_CYCLE == 0) {
          usleep(MAJOR_DELAY);
        } else {
          usleep(MINOR_DELAY);
        }
        GLOBALCOUNTER++;
      }
	    i = (i + 1) % 5;
    }
    return;
}
