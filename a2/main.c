#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
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

    // 2. Turn off all 3 LEDs
    char *command = strcat("echo 0 > ", led1.path);
    system(command);

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
  char *led_command = strcat("echo 1 > ", led->path);
  system(led_command);
  pauseSec(led->sec);
}

void deactive(LED *led) {
  char *led_command = strcat("echo 0 > ", led->path);
  system(led_command);
}

void pauseSec(int sec) {
  time_t now, later;
  now = time(NULL);
  later = time(NULL);
  while((later - now) < (double)sec) {
    later = time(NULL);
  }
}
