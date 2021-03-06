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

#define WHEN_YOURE_READY

// CommandParser specific globals
extern TaskQueue queue;
extern void sigHandler(int sig);
extern TCB powerSubsystemTCB;
extern TCB solarPanelControlTCB;
extern TCB keyboardConsoleTCB;
extern TCB vehicleCommsTCB;
extern TCB thrusterSubsystemTCB;
extern TCB satelliteComsTCB;
extern TCB consoleDisplayTCB;
extern TCB warningAlarmTCB;
extern TCB transportDistanceTCB;
extern TCB imageCaptureTCB;
extern TCB batteryTempTCB;
extern TCB commandParserTCB;
extern TCB pirateDetectionTCB;
extern TCB pirateManagementTCB;


extern int fd0;
extern bool isPaused;
bool vehicleCommsInQueue;

// Adds measurement tasks. True if successful, false otherwise.
bool AddMeasureTasks();
// Removes measurement tasks. True if successful, false otherwise.
bool RemoveMeasureTasks();
// Returns true if the payload is valid for either thruster command
// or measurement, and false otherwise. (based on cmd)
bool isValidPayload(char cmd, char *payload);
// Returns true if test is a valid measurement command,
// and false otherwise.
bool isValidMeasurement(char test);
void userPrint(char c);



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
  char *ack = cData->ack;
  bool *display = cData->displayPtr;

  // 2. Parse the input
  char cmd = toupper(received[0]);  // e.g. 'M', 'T', 'D', etc.
  char *payload = &received[1];  // e.g. '12345', 'F' (for fuel level), etc
	ack[1] = ' ';
  ack[3] = '\0';
	#ifdef WHEN_YOURE_READY
	switch(cmd) {
      case THRUSTER:
        // Thruster Command!
        if (isValidPayload(cmd, payload)) {
          ack[0] = OK;
          *thrusterCommand = atoi(payload);
          maskBit(thrusterCommand);
        } else {
          ack[0] = ERR;
        }
        ack[2] = THRUSTER;
        *transmit = SHOW_EMPTY;
        break;
      case MEASURE:
        // Measurement Command!
        ack[0] = isValidPayload(cmd, payload) ? OK : ERR;
        ack[2] = MEASURE;
        *transmit = isValidPayload(cmd, payload) ? toupper(*payload) : ERR;  // e.g. 'F' (fuel level), 'B' (battery level), etc
		userPrint(*transmit);
		break;
      case START:
        // Start Command!
        ack[0] = AddMeasureTasks() ? OK : ERR;
        ack[2] = START;
        *transmit = SHOW_EMPTY;
        printf(GRN"Starting"RST" embedded tasks...\n");		
        break;
      case STOP:
        // Pause Command!
        ack[0] = RemoveMeasureTasks() ? OK : ERR;
        ack[2] = STOP;
        *transmit = NO_MEASURE;
        printf(RED"Stopping"RST" embedded tasks...\n");		
		
        break;
      case DISPLAY:
        // Display Command
        ack[0] = OK;  // always successful (?)
        ack[2] = DISPLAY;
        *transmit = SHOW_EMPTY;
        *display = !(*display);  // see extern above
        if (*display) {
          if (!ContainsTCB(queue, &consoleDisplayTCB)) {
            AppendTCB(queue, &consoleDisplayTCB);
          }
        } else {
          if (ContainsTCB(queue, &consoleDisplayTCB)) {
	          dprintf(fd0, "\033[2J");
	          dprintf(fd0, "\033[1;1H");
	          RemoveTCB(queue, &consoleDisplayTCB);
          }
        }
        printf("Turning on/off"MAG" Satellite Display...\n"RST);		
		
        break;
      default:
        // Bad command (e.g. user enters 'Z' or something)
        ack[0] = ERR;
        ack[2] = BAD_COMMAND;
        *transmit = SHOW_EMPTY;
        break;
    }
	  #endif
    *commandOn = false;
  }

  bool isValidPayload(char cmd, char *payload) {
    if (cmd == THRUSTER) {
      // Check thruster command
      unsigned int test = atoi(payload);
      return (test > 0 && test < 65536);
    } else {
      // Check measurement command
      if (strlen(payload) > 1) {
        return false;
      } else {
        char test = toupper(*payload);
        return isValidMeasurement(test);
      }
    }
  }

  bool AddMeasureTasks() {
    // 1. Enable data collecting interrupts
    if (signal(SIGINT, sigHandler) == SIG_ERR ||
        signal(SIGUSR1, sigHandler) == SIG_ERR) {
      return false;
    }
    // 2. Add measurement tasks to the task queue
	if (isPaused) {
    AppendTCB(queue, &thrusterSubsystemTCB);
    AppendTCB(queue, &powerSubsystemTCB);
    AppendTCB(queue, &vehicleCommsTCB);
	vehicleCommsInQueue = true;
    AppendTCB(queue, &pirateDetectionTCB);
	}
	isPaused = false;
    // 3. Initialize Hardware
    return true;
  }

  bool RemoveMeasureTasks() {
    // 1. Disable data collecting interrupts
     if (signal(SIGINT, SIG_DFL) == SIG_ERR ||
         signal(SIGUSR1, SIG_DFL) == SIG_ERR) {
       return false;
     }
    // 2. Add measurement tasks to the task queue
	if (!isPaused) {
    RemoveTCB(queue, &thrusterSubsystemTCB);
    RemoveTCB(queue, &solarPanelControlTCB);
    RemoveTCB(queue, &powerSubsystemTCB);
    RemoveTCB(queue, &keyboardConsoleTCB);
	RemoveTCB(queue, &vehicleCommsTCB);
	RemoveTCB(queue, &transportDistanceTCB);
	RemoveTCB(queue, &imageCaptureTCB);
	RemoveTCB(queue, &batteryTempTCB);
	RemoveTCB(queue, &pirateDetectionTCB);
	RemoveTCB(queue, &pirateManagementTCB);
	vehicleCommsInQueue = false;
	}
	isPaused = true;

    return true;
  }

bool isValidMeasurement(char test){
    return (test == SHOW_FUEL) ||
           (test == SHOW_BATT) ||
           (test == SHOW_PCON) ||
           (test == SHOW_TEMP) ||
           (test == SHOW_PANEL) ||
           (test == SHOW_DIST) ||
           (test == SHOW_IMAG) ||
           (test == SHOW_PIRATE);
  }
void userPrint(char c) {
	  switch(c) {
		case SHOW_FUEL:
            printf("Showing measurements for "CYN"fuel level...\n"RST);
			break;
		case SHOW_BATT:
            printf("Showing measurements for "CYN"battery level...\n"RST);
			break;			
		case SHOW_PCON:
            printf("Showing measurements for "CYN"power consumption...\n"RST);
			break;			
		case SHOW_TEMP:
            printf("Showing measurements for "CYN"battery temperature...\n"RST);
			break;			
		case SHOW_PANEL:
            printf("Showing measurements for "CYN"solar panels\n"RST);	
			break;			
		case SHOW_DIST:
            printf("Showing measurements for "CYN"transport distance...\n"RST);	
			break;			
		case SHOW_IMAG:
            printf("Showing measurements for "CYN"image frequency...\n"RST);
			break;			
		case SHOW_PIRATE:		  
            printf("Showing measurements for "CYN"dem alien piratez...\n"RST);	
			break;
	  }
}
