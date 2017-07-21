/*
A FIFO special file is similar to a pipe, except that it is created in a different way. 
Instead of being an anonymous communications channel, a FIFO special file is entered into 
the file system by calling mkfifo.

Once you have created a FIFO special file in this way, any process can open it for 
reading or writing, in the same way as an ordinary file. However, it has to be open at 
both ends simultaneously before you can proceed to do any input or output operations 
on it. Opening a FIFO for reading normally blocks until some other process opens the 
same FIFO for writing, and vice versa.

The mkfifo function is declared in the header file sys/stat.h.

Function: int mkfifo (const char *filename, mode_t mode)

    Preliminary: | MT-Safe | AS-Safe | AC-Safe | See POSIX Safety Concepts.

    The mkfifo function makes a FIFO special file with name filename. The mode argument is 
	used to set the file’s permissions; see Setting Permissions.

    The normal, successful return value from mkfifo is 0. In the case of an error, -1 
	is returned. In addition to the usual file name errors (see File Name Errors), the 
	following errno error conditions are defined for this function:

    EEXIST

        The named file already exists.
    ENOSPC

        The directory or file system cannot be extended.
    EROFS

        The directory that would contain the file resides on a read-only file system. 

*/


//  rw0.c
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_BUF 1024

typedef enum {wrt, rd} rw;

int fd0;    
char buf[MAX_BUF];

void funct0(int fd0, rw coms);
void funct1(int fd0);

int main()
{
    char * myfifo0 = "/tmp/myfifo0";
	    
	/* create the FIFO (named pipe) */
    mkfifo(myfifo0, 0666);

    /* open the FIFO */
	
    fd0 = open(myfifo0, O_RDWR);
	
	funct0(fd0, wrt);		//  write to fifo
	funct1(fd0);			//  read then write to the fifo
	funct0(fd0, rd);		//  read fromn the fifo
	
    close(fd0);

    /* remove the FIFO */
    //unlink(myfifo0);

    return 0;
}

/*
 *  Test function0
 */
 
 //  test write then read back
 
void funct0(int fd0, rw coms)
{	char buf[MAX_BUF];
	int size = 0;
	
	//  testing
	printf("in funct0\n");
	
	// check for write or read
	switch(coms)
	{
		case wrt: 
		{
			printf("send command\n");
			size = sizeof("Launch Rocket");
			write(fd0, "Launch Rocket", size);
			break;
		}
		case rd:
		{
			printf("receive response\n");
			size = sizeof("Roger That\n");
			read(fd0, buf, MAX_BUF);
			printf("Received: %s\n", buf);
			break;
		}
	}

	return;
}
	







