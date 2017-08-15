/*
 * commandParser.c - This file contains
 * the implementation of commandParser
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <signal.h>
#include <ctype.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "TCB.h"
#include "dataStructs.h"
#include "startup.h"
#include "scheduler.h"
#include "satComsParse.h"

extern bool warningBattTemp;

// CommandParser specific globals
extern char ack[3];

extern TaskQueue queue;
extern bool display;
extern void sigHandler(int sig);

#ifdef WHEN_YOURE_READY
// Adds measurement tasks. True if successful, false otherwise.
bool AddMeasureTasks();
// Removes measurement tasks. True if successful, false otherwise.
bool RemoveMeasureTasks();
// Returns true if the payload is valid for either thruster command
// or measurement, and false otherwise. (based on cmd)
bool isValidPayload(char cmd, char *payload);
// Returns true if test is a valid measurement command,
// and false otherwise.
bool isValidCommand(char test);
#endif

void maskBit(unsigned int *thrusterCommand) {
    // 0. Define a mask 1111111111110011
    uint16_t MASK = 0xFFF3;
    // 1. Mask the bit 2 and 3 to 0
    *thrusterCommand &= MASK;
}

void commandParser(void *cmdStruct) {
  // 1. Assign the data of cData into local variables
  cmdData *cData = (cmdData*)cmdStruct;
  char *received = cData->received;
  char *transmit = cData->transmit;
  unsigned int *thrusterCommand = cData->thrusterCommandPtr;
  bool *commandOn = cData->commandOnPtr;
  // 2. Parse the input
  char cmd = tolower(received[0]);  // e.g. 'M', 'T', 'D', etc.
  char *payload = &received[1];  // e.g. '12345', 'F' (for fuel level), etc
	printf("cmd = %c, payload = %s\n", cmd, payload);
	#ifdef WHEN_YOURE_READY
	switch(cmd) {
      case 't':
        // Thruster Command!
        if (isValidPayload(cmd, payload)) {
          ack[0] = 'A';
          *thrusterCommand = atoi(payload);
          maskBit(thrusterCommand);
        } else {
          ack[0] = 'E';
        }
        ack[2] = 'T';
        *transmit = '\0';
        break;
      case 'm':
        // Measurement Command!
        ack[0] = isValidPayload(cmd, payload) ? 'A' : 'E';
        ack[2] = 'M';
        *transmit = *payload;  // e.g. 'F' (fuel level), 'B' (battery level), etc
        break;
      case 's':
        // Start Command!
        ack[0] = (InitHardware() && AddMeasureTasks()) ? 'A' : 'E';
            // Attempt to initialize the PWM + ADC and add measurement tasks.
            // 'A' if successful, 'E' otherwise.
        ack[2] = 'S';
        *transmit = '\0';
        break;
      case 'p':
        // Pause Command!
        ack[0] = (CloseHardware() && RemoveMeasureTasks()) ? 'A' : 'E';
          // Attempt to terminate the PWM + ADC and remove measurement tasks.
          // 'A' if successful, 'E' otherwise.
        ack[2] = 'P';
        *transmit = '\0';
        break;
      case 'd':
        // Display Command
        ack[0] = 'A';  // always successful (?)
        ack[2] = 'D';
        *transmit = '\0';
        display = !display;  // see extern above
        // AppendTCB(queue, &consoleDisplayTCB); (?)
        break;
      default:
        // Bad command (e.g. user enters 'Z' <something>)
        ack[0] = 'E';
        ack[2] = 'X';
        *transmit = '\0';
        break;
    }
	  #endif
    *commandOn = false;
  }

  bool isValidPayload(char cmd, char *payload) {
    if (cmd == 't') {
      // Check thruster command
      unsigned int test = atoi(payload);
      return (test > 0 && test < 65536);
    } else {
      // Check measurement command
      if (strlen(payload) > 1) {
        return false;
      } else {
        char test = tolower(*payload);
        return isValidCommand(test);
      }
    }
  }

  bool AddMeasureTasks() {
    // 1. Enable data collecting interrupts
    signal(SIGINT, sigHandler);
    signal(SIGUSR1, sigHandler);
    // 2. Add measurement tasks to the task queue
    AppendTCB(queue, &thrusterSubsystemTCB);
    AppendTCB(queue, &powerSubsystemTCB);
    AppendTCB(queue, &vehicleCommsTCB);
    AppendTCB(queue, &pirateDetectionTCB);
    // 3. Initialize Hardware
    return true;
  }

  bool RemoveMeasureTasks() {
    // 1. Disable data collecting interrupts
    signal(SIGINT, SIG_DFL);
    signal(SIGUSR1, SIG_DFL);
    // 2. Add measurement tasks to the task queue
    RemoveTCB(queue, &thrusterSubsystemTCB);
    RemoveTCB(queue, &powerSubsystemTCB);
    RemoveTCB(queue, &vehicleCommsTCB);
    RemoveTCB(queue, &pirateDetectionTCB);
    return true;
  }

  bool isValidCommand(char test) {
    // t = batteryTemp, i = imageCapture, p = pirateDetection,
    // b = batteryLevel, s = solarPanelState, d = solarPanelDeploy,
    // r = solarPanelRetract, f = fuelLvl, o = transportDistance
    return (test == 't') ||
           (test == 'i') ||
           (test == 'p') ||
           (test == 'b') ||
           (test == 's') ||
           (test == 'd') ||
           (test == 'r') ||
           (test == 'f') ||
           (test == 'o');
  }
