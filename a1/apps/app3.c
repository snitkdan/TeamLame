#include <stdio.h>  // for printf()
#include <stdbool.h>  // for bool data type

#define SIZE 4  // demo size for output string.

void delay(unsigned long aValue);
void undisplay(int size);
void display(char letters[], int size);

const unsigned long myDelay = 1000;

/*
  @modifies console window
  @effects
    flashes A C to the console at approx. 1 s delay,
    and B D to the console at approx 2 s delay.
*/
void main(void)
{
  char letters[SIZE] = "ABCD";
  char spaces[SIZE] = "    ";
  while(true)
  {
      for(int i = 0; i < SIZE; i++)
      {
  	    if(i == 0)
  	    {  // time == 0 sec
          spaces[0] = letters[0];
  	      spaces[2] = letters[2];
          spaces[1] = ' ';
          spaces[3] = ' ';
  	     }
      	else if (i == 2)
  	    {  // time == 3 sec
  	      spaces[0] = letters[0];
          spaces[1] = letters[1];
          spaces[2] = letters[2];
          spaces[3] = letters[3];
    	  }
        else
        {  // time == 2 or 4 sec
          spaces[0] = ' ';
          spaces[1] = ' ';
          spaces[2] = ' ';
          spaces[3] = ' ';
        }
      display(spaces, SIZE);  // print current output string
      printf("%c", 0x0d);  // return to the beginning of the console.
      delay(myDelay);  // 1 sec delay
      }
   }
}

/*
  @param aValue
    user specified delay
  @effects
    delays execution by
*/
void delay(unsigned long delay)
{
  volatile unsigned long i = 0;
  volatile unsigned int j = 0;
  for(i = delay; i > 0; i--)
  {
    for(j = 0; j < 100000; j++);
  }
}

/*
  @param letters
    array of letters to print
  @modifies console window
  @effects
    outputs all letters in the array
*/
void display(char letters[], int size)
{
  int i;
  for(i = 0; i < size; i++)
  {
    printf("%c", letters[i]);  // print a character
    fflush(stdout);  // add output to console
  }
}
