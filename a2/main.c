#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "TCB.h"
#include "dataStructs.h"
#include "warningAlarm.h"
#include <time.h>

void pauseSec(int sec);
void deactive(LED *led);
void blink(LED *led);

void main(void) {
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

    // 1. Declare structures to store LED metadata
    LED led1 = {"/sys/class/leds/beaglebone:green:usr1/brightness", NULL, 0, false};
    LED led2 = {"/sys/class/leds/beaglebone:green:usr2/brightness", NULL, 0, false};
    LED led3 = {"/sys/class/leds/beaglebone:green:usr3/brightness", NULL, 0, false};
    LED *leds[3] = {&led1, &led2, &led3};

    wData.leds[0] = leds[0];
    wData.leds[1] = leds[1];
    wData.leds[2] = leds[2];

    // 2. Turn off all 3 LEDs
    char *echoCommand = "echo 0 > ";
    char command1[strlen(echoCommand) + strlen(led1.path) + 1]; 
    char command2[strlen(echoCommand) + strlen(led2.path) + 1]; 
    char command3[strlen(echoCommand) + strlen(led3.path) + 1]; 
    sprintf(command1, "%s%s", echoCommand, led1.path);
    sprintf(command2, "%s%s", echoCommand, led2.path);
    sprintf(command3, "%s%s", echoCommand, led3.path);
    
//    strcat(command1, echoCommand);
//    strcat(command2, echoCommand);
//    strcat(command3, echoCommand);
//
//    strcat(command1, led1.path);
//    strcat(command2, led2.path);
//    strcat(command3, led3.path);
//
    system(command1);
    system(command2);
    system(command3);
    system("echo 0 > /sys/class/leds/beaglebone:green:usr0/brightness");

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
    queue[3] = &powerSubsystemTCB;
    queue[2] = &thrusterSubsystemTCB;
    queue[1] = &satelliteComsTCB;
    queue[4] = &consoleDisplayTCB;
    queue[0] = &warningAlarmTCB;

    int i = 0;   // queue index
    while (true) {
       aTCBPtr = queue[i];
	     aTCBPtr->myTask((aTCBPtr->taskDataPtr));
	     i = (i + 1) % 5;
       // Fuel Lvl Status
       if(led1.active) {
         blink(&led1);
       } else {
         deactive(&led1);
       }
       // Battery Lvl Status
       if(led2.active) {
         blink(&led2);
       } else {
         deactive(&led2);
       }
       // Okay
       if(led3.active) {
         blink(&led3);
       } else {
         deactive(&led3);
       }
    }
    return;
}

void blink(LED *led) {
    char *echoCommand = "echo 1 > ";
    char ledCommand[strlen(echoCommand) + strlen(led->path)];
    sprintf(ledCommand, "%s%s", echoCommand, led->path);
   // strcat(ledCommand, echoCommand);
   // strcat(ledCommand, led->path);
    system(ledCommand);
    deactive(led);
    pauseSec(led->sec);
}

void deactive(LED *led) {
    char *echoCommand = "echo 0 > ";
    char ledCommand[strlen(echoCommand) + strlen(led->path)];
    sprintf(ledCommand, "%s%s", echoCommand, led->path);
    system(ledCommand);
}

void pauseSec(int sec) {
  time_t now, later;
  now = time(NULL);
  later = time(NULL);
  while((later - now) < (double)sec) {
    later = time(NULL);
  }
}
