#include <stdio.h>
#include "TCB.h"
#include "dataStructs.h"
#include "Bool.h"
#include <stdbool.h>
#define TRUE 1


void main(void)
{
    // Declare shared variables
    unsigned int thrusterCommand;
    unsigned short fuelLvl;
    bool solarPanelState;
    unsigned short pConsume;
    unsigned short pGenerate = 0;
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

    // Initialize the TCBs
    // powerSubsystemTCB.taskDataPtr = (void*)&powerData;
    powerData->pGeneratePtr = &pGenerate;
    // powerSubsystemTCB.taskPtr = powerSubsystem;
  
    // thrusterSubsystemTCB.taskDataPtr = (void*)&thrustData;
    // thrusterSubsystemTCB.taskPtr = thrusterSubsystem;
    
    // satelliteComsTCB.taskDataPtr = (void*)&satData;
    // satelliteComsTCB.taskPtr = satelliteComs;

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
	aTCBPtr->taskPtr((aTCBPtr->taskDataPtr));
	//i = (i + 1) % 5;  // cycles through queue
	i = (i + 1) % 2;
    }	    
    return;
}    
