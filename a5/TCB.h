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
int terminalComs(char mode, char* output);
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
void commandParser(void *cmdStruct);
void pirateDetection(void *pdStruct);
void pirateManagement(void *pmStruct);

#define BLK   "\x1B[30m"
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define BOLD  "\x1B[1m"
#define ULINE "\x1B[4m"
#define RST   "\x1B[0m"

#define BLK_BG  "\x1B[40m"
#define RED_BG  "\x1B[41m"
#define GRN_BG  "\x1B[42m"
#define YEL_BG  "\x1B[43m"
#define BLU_BG  "\x1B[44m"
#define MAG_BG  "\x1B[45m"
#define CYN_BG  "\x1B[46m"
#define WHT_BG  "\x1B[47m"
#endif
