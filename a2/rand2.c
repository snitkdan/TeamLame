#include <stdio.h>
#include <math.h>
#include <float.h>
#include <limits.h>

//  random number generator 
unsigned int reseed(void);
double randomReal(void);

unsigned int seed = 1;
unsigned int multiplier = 2743;
unsigned int addOn = 5923;
double max = INT_MAX + 1.0;

//  demonstrate the generators
unsigned int randomInteger(unsigned int low, unsigned int high)
{
    if (low > high)
        return randomInteger(high, low);
    else
        return ((unsigned int)((high-low+1)*randomReal()))+low;
}

unsigned int reseed(void)
{
    seed = seed*multiplier + addOn;
    return seed;
}

//  generate a real random number
double randomReal(void)
{
    double temp = reseed();
    return temp/max;
}



