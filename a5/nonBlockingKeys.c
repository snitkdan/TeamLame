#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <stdbool.h>
#include "nonBlockingKeys.h"
#include "scheduler.h"
#include "startup.h"

bool checkAll(char c) {
	return satVehicleCmd(c) ||
	       consoleModeCmd(c)||
		   motorSpeedCmd(c) ||
		   warningCmd(c) ||
		   payloadCmd(c) ||
		   piratesCmd(c) ||
		   miscCmd(c);
	
}

bool satVehicleCmd(char c) {
	return c == FORWARD         || 
	       c == BACK            || 
		   c == LEFT            || 
		   c == RIGHT           || 
		   c == DRILL_DOWN      || 
		   c == DRILL_UP        ||
		   c == START_IMAGE     ||
		   c == SEND_IMAGE;
}

extern TaskQueue queue;
extern TCB consoleDisplayTCB;
extern TCB solarPanelControlTCB;

bool consoleModeCmd(char c) {
	if (ContainsTCB(queue, &consoleDisplayTCB)) {
	    return c == SATELLITESTATUS || 
	           c == ANNUNCIATION;
    } else {
		return false; 
	}
}


bool motorSpeedCmd(char c) {
        //unsigned int taskQueue = NumTasksInTaskQueue(queue) + 1;
	if (ContainsTCB(queue, &solarPanelControlTCB)) {
		return c == SPEEDINC ||
		       c == SPEEDDEC;
	}
	return false;
}

bool warningCmd(char c) {
	return c == OVERHEAT_ACK;
}

bool payloadCmd(char c) {
	return c == THRUSTER ||
	       c == MEASURE;
}

bool piratesCmd(char c) {
	return c == PHASORS_BITCH ||
	       c == PHOTONS_BITCH;
}

bool miscCmd(char c) {
	return c == CLEAR; // for clear
}
