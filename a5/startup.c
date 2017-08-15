/*
  startup.h - This file contains
  the implementation for the
  Startup task.
  (see startup.c for implementation)
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "TCB.h"
#include "dataStructs.h"
#include "warningAlarm.h"
#include "scheduler.h"
#include "startup.h"
#include "adc_utils.h"
#include "pwm_utils.h"

#define PWM_PIN "P9_14"

#define BUF_SIZE 16
#define OFF 0

void sigHandler(int sig);

// Define shared variables
// Thrusters
unsigned int thrusterCommand;
unsigned short fuelLvl;
// Power Subsystem
unsigned int *batteryLvl;
unsigned int batteryBuff[BUF_SIZE] = {100};
unsigned short pConsume;
unsigned short pGenerate;
// Battery Temp
unsigned int batteryTempBuff1[BUF_SIZE] = {100};
unsigned int batteryTempBuff2[BUF_SIZE] = {100};
unsigned int *batteryTmp1;
unsigned int *batteryTmp2;
unsigned int *distance;
unsigned int distanceBuff[8] = {1000};
signed int presentationBuffer[BUF_SIZE] = {0};
bool solarPanelState;
bool fuelLow;
bool batteryLow;
bool solarPanelDeploy;
bool solarPanelRetract;
bool motorInc;
bool motorDec;
bool batteryOverTemp;
char command;
char response;
char request;
int *processImage;

//Pirate
bool pirateDetected;
unsigned int pirateDistance;

//Command Parser
char received[20] = {'\0'};
char transmit[20] = {'\0'};
bool commandOn;
char ack[20] = {'\0'};
bool display;

// For signals
bool snapshot = false;
bool fromPowerSS = false;
bool fromSolar = false;
bool fromTransport = false;
bool stable = false;
bool endOfTravel = false;
bool adc_connection = false;
bool pwm_connection = false;

// Defines some TCBs
TCB powerSubsystemTCB;
TCB solarPanelControlTCB;
TCB keyboardConsoleTCB;
TCB vehicleCommsTCB;
TCB thrusterSubsystemTCB;
TCB satelliteComsTCB;
TCB consoleDisplayTCB;
TCB warningAlarmTCB;
TCB transportDistanceTCB;
TCB imageCaptureTCB;
TCB batteryTempTCB;
TCB commandParserTCB;
TCB pirateDetectionTCB;
TCB pirateManagementTCB;

// Defines data structures
powerData pData;
solarData solData;
keyboardData kData;
vehicleData vData;
thrustData tData;
satData sData;
consoleData cData;
warnData wData;
transportData tranData;
imageData iData;
tempData temData;
cmdData cmData;
dPirateData pdData;
mPirateData pmData;

// Define the Task Queue
TQ q;
TaskQueue queue;
// Define the Global Counter
unsigned long GLOBALCOUNTER;

void Initialize(void) {
  // 1. Assign initial values to shared variables
  thrusterCommand = 0;
  batteryLvl = &batteryBuff[0];

  // Battery Temperature
  batteryTmp1 = &batteryTempBuff1[0];
  batteryTmp2 = &batteryTempBuff2[0];

  fuelLvl = 100;
  pConsume = 0;
  pGenerate = 0;
  solarPanelState = false;
  fuelLow = false;
  batteryLow = false;
  solarPanelDeploy = false;
  solarPanelRetract = false;
  motorInc = false;
  motorDec = false;
  batteryOverTemp = false;
  command = '\0';
  response = '\0';
  request = '\0';
  distance = &distanceBuff[0];
  processImage = &presentationBuffer[0];
  pirateDetected = false;
  pirateDistance = 500;
  commandOn = false;
  display = false;

  // 2. Turn off led0 initially
  #define BEAGLEBONE
  #ifdef BEAGLEBONE
  FILE *led0 = fopen("/sys/class/leds/beaglebone:green:usr0/brightness", "w");
  FILE *led1 = fopen("/sys/class/leds/beaglebone:green:usr1/brightness", "w");
  FILE *led2 = fopen("/sys/class/leds/beaglebone:green:usr2/brightness", "w");
  FILE *led3 = fopen("/sys/class/leds/beaglebone:green:usr3/brightness", "w");
  if (!led0 || !led1 || !led2 || !led3) {
     fprintf(stderr, "BEAGLEBONE: Couldn't open led\n");
     return;
  } else {
     fprintf(led0, "%d", 0); fflush(led0); fclose(led0);
     fprintf(led1, "%d", 0); fflush(led1); fclose(led1);
     fprintf(led2, "%d", 0); fflush(led2); fclose(led2);
     fprintf(led3, "%d", 0); fflush(led3); fclose(led3);
  }
  // 2.1: Initialize the Hardware Perepherals
  #endif

  InitHardware();

  // 3. Assign shared variables to pointers
  // 3.1: powerSubsystem
  pData.solarPanelStatePtr = &solarPanelState;
  pData.solarPanelDeployPtr = &solarPanelDeploy;
  pData.solarPanelRetractPtr = &solarPanelRetract;
  pData.batteryLvlPtr = batteryLvl;
  pData.pConsumePtr = &pConsume;
  pData.pGeneratePtr = &pGenerate;
  // 3.2: solarPanelControl
  solData.solarPanelStatePtr = &solarPanelState;
  solData.solarPanelDeployPtr = &solarPanelDeploy;
  solData.solarPanelRetractPtr = &solarPanelRetract;
  solData.motorIncPtr = &motorInc;
  solData.motorDecPtr = &motorDec;
  // 3.3: keyboardConsole
  kData.motorIncPtr = &motorInc;
  kData.motorDecPtr = &motorDec;
  // 3.4: vehicleComms
  vData.commandPtr = &command;
  vData.responsePtr = &response;
  vData.requestPtr = &request; //address needed for strings and chars?
  // 3.5: thrusterSubsystem
  tData.thrusterCommandPtr = &thrusterCommand;
  tData.fuelLvlPtr = &fuelLvl;
  // 3.6: satelliteComs
  sData.fuelLowPtr = &fuelLow;
  sData.batteryLowPtr = &batteryLow;
  sData.solarPanelStatePtr = &solarPanelState;
  sData.batteryLvlPtr = batteryLvl;
  sData.fuelLvlPtr = &fuelLvl;
  sData.pConsumePtr = &pConsume;
  sData.pGeneratePtr = &pGenerate;
  sData.thrusterCommandPtr = &thrusterCommand;
  sData.commandPtr = &command;
  sData.responsePtr = &response;
  sData.batteryTmp1 = batteryTmp1;
  sData.batteryTmp2 = batteryTmp2;
  sData.distancePtr = distance;
  sData.requestPtr = &request;
  sData.processImagePtr = processImage;
  sData.received = received;
  sData.transmit = transmit;
  sData.commandOnPtr = &commandOn;
  sData.pirateDistancePtr = &pirateDistance;
  sData.ack = ack;
  sData.displayPtr = &display;  
  

  // 3.7: consoleDisplay
  cData.fuelLowPtr = &fuelLow;
  cData.batteryLowPtr = &batteryLow;
  cData.solarPanelStatePtr = &solarPanelState;
  cData.batteryLvlPtr = batteryLvl;
  cData.fuelLvlPtr = &fuelLvl;
  cData.pConsumePtr = &pConsume;
  cData.pGeneratePtr = &pGenerate;
  cData.distancePtr = distance;
  cData.batteryOverTempPtr = &batteryOverTemp;
  cData.batteryTmp1 = batteryTmp1;
  cData.batteryTmp2 = batteryTmp2;
  cData.pirateDistancePtr = &pirateDistance;

  // 3.8: warningAlarm
  wData.fuelLowPtr = &fuelLow;
  wData.batteryLowPtr = &batteryLow;
  wData.batteryLvlPtr = batteryLvl;
  wData.fuelLvlPtr = &fuelLvl;
  wData.batteryOverTempPtr = &batteryOverTemp;

  // 3.9: transportDistance
  tranData.distancePtr = distance;

  //3.10 imageCapture
  iData.processImagePtr = processImage;
  //3.11 batteryTemp
  temData.batteryOverTempPtr = &batteryOverTemp;
  temData.batteryTmp1 = batteryTmp1;
  temData.batteryTmp2 = batteryTmp2;
  //3.12 commandParser
  cmData.fuelLowPtr = &fuelLow;
  cmData.batteryLowPtr = &batteryLow;
  cmData.solarPanelStatePtr = &solarPanelState;
  cmData.batteryLvlPtr = batteryLvl;
  cmData.fuelLvlPtr = &fuelLvl;
  cmData.pConsumePtr = &pConsume;
  cmData.pGeneratePtr = &pGenerate;
  cmData.thrusterCommandPtr = &thrusterCommand;
  cmData.commandPtr = &command;
  cmData.responsePtr = &response;
  cmData.batteryTmp1 = batteryTmp1;
  cmData.batteryTmp2 = batteryTmp2;
  cmData.distancePtr = distance;
  cmData.requestPtr = &request;
  cmData.processImagePtr = processImage;
  cmData.received = received;
  cmData.transmit = transmit;
  cmData.commandOnPtr = &commandOn;
  cmData.ack = ack;
  cmData.displayPtr = &display;  
  


  //3.13 pirateDetection
  pdData.pirateDetectedPtr = &pirateDetected;
  pdData.pirateDistancePtr = &pirateDistance;

  //3.13 pirateManagement
  pmData.pirateDetectedPtr = &pirateDetected;
  pmData.pirateDistancePtr = &pirateDistance;

  // 4. Initialize the TCBs
  // 4.1: powerSubsystem
  powerSubsystemTCB.taskDataPtr = (void*)&pData;
  powerSubsystemTCB.myTask = powerSubsystem;
  powerSubsystemTCB.priority = 3;
  // 4.2: solarPanelControl
  solarPanelControlTCB.taskDataPtr = (void*)&solData;
  solarPanelControlTCB.myTask = solarPanelControl;
  solarPanelControlTCB.priority = 2;
  // 4.3: keyboardConsole
  keyboardConsoleTCB.taskDataPtr = (void*)&kData;
  keyboardConsoleTCB.myTask = keyboardConsole;
  keyboardConsoleTCB.priority = 2;
  // 4.4: vehicleComms
  vehicleCommsTCB.taskDataPtr = (void*)&vData;
  vehicleCommsTCB.myTask = vehicleComms;
  vehicleCommsTCB.priority = 3;
  // 4.5: thrusterSubsystem
  thrusterSubsystemTCB.taskDataPtr = (void*)&tData;
  thrusterSubsystemTCB.myTask = thrusterSubsystem;
  thrusterSubsystemTCB.priority = 3;
  // 4.6: satelliteComs
  satelliteComsTCB.taskDataPtr = (void*)&sData;
  satelliteComsTCB.myTask = satelliteComs;
  satelliteComsTCB.priority = 1;
  // 4.7: consoleDisplay
  consoleDisplayTCB.taskDataPtr = (void*)&cData;
  consoleDisplayTCB.myTask = consoleDisplay;
  satelliteComsTCB.priority = 3;
  // 4.8: warningAlarm
  warningAlarmTCB.taskDataPtr = (void*)&wData;
  warningAlarmTCB.myTask = warningAlarm;
  warningAlarmTCB.priority = 1;
   // 4.9: transportDistance
  transportDistanceTCB.taskDataPtr = (void*)&tranData;
  transportDistanceTCB.myTask = transportDistance;
  transportDistanceTCB.priority = 2;
  // 4.10: imageCapture
  imageCaptureTCB.taskDataPtr = (void*)&iData;
  imageCaptureTCB.myTask = imageCapture;
  imageCaptureTCB.priority = 2;
  // 4.11: batteryTemp
  batteryTempTCB.taskDataPtr = (void*)&temData;
  batteryTempTCB.myTask = batteryTemp;
  batteryTempTCB.priority = 2;
  // 4.12 commandParser
  commandParserTCB.taskDataPtr = (void*)&cmData;
  commandParserTCB.myTask = commandParser;
  commandParserTCB.priority = 1;

  // 4.13 pirateDetection
  pirateDetectionTCB.taskDataPtr = (void*)&pdData;
  pirateDetectionTCB.myTask = pirateDetection;
  pirateDetectionTCB.priority = 1;

  // 4.14 pirateManagement
  pirateManagementTCB.taskDataPtr = (void*)&pmData;
  pirateManagementTCB.myTask = pirateManagement;
  pirateManagementTCB.priority = 1;

  // 5. Initialize the task queue
  queue = &q;
  InitializeTaskQueue(queue);
  AppendTCB(queue, &warningAlarmTCB);
  AppendTCB(queue, &satelliteComsTCB);
  /*
  AppendTCB(queue, &thrusterSubsystemTCB);
  AppendTCB(queue, &powerSubsystemTCB);
  AppendTCB(queue, &consoleDisplayTCB);
  AppendTCB(queue, &vehicleCommsTCB);
  AppendTCB(queue, &pirateDetectionTCB);
  */
}

