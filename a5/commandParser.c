/*
 * commandParser.c - This file contains
 * the implementation of commandParser
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "TCB.h"
#include "dataStructs.h"

extern bool warningBattTemp;

// CommandParser specific globals
extern char ack[3];

extern bool commandOn;
extern unsigned int thrusterCommand;

// Initializes Hardware components. True if successful, false otherwise.
bool InitHardware();
// Terminates hardware components. True if successful, false otherwise.
bool CloseHardware();
// Adds measurement tasks. True if successful, false otherwise.
bool AddMeasureTasks();
// Removes measurement tasks. True if successful, false otherwise.
bool RemoveMeasureTasks();

// Returns true if the payload is valid for either thruster command
// or measurement, and false otherwise.
bool isValidPayload(char *payload);


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
    // 2. Parse the input
    char cmd = tolower(input[0]);
    char *payload = &input[2];
    switch(cmd) {
      case 't':
        if (isValidPayload(payload)) {
          ack[0] = 'A';
          maskBit(&thrusterCommand);
        } else {
          ack[0] = 'E';
        }
        ack[2] = 'T';
        *transmit = '\0';
        break;
      case 'm':
        ack[0] = isValidPayload(payload) ? 'A' : 'E';
        ack[2] = 'M';
        *transmit = *payload;
        break;
      case 's':
        ack[0] = (InitHardware() && AddMeasureTasks()) ? 'A' : 'E';
        ack[2] = 'S';
        *transmit = '\0';
        break;
      case 'p':
        ack[0] = (CloseHardware() && RemoveMeasureTasks()) ? 'A' : 'E';
        ack[2] = 'P';
        *transmit = '\0';
        break;
      case 'd':
        ack[0] = 'A';
        ack[2] = 'D';
        *transmit = '\0';
        display = !display;
        break;
      default:
        ack[0] = 'E';
        ack[2] = 'X';
        *transmit = '\0';
        break;
    }
  }
