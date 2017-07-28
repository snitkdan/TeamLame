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

#define P8_19 "P8_19"
#define ON 1
#define OFF 0

extern unsigned long GLOBALCOUNTER;

void main(void) {
    // Startup task
    Initialize();
    ActivateTimeBase();
    // Defines a TCB pointer
    extern TCB thrusterSubsystem;
    extern thrustData tData;
    // Run task in a loop
    int i = 0;
    while(i < 100) {
      thrusterSubsystem.myTask((void*)&tData);
      usleep(500000);
      i++;
    }
    // Turn off PWM
    setPWMProperty(P8_19, "run", OFF);



}
