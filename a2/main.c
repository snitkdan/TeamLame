#include <stdio.h>
#include "TCB.h"

void main(void)
{
    // Defines a task queue 
    // Note: only using 5, (the extra index will be used in future projects)	
    TCB* queue[6];

    // Declare some TCBs
    TCB powerSubsystem;
    TCB thrusterSubsystem;
    TCB satelliteComs;
    TCB consoleDisplay;
    TCB warningAlarm;

    // Declare a working TCB pointer
    TCB* aTCBPtr;

    // Initialize the TCBs
    // For example, managePower.taskPtr = someFunction;
    
    // Initialize the task queue
    queue[0] = powerSubsystem;
    queue[1] = thrusterSubsystem;
    queue[2] = &satelliteComs;
    queue[3] = &consoleDisplay;
    queue[4] = &warningAlarm;



    return;
}    
