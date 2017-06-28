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
  while(TRUE)
  {
    for(int i = 0; i < SIZE; i++)
    {
    	display(&letters[i], 1);
    	delay(myDelay);
        printf("%c", 0x0d); 
        fflush(stdout); 
        undisplay(1);
        delay(myDelay);
        printf("%c", 0x0d);
        fflush(stdout);
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

/*
  @param size
    size of the output to erase
  @modifies console window
  @effects
    erases all size characters from the window
*/
void undisplay(int size)
{
  int i;
  for(i = 0; i < size; i++)
  {
    printf(" ");
    fflush(stdout);
  }
}
