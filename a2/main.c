#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include "TCB.h"
#include "dataStructs.h"
#include "warningAlarm.h"

unsigned long GLOBALCOUNTER = 0; 

void main(void)
{
    // Define shared variables
    unsigned int thrusterCommand = 0;
    unsigned short batteryLvl = 100;
    unsigned short fuelLvl = 100;
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

    // 1. Turn off all lights initially
	FILE *led0 = fopen("/sys/class/leds/beaglebone:green:usr0/brightness", "w");
	FILE *led1 = fopen("/sys/class/leds/beaglebone:green:usr1/brightness", "w");
	FILE *led2 = fopen("/sys/class/leds/beaglebone:green:usr2/brightness", "w");
	FILE *led3 = fopen("/sys/class/leds/beaglebone:green:usr3/brightness", "w");
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
    queue[0] = &powerSubsystemTCB;
    queue[1] = &thrusterSubsystemTCB;
    queue[2] = &satelliteComsTCB;
    queue[3] = &consoleDisplayTCB;
    queue[4] = &warningAlarmTCB;

    int i = 0;   // queue index
	// static int calls = 0;
    while (true)
    {
        aTCBPtr = queue[i];
	    aTCBPtr->myTask((aTCBPtr->taskDataPtr));
	    i = (i + 1) % 5;
	    GLOBALCOUNTER++; 
		usleep(100000);
        		
    }
    return;
}
