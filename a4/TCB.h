// Declares the prototypes for the tasks

#ifndef HEAD_TCB
#define HEAD_TCB

// Declares a TCB structure
typedef struct myStruct {
  // Pointer to a function taking void* arg and returning a void
  void (*myTask)(void*);
  // Pointer to void used to reference the shared data for the task
  void *taskDataPtr;
  // Priority
  int priority;
  // Pointers to next/prev tasks in the task queue
	struct myStruct *next;
	struct myStruct *prev;
} TCB, *TCB_Ptr;

// Declares function prototypes
int terminalComs(char* output);
unsigned int randomInteger(unsigned int low, unsigned int high);
void powerSubsystem(void *powerStruct);
void solarPanelControl(void *solarStruct);
void keyboardConsole(void *keyboardStruct);
void vehicleComms(void *vehicleStruct);
void satelliteComs(void *satStruct);
void consoleDisplay(void *consoleStruct);
void thrusterSubsystem(void *thrustStruct);
void warningAlarm(void *warnStruct);
void transportDistance(void *tranportStruct);
void imageCapture(void *imageStruct);
void batteryTemp(void *temperatureStruct);

#endif
