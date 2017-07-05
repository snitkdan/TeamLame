#include <stdio.h>
#include "TCB.h"
#include "dataStructs.h"
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

void main(void)
{
    // Declare shared variables
    unsigned int thrusterCommand;
    unsigned short fuelLvl;
    bool solarPanelState;
    unsigned short pConsume;
    unsigned short pGenerate = 500;
    bool fuelLow;
    bool batteryLow;

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

    // Allocate them structs
    powerData *pData = (powerData*)malloc(sizeof(powerData));
    satData *sData = (satData*)malloc(sizeof(satData));

    // Initialize the TCBs
    powerSubsystemTCB.taskDataPtr = (void*)pData;
    pData->pGeneratePtr = &pGenerate;
    powerSubsystemTCB.myTask = powerSubsystem;
  
    // thrusterSubsystemTCB.taskDataPtr = (void*)&thrustData;
    // thrusterSubsystemTCB.taskPtr = thrusterSubsystem;
    
    satelliteComsTCB.taskDataPtr = (void*)sData;
    sData->pGeneratePtr = &pGenerate;
    satelliteComsTCB.myTask = satelliteComs;

    // consoleDisplayTCB.taskDataPtr = (void*)&consoleData;
    // consoleDisplayTCB.taskPtr = consoleDisplay;
    //
    // warningAlarmTCB.taskDataPtr = (void*)&warnData;
    // warningAlarmTCB.taskPtr = warningAlarm;

    // Initialize the task queue
    queue[0] = &powerSubsystemTCB;
    //queue[1] = &thrusterSubsystemTCB;
    queue[1] = &satelliteComsTCB;
    //queue[3] = &consoleDisplayTCB;
    //queue[4] = &warningAlarmTCB;

    int i = 0;   // queue index
    while (true) 
    {
        aTCBPtr = queue[i];
	aTCBPtr->myTask((aTCBPtr->taskDataPtr));
	//i = (i + 1) % 5;  // cycles through queue
	i = (i + 1) % 2;
	usleep(500000);
    }	    
    return;
}    
