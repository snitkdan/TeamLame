#include <stdio.h>

void delay(unsigned long *aPtr);

/*
  @param delay1
	Delay between each character
  @param toPrint
	Character to be displayed
  @effects
	Displays toPrint with delay delay1

*/
void printChar(unsigned long *delay1, char toPrint)
{
      char myData[3];
      myData[0] = toPrint == ' ' ? toPrint : toPrint + '0';        
      myData[1] = '\0';           //  terminate the string
      printf("%s ", myData);
      fflush(stdout);
      delay(delay1);            //  delay so we can read the display
}
