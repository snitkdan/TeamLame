#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "TCB.h"
#include "dataStructs.h"
#include "warningAlarm.h"

void pauseSec(double sec);
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
    LED led3 = {"/sys/class/leds/beaglebone:green:usr3/brightness", NULL, 0, true};
    LED *leds[3] = {&led1, &led2, &led3};

    wData.leds[0] = leds[0];
    wData.leds[1] = leds[1];
    wData.leds[2] = leds[2];

    // 2. Turn off all 3 LEDs
    /*char *command1 = strcat("echo 0 > ", led1.path);
    char *command2 = strcat("echo 0 > ", led2.path);
    char *command3 = strcat("echo 0 > ", led3.path);*/
    //system(command1);
    //system(command2);
    //system(command3);


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
    queue[0] = &warningAlarmTCB;
    queue[1] = &thrusterSubsystemTCB;
    queue[2] = &satelliteComsTCB;
    queue[3] = &powerSubsystemTCB;
    queue[4] = &consoleDisplayTCB;

    int i = 0;   // queue index
    time_t timeStart = 0;
    time_t timeEnd = 0;
    time_t timeElapsed = 0;
    unsigned long globalCounter = 0L;
    while (true) {
       if(timeElapsed % 5 == 0) {
           timeStart = time(time_t);
           aTCBPtr = queue[i];
           aTCBPtr->myTask((aTCBPtr->taskDataPtr));
           timeEnd = time(time_t);
           timeElapsed = timeEnd - timeStart;
           globalCounter++;
       }
       // LED Flashing
       if(led1.active || led2.active) {
         printf("LED3 OFF @ %d secs\n", led3.sec);
         //deactive(&led3);
       } else {
         printf("LED3 ON @ %d secs\n", led3.sec);
         //blink(&led3);
       }
       if(led1.active) {
         printf("LED1 ON @ %d secs\n", led1.sec);
         //blink(&led1);
       }
       if(led2.active) {
         printf("LED2 ON @ %d secs\n", led2.sec);
         //blink(&led2);
       }
       i = (i + 1) % 5;
       pauseSec(0.5);
    }
    return;
}

void blink(LED *led) {
  char *led_command = strcat("echo 1 > ", led->path);
  system(led_command);
  //pauseSec(led->sec);
}

void deactive(LED *led) {
  char *led_command = strcat("echo 0 > ", led->path);
  system(led_command);
}

// BROKEN (too slow)
void pauseSec(double sec) {
  time_t now, later;
  now = time(NULL);
  later = time(NULL);
  while((later - now) < sec) {
    later = time(NULL);
  }
}
