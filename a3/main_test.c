#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "TCB.h"
#include "dataStructs.h"
#include "warningAlarm.h"
#include "pwm_utils.h"
#include "startup.h"
#include<stdint.h>


#define MAX 65536

#define P8_19 "P8_19"
#define HNUM_19 18
#define ON 1
#define OFF 0

extern unsigned long GLOBALCOUNTER;

void main(void) {
    // Startup task
    Initialize();
    ActivateTimeBase();
    // Defines a TCB pointer
    extern TCB thrusterSubsystemTCB;
    extern thrustData tData;
    extern unsigned int thrusterCommand;
    // Run task in a loop
    int i = 0;
    while(i < 100) {
      thrusterCommand = randomInteger(0, MAX) % MAX;
      uint16_t MASK = 0xFFF3;
      thrusterCommand &= MASK;
      thrusterSubsystemTCB.myTask((void*)&tData);
      usleep(5000000);
      i++;
    }
    // Turn off PWM
    setPWMProperty(P8_19, "run", OFF, HNUM_19);



}