void ActivateTimeBase(void) {
  GLOBALCOUNTER = 0;
}

void InitHardware(void) {
  #ifdef BEAGLEBONE
  if(!initADC()) {
    fprintf(stderr, "ADC ERROR\n");
    exit(EXIT_FAILURE);
    return;
  }
  adc_connection = true;
  raise(SIGUSR1);
  if(!initPWM(PWM_PIN)) {
    fprintf(stderr, "PWM ERROR\n");
    exit(EXIT_FAILURE);
  }
  pwm_connection = true;
  raise(SIGUSR1);
  #endif
}

void sigHandler(int sig) {
  if (sig == SIGUSR1) {
    if(fromPowerSS) {
      // 1. Handle connection with ADC Channel (batteryLevel Measurement ON)
      // set stable = true (connected!)
      stable = true;
    }
    if(fromSolar) {
      // 2. Handle connection with solar panel output as well
      // a deployment sensor on the solar panel will generate a signaling
      // event to indicate end of travel. (endofTravel = true and read by solarPanelControl)
      endOfTravel = true;
    }
    if(fromTransport) {
      // RADLEIGH
	  //printf("fromTransport\n");
	  request = 'T';
	  printf("Received %c...\n", request);
	  vehicleComms((void*) &vData);
	  request = 'D';
	  printf("Received %c...\n", request);
	  vehicleComms((void*) &vData);
	  request = '\0';
	  snapshot = false;
    }
  }
  if (sig == SIGINT) {
    snapshot = true;
  }
  if(adc_connection) {
  	adc_connection = false;
  	printf("Successfully connected to ADC :P\n");
  }
  if(pwm_connection) {
  	pwm_connection = false;
  	printf("Successfully connected to PWM ;)\n");
  }
}
