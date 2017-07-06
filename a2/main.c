#include <stdio.h>
#include "TCB.h"
#include "dataStructs.h"
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

void main(void)
{
    // Declare shared variables
    unsigned int thrusterCommand = 0;
    unsigned short fuelLvl = 100;
    bool solarPanelState = false;
    unsigned short pConsume = 0;
    unsigned short pGenerate = 0;
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

    // Allocate them structs for the taskDataPtr
    powerData *pData = (powerData*)malloc(sizeof(powerData));
    satData *sData = (satData*)malloc(sizeof(satData));
    thrustData *tData = (thrustData*)malloc(sizeof(thrustData));
    consoleData *cData = (consoleData*)malloc(sizeof(consoleData));
    warnData *wData = (warnData*)malloc(sizeof(warnData));

    // Assign shared variables to pointers
    pData->pGeneratePtr = &pGenerate;
    sData->pGeneratePtr = &pGenerate;
    cData->pGeneratePtr = &pGenerate;

    // Initialize the TCBs
    powerSubsystemTCB.taskDataPtr = (void*)pData;
    powerSubsystemTCB.myTask = powerSubsystem;
  
    // thrusterSubsystemTCB.taskDataPtr = (void*)thrustData;
    // thrusterSubsystemTCB.myTask = thrusterSubsystem;
    
    satelliteComsTCB.taskDataPtr = (void*)sData;
    satelliteComsTCB.myTask = satelliteComs;

    consoleDisplayTCB.taskDataPtr = (void*)cData;
    consoleDisplayTCB.myTask = consoleDisplay;
    //
    // warningAlarmTCB.taskDataPtr = (void*)warnData;
    // warningAlarmTCB.myTask = warningAlarm;

    // Initialize the task queue
    queue[0] = &powerSubsystemTCB;
    //queue[1] = &thrusterSubsystemTCB;
    queue[1] = &satelliteComsTCB;
    queue[2] = &consoleDisplayTCB;
    //queue[4] = &warningAlarmTCB;

    int i = 0;   // queue index
    while (true) 
    {
        aTCBPtr = queue[i];
	aTCBPtr->myTask((aTCBPtr->taskDataPtr));
	//i = (i + 1) % 5;  // cycles through queue
	i = (i + 1) % 3;
	usleep(500000);
    }	    
    return;
}    
