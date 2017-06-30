// Declares the prototypes for the tasks

#ifndef HEAD_TCB
#define HEAD_TCB

// function prototypes
// --- Insert here ---


// Declares a TCB structure
struct myStruct
{
    // Pointer to a function taking void* arg and returning a void	
    void (*myTask)(void*);

    // Pointer to void used to reference the shared data for the task
    void *taskDataPtr;
};
typedef struct myStruct TCB;

// Enumerates the Boolean type
enum myBool { FALSE = 0, TRUE = 1 };
typedef enum _myBool Bool;

#endif
