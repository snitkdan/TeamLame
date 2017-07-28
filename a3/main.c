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
#include "startup.h"
#include "scheduler.h"

extern unsigned long GLOBALCOUNTER;

void main(void) {
    // Extern for the queue
    extern TaskQueue queue;
    // Startup task
    Initialize();
    ActivateTimeBase();
    // Defines a TCB pointer
    TCB_Ptr aTCBPtr;
    // Queue index
	
	// make a pipe that can be seen by vehicleComs and vehicle
	// fork the two processes
	#ifdef TEST
	char * myfifo0 = "/tmp/myfifo0";   
	/* create the FIFO (named pipe) */
    mkfifo(myfifo0, 0666);
    /* open the FIFO */
    fd0 = open(myfifo0, O_RDWR);
    #endif	
	pid_t pid;
	pid = fork();
	if (pid == (pid_t)0) {
		while (1) {
			// vehicle
			char buf[10];
			int readState = read(fd0, buf, 10);
			if (readState > 0) {
				printf("In vehicle: received %s\n", buf);
			}
			printf("Here be the child process..........\n");
			usleep(10000);
		}
	} else if (pid < (pid_t) 0) {
		// Fork failed
		fprintf(stderr, "Fork failed\n");
		return EXIT_FAILURE;
	} else {
	#ifdef TEST
		int i = 0;
		while (true) {
		  aTCBPtr = PopTCB(queue);
		  aTCBPtr->myTask((aTCBPtr->taskDataPtr));
		  if(i == 5) {
			if(GLOBALCOUNTER % MAJOR_CYCLE == 0) {
			  usleep(9441.8);
			} else {
			  usleep(9465);
			}
			GLOBALCOUNTER++;
		  }
		  AppendTCB(queue, aTCBPtr);
			i = (i + 1) % 6;
		}
		return;
	#endif
	}
}
