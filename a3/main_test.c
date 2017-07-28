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

#define PIN "P8_13"
#define ON 1
#define OFF 0

extern unsigned long GLOBALCOUNTER;

void main(void) {
    // Startup task
    Initialize();
    ActivateTimeBase();
    // Defines a TCB pointer
    extern TCB solarPanelControlTCB;
    extern solarData solData;
    *solData.motorIncPtr = true;
    // Run task in a loop
    int i = 0;
    while(i < 3) {
      solarPanelControlTCB.myTask((void*)solData);
      usleep(5000000)
      i++;
    }
    // Turn off PWM
    setPWMProperty(PIN, "run", OFF);



}
