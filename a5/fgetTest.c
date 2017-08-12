#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#define SIZE 100
void test1();
void test2();

void main() {
     char pString[100];
     int i = 0;
     int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
     fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
     while (1) {
         test1();
         usleep(1000000);
         test2();
         usleep(1000000);
     }


}

void test1() {
    char pString[10];
    char buffer[100]; 
    if(fgets(pString, SIZE, stdin) != NULL) {
          printf("INBUF1 %s", pString); 
       // remove newline
       pString[strcspn(pString, "\n")] = 0;
       if (strstr(pString, "FUCK")) {
          printf("TEST1: %s", pString); 
       } else {
           int i;
           for (i = strlen(pString); i >= 0; i--) {
               ungetc(pString[i], stdin);
           }
       }  
    }
}

void test2() {
    char pString[100];
    if(fgets(pString, SIZE, stdin) != NULL) {
       pString[strcspn(pString, "\n")] = 0;
          printf("INBUF2 %s", pString); 
       // remove newline
       if (strstr(pString, "SHIT")) {
          printf("TEST2: %s", pString); 
       }  
    }
}
