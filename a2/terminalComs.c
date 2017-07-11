/*
 * Filename: TerminalComs.c (adapted)
 * Author: James K. Peckol
 * Contact: jkp@uw.washington.edu
 */

// http://man7.org/linux/man-pages/man2/open.2.html

#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include "TCB.h"

int terminalComs(char *satelliteStatus, char *annunciation) {
    FILE *fp;
    //char c[] = *satelliteStatus;
    char buffer[100];
    int fd0;
    int fd1;

    /*
     *   open file for reading and writing
     */
    fp = fopen("file.txt", "w+");

    /* open terminal ports for writing */
    fd0 = open("/dev/pts/1", O_WRONLY);
    fd1 = open("/dev/pts/0", O_WRONLY);

    /*
     *    write data to the file
     */
    //fwrite(c, strlen(c) +1, 1, fp);
    fwrite(satelliteStatus, strlen(satelliteStatus) +1, 1, fp);

    /*
     *  Seek to the beginning of the file
     */
     fseek(fp, SEEK_SET, 0);

    /*
     *    Read and print the data from the file
     */
     fread(buffer, 1, 99, fp);

    /*
     *   Append the buffer contents to a test string and transmit to terminal0
     *   Transmit a second test string to terminal1
     */
     dprintf(fd0, "%s", buffer);
     dprintf(fd1, "%s", annunciation);

     fclose(fp);
     remove("file.txt");
     return(0);
}
