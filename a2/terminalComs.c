/*
 * Filename: TerminalComs.c (adapted)
 * Author: James K. Peckol
 * Contact: jkp@uw.washington.edu
 */

// http://man7.org/linux/man-pages/man2/open.2.html

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h> // for exit
#include "TCB.h"
#define MAX 1000

FILE *fp = NULL; // declare file here

int terminalComs(char *output) {
	// 1. Declare storage and data to be used
    char buffer[MAX];
    static int fd0; // for terminal0
    static int fd1; // for terminal1
	static int firstTime = 0;
	
    // 2. open file for reading and writing
	//    open terminal ports for writing
	if (firstTime == 0) {
		fp = fopen("file.txt", "w+");
        if(!fp) {
			exit(1);
		}		 
        fd0 = open("/dev/pts/1", O_WRONLY);
        fd1 = open("/dev/pts/0", O_WRONLY);
		dprintf(fd0, "\r");
		dprintf(fd1, "\r");
		firstTime++;
	}
	// 3. error handling: check if file and terminal ports exist 
    if (fp && fd0 && fd1) {
		
	    // 3.1 Write data to the file 
        fseek(fp, 0, SEEK_SET); // seeks pointer back to beginning for overwriting
        fwrite(output, strlen(output) + 1, 1, fp);

	    //3.2 Read and print the data from the file
	    fseek(fp, 0, SEEK_SET);	 
        fread(buffer, 1, MAX, fp);
	
        // 3.3 Transmit buffer to terminal0
        //     Transmit annunciation to terminal1
        dprintf(fd0, "%s\n", buffer);
        dprintf(fd1, "%s\n", output);
        //printf("%s\n", output);
		return (0);
	} else {
		fprintf(stderr, "ERROR, fp, fd0 and/or fd1 not opened correctly \n");
		exit(1);
	}
}
