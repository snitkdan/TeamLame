// http://man7.org/linux/man-pages/man2/open.2.html

#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
int main(void)
{
    FILE *fp;										//  working file
    char c[] = "text here...";						//  test string
    char buffer[100];								//  working buffer			

    int fd0;										//  device / terminal0
    int fd1;										//  device / terminal1

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
    fwrite(c, strlen(c) +1, 1, fp);

    /*  
     *  Seek to the beginning of the file 
     */
    fseek(fp, SEEK_SET, 0);

   
			   /* 
    *    Read and print the data from the file 
    */
   fread(buffer, 1, 99, fp);
   printf("%s\n", buffer);

   
    /*
     *   Append the buffer contents to a test string and transmit to terminal0
     *   Transmit a second test string to terminal1
     *   Delay for 2 seconds
     */ 
    while (1) 
    {
        dprintf(fd0, "If you see %s\n", buffer);
        dprintf(fd1, "...and here, the terminal comm is linked\n");
        sleep(2);
    }

    fclose(fp);
    return(0);
}
