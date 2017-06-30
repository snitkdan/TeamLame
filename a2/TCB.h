// Declares a TCB structure
struct myStruct
{
    void (*myTask)(void*);
    void *taskDataPtr;
};
typedef struct myStruct TCB;

// Emulates the Boolean type
enum myBool { FALSE = 0, TRUE = 1 };
typedef enum _myBool Bool;
