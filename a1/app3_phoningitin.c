#include <stdio.h>

#define TRUE 1
#define FALSE 0
#define SIZE 4

void delay(unsigned long aValue);
void undisplay(int size);
void display(char letters[], int size);

const unsigned long myDelay = 5000;

/*
  @modifies console window
  @effects
    flashes A B C D to the console at approx. 1 s delay.
*/
void main(void)
{
  char letters[SIZE] = "ABCD";
  char spaces[SIZE] = "    ";
  while(TRUE)
  {
      for(int i = 0; i < SIZE; i++)
      {
	  if(i == 0)
	  {
              spaces[0] = letters[0];
	      spaces[2] = letters[2]; 
              spaces[1] = ' ';
              spaces[3] = ' '; 
	  }
    	  else if (i == 2) 
	  {
	      spaces[0] = letters[0];
              spaces[1] = letters[1];
              spaces[2] = letters[2]; 
              spaces[3] = letters[3];
  	  }
          else {
              spaces[0] = ' ';
              spaces[1] = ' ';
              spaces[2] = ' ';
              spaces[3] = ' ';
          }
          display(spaces, SIZE);
          printf("%c", 0x0d);
          delay(myDelay);
      }
   }
}

/*
  @param aValue
    user specified delay
  @effects
    delays execution by
*/
void delay(unsigned long aValue)
{
  volatile unsigned long i = 0;
  volatile unsigned int j = 0;
  for(i = aValue; i > 0; i--)
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
    printf("%c", letters[i]);
    fflush(stdout);
  }
}
