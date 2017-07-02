#include <stdio.h>
#include "TCB.h"
#include "dataStructs.h"
void main(void)
{
    // Defines a task queue 
    // Note: only using 5, (the extra index will be used in future projects)	
    TCB* queue[6];

    // Declare some TCBs
    TCB powerSubsystemData;
    TCB thrusterSubsystemData;
    TCB satelliteComsData;
    TCB consoleDisplayData;
    TCB warningAlarmData;

    // Declare a working TCB pointer (is this needed?)
    TCB* aTCBPtr;

    // Initialize the TCBs
    // PowerSubsystemData.taskDataPtr = (void*)&data;
    // PowerSubsystemData.taskPtr = someFunction;
    //
  
    // thrusterSubsystemData.taskDataPtr = (void*)&data;
    // thrusterSubsystemData.taskPtr = someFunction;
    
    // satelliteComsData.taskDataPtr = (void*)&data;
    // satelliteComsData.taskPtr = someFunction;

    // consoleDisplayData.taskDataPtr = (void*)&data;
    // consoleDisplayData.taskPtr = someFunction;
    //
    // warningAlarmData.taskDataPtr = (void*)&data;
    // warningAlarmData.taskPtr = someFunction;

    // Initialize the task queue
    queue[0] = &powerSubsystemData;
    queue[1] = &thrusterSubsystemData;
    queue[2] = &satelliteComsData;
    queue[3] = &consoleDisplayData;
    queue[4] = &warningAlarmData;

    return;
}    
