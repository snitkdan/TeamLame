// Declares the prototypes for the tasks

#ifndef HEAD_TCB
#define HEAD_TCB

// Declares a TCB structure
struct myStruct
{
    // Pointer to a function taking void* arg and returning a void
    void (*myTask)(void*);

    // Pointer to void used to reference the shared data for the task
    void *taskDataPtr;
	
	struct myStruct* next;
	struct myStruct* prev;
};
typedef struct myStruct TCB;

// Declares function prototypes
//int terminalComs(char* output);
unsigned int randomInteger(unsigned int low, unsigned int high);
void powerSubsystem(void *powerStruct);
void solarPanelControl(void *solarStruct);
void keyboardConsole(void *keyboardStruct);
void vehicleComms(void *vehicleStruct);
void satelliteComs(void *satStruct);
void consoleDisplay(void *consoleStruct);
void thrusterSubsystem(void *thrustStruct);
void warningAlarm(void *warnStruct);
#endif
