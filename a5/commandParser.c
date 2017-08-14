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

bool InitHardware();
void CloseHardware();
bool AddMeasureTasks();
void RemoveMeasureTasks();

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
        ack[0] = isValidPayload(payload) ? 'A' : 'E';
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





    AppendTCB(queue, &thrusterSubsystemTCB);
    AppendTCB(queue, &powerSubsystemTCB);
    AppendTCB(queue, &consoleDisplayTCB);
    AppendTCB(queue, &vehicleCommsTCB);
}
