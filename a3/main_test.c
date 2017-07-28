#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdint.h>
#include "TCB.h"
#include "dataStructs.h"
#include "warningAlarm.h"
#include "pwm_utils.h"
#include "startup.h"

#define MAX 65536

#define ON 1
#define OFF 0
#define BUF_SIZE 16

extern unsigned long GLOBALCOUNTER;

void main(void) {
    // Startup task
    Initialize();
    ActivateTimeBase();
    // Defines a TCB pointer
    extern TCB powerSubsystemTCB;
    extern int current_measurement;
    extern unsigned int batteryBuff[BUF_SIZE];
    /* PWM
    extern TCB thrusterSubsystemTCB;
    extern thrustData tData;
    extern unsigned int thrusterCommand;
    extern TCB solarPanelControlTCB;
    extern solarData solData;
    *solData.motorIncPtr = true;
    */
    // Run task in a loop
    int i = 0;
    while(i < 100) {
      /*thrusterCommand = randomInteger(0, MAX) % MAX;
      uint16_t MASK = 0xFFF3;
      thrusterCommand &= MASK;
      thrusterSubsystemTCB.myTask((void*)&tData);
      solarPanelControlTCB.myTask((void*)&solData);*/
      powerSubsystemTCB.myTask(powerSubsystemTCB.taskDataPtr);
      printf("Current measurement: %d\n", batteryBuff[current_measurement]);
      usleep(4000000);
      i++;
    }
    // Turn off PWM
    //setPWMProperty(P8_13, "run", OFF, HNUM_13);
    //setPWMProperty(P8_19, "run", OFF, HNUM_19);
}
