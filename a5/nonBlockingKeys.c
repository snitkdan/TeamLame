#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <stdbool.h>
#include "nonBlockingKeys.h"
#include "scheduler.h"
#include "startup.h"

//kbhit, Non-blocking keypress detector, when go keypress, return 1 else always return 0
int kbhit() {
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    //FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds); //STDOUT_FILENO is 0
    select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &fds);
}

void nonblock(int state) {
    struct termios ttystate;
    //get the terminal state
    tcgetattr(STDIN_FILENO, &ttystate);
    if (state==NB_ENABLE) {
        //turn off canonical mode
        ttystate.c_lflag &= ~ICANON;
        //minimum of number input read.
        ttystate.c_cc[VMIN] = 1;
    }
    else if (state==NB_DISABLE) {
        //turn on canonical mode
        ttystate.c_lflag |= ICANON;
    }
    //set the terminal attributes.
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
}

bool satVehicleCmd(char c) {
	return c == FORWARD         || 
	       c == BACK            || 
		   c == LEFT            || 
		   c == RIGHT           || 
		   c == DRILL_DOWN      || 
		   c == DRILL_UP        ||
		   c == REQUEST_LIFTOFF || 
		   c == REQUEST_DOCK    ||
		   c == START_IMAGE     ||
		   c == SEND_IMAGE;
}

bool consoleModeCmd(char c) {
	return c == SATELLITESTATUS || 
	       c == ANNUNCIATION;
}

extern TaskQueue queue;
extern TCB solarPanelControlTCB;

bool motorSpeedCmd(char c) {
        //unsigned int taskQueue = NumTasksInTaskQueue(queue) + 1;
	if (ContainsTCB(queue, &solarPanelControlTCB)) {
		return c == SPEEDINC ||
		       c == SPEEDDEC;
	}
	return false;
        /* 
	return c == SPEEDINC ||
	       c == SPEEDDEC;
        */
}

bool warningCmd(char c) {
	return c == OVERHEAT_ACK;
}
