#include <stdio.h>
#include "TCB.h"
#include "dataStructs.h"
#define TRUE 1
void main(void)
{
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
    // powerSubsystemTCB.taskDataPtr = (void*)&powerSubsystemData;
    // powerSubsystemTCB.taskPtr = powerSubsystem;
  
    // thrusterSubsystemTCB.taskDataPtr = (void*)&thrusterSubsystemData;
    // thrusterSubsystemTCB.taskPtr = thrusterSubsystem;
    
    // satelliteComsTCB.taskDataPtr = (void*)&satelliteComsData;
    // satelliteComsTCB.taskPtr = satelliteComs;

    // consoleDisplayTCB.taskDataPtr = (void*)&consoleDisplayData;
    // consoleDisplayTCB.taskPtr = consoleDisplay;
    //
    // warningAlarmTCB.taskDataPtr = (void*)&warningAlarmData;
    // warningAlarmTCB.taskPtr = warningAlarm;

    // Initialize the task queue
    queue[0] = &powerSubsystemTCB;
    queue[1] = &thrusterSubsystemTCB;
    queue[2] = &satelliteComsTCB;
    queue[3] = &consoleDisplayTCB;
    queue[4] = &warningAlarmTCB;

    int i = 0;   // queue index
    while (TRUE) 
    {
        aTCBPtr = queue[i];
	aTCBPtr->taskPtr((aTCBPtr->taskDataPtr));
	i = (i+1)%5;  // cycles through queue
    }	    
    return;
}    
