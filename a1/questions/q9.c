//*****************************************************************************
//
// Built from hello.c - Simple hello world example.
//
// Copyright (c) 2006-2011 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
//
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
//
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
//
// This is part of revision 7243 of the EK-LM3S8962 Firmware Package.
//
//*****************************************************************************

#include <stdio.h>

#define TRUE 1
#define FALSE 0


void delay(unsigned long *aPtr);
void f1Data(unsigned long *delay1);
void f2Clear(unsigned long *delay2);
// Print "0123456789" 
//
//*****************************************************************************
int main(void)
{
    //
    // The value if i is:
    //
    fflush(stdout);
    //
    //  print the digits 9 8 7 6 5 4 3 2 1 0
    long unsigned int d1 = 1000;
    long unsigned int d2 = 1000;  
    long unsigned int *d1ptr = &d1;
    long unsigned int *d2ptr = &d2; 
    while(TRUE)
    {
        f1Data(d1ptr);
        f2Clear(d2ptr);
    } 
}

void f1Data(unsigned long *delay1)
{
      char myData[3];
      for (volatile int i = 9; i >=0; i--)
      {
          myData[0] = i + '0';        //  convert the int i to ascii
          myData[1] = '\0';           //  terminate the string
          printf("%s ", myData);
	  fflush(stdout);
          delay(delay1);            //  delay so we can read the display
      }
      printf("%c", 0x0d);           //  print a carridge return
      fflush(stdout);
      myData[0] = ' ';              //  print a space
      myData[1] = '\0';
}

void f2Clear(unsigned long *delay2)
{
      char myData[3];
      for (volatile int i = 0; i < 10; i++)
      {
          printf("%s ", myData);
  	  fflush(stdout);
          delay(delay2);
      }

      printf("%c", 0x0d);           //  print a carridge return
      fflush(stdout);
      myData[0] = ' ';              //  print a space
      myData[1] = '\0';
}
//  software delay
void delay(unsigned long *aPtr)
{
    int aValue = *aPtr;
    volatile unsigned long i = 0;
    volatile unsigned int j = 0;
    for (i = aValue; i > 0; i--)
    {
        for (j = 0; j < 100000; j++);
    }
    return;
}
