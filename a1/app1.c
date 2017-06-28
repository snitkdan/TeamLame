#include <stdio.h>

#define TRUE 1
#define FALSE 0

void delay(unsigned long aValue);
void undisplay(int size);
void display(char letters[], int size);

const unsigned long myDelay = 1000;

#define SIZE 4;  // demo size for output string.

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
    display(letters, SIZE);  // print A B C D to the console.
    delay(myDelay);
    printf("%c", 0x0d);  // return to the beginning of the console.
    undisplay(SIZE);  // erase A B C D from the console
    delay(myDelay);
    printf("%c", 0x0d);
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
    printf("%c", letters[i]);  // print a character
    fflush(stdout);  // add output to console
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
    printf(" ");  // print a space character
    fflush(stdout);  // add output to console
  }
}
