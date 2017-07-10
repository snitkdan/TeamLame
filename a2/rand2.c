#include <stdio.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include "rand2.h"

unsigned int seed = 1;
unsigned int multiplier = 2743;
unsigned int addOn = 5923;
double max = INT_MAX + 1.0;

unsigned int randomInteger(unsigned int low, unsigned int high) {
    return (low > high) ? randomInteger(high, low) : (((unsigned int)((high-low+1)*randomReal()))+low);
}

unsigned int reseed(void) {
    seed = seed*multiplier + addOn;
    return seed;
}

double randomReal(void) {
    double temp = reseed();
    return temp/max;
}
