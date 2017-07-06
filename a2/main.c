#include <stdio.h>
//#include <stdbool.h>
#include <stdlib.h> // included for malloc
#include <unistd.h> // included for usleep
#include "TCB.h"
#include "dataStructs.h"
#include "Bool.h"
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

    // Allocate the structs for the taskDataPtr
    powerData *pData = (powerData*)malloc(sizeof(powerData));
    thrustData *tData = (thrustData*)malloc(sizeof(thrustData));
    satData *sData = (satData*)malloc(sizeof(satData));
    consoleData *cData = (consoleData*)malloc(sizeof(consoleData));
    warnData *wData = (warnData*)malloc(sizeof(warnData));

    //.....................................
    //  Assign shared variables to pointers
    //.....................................
    // powerSubsystem
    pData->batteryLvlPtr = &batteryLvl;
    pData->fuelLvlPtr = &fuelLvl;
    pData->pConsumePtr = &pConsume;
    pData->pGeneratePtr = &pGenerate;

    // thrusterSubsystem
    tData->thrusterCommandPtr = &thrusterCommand;
    tData->fuelLvlPtr = &fuelLvl;

    // satelliteComs
    sData->fuelLowPtr = &fuelLow;
    sData->batteryLowPtr = &batteryLow;
    sData->solarPanelStatePtr = &solarPanelState;
    sData->batteryLvlPtr = &batteryLvl;
    sData->fuelLvlPtr = &fuelLvl;
    sData->pConsumePtr = &pConsume;
    sData->pGeneratePtr = &pGenerate;
    sData->thrusterCommandPtr = &thrusterCommand;

    // consoleDisplay
    cData->fuelLowPtr = &fuelLow;
    cData->batteryLowPtr = &batteryLow;
    cData->solarPanelStatePtr = &solarPanelState;
    cData->batteryLvlPtr = &batteryLvl;
    cData->fuelLvlPtr = &fuelLvl;
    cData->pConsumePtr = &pConsume;
    cData->pGeneratePtr = &pGenerate;

    // warningAlarm
    wData->fuelLowPtr = &fuelLow;
    wData->batteryLowPtr = &batteryLow;
    wData->batteryLvlPtr = &batteryLvl;
    wData->fuelLvlPtr = &fuelLvl;

    // Initialize the TCBs
    powerSubsystemTCB.taskDataPtr = (void*)pData;
    powerSubsystemTCB.myTask = powerSubsystem;
  
    thrusterSubsystemTCB.taskDataPtr = (void*)tData;
    thrusterSubsystemTCB.myTask = thrusterSubsystem;
    
    satelliteComsTCB.taskDataPtr = (void*)sData;
    satelliteComsTCB.myTask = satelliteComs;

    consoleDisplayTCB.taskDataPtr = (void*)cData;
    consoleDisplayTCB.myTask = consoleDisplay;
    
    warningAlarmTCB.taskDataPtr = (void*)wData;
    warningAlarmTCB.myTask = warningAlarm;

    // Initialize the task queue
    queue[0] = &powerSubsystemTCB;
    queue[1] = &thrusterSubsystemTCB;
    queue[2] = &satelliteComsTCB;
    queue[3] = &consoleDisplayTCB;
    queue[4] = &warningAlarmTCB;

    int i = 0;   // queue index
    while (true) 
    {
        aTCBPtr = queue[i];
	aTCBPtr->myTask((aTCBPtr->taskDataPtr));
	i = (i + 1) % 5;  // cycles through queue
	usleep(500000);
    }	    
    return;
}    
