/*******************************************************************************/
/* optfft.c                                                                    */
/*                                                                             */
/* An optimized version of the fft function using only 16-bit integer math.    */
/*                                                                             */
/* Optimized by Brent Plump                                                    */
/* Based heavily on code by Jinhun Joung                                       */
/*                                                                             */
/* - Works only for input arrays of 256 length.                                */
/* - Requires two arrays of 16-bit ints.  The first contains the samples, the  */
/*   second contains all zeros.  The samples range from -31 to 32              */ 
/* - Returns the index of the peak frequency                                   */
/*******************************************************************************/
#include "optfft.h"

#define ABS(x)  (((x)<0)?(-(x)):(x))
#define CEILING(x) (((x)>511)?511:(x))

signed int optfft(signed int real[256], signed int imag[256]) {

signed int i, i1, j, l, l1, l2, t1, t2, u;

#include "tables.c"

	/* Bit reversal. */
	/*Do the bit reversal */
	l2 = 128;
	i=0;
	for(l=0;l<255;l++) {
		if(l < i) {
			j=real[l];real[l]=real[i];real[i]=j;
		}
		l1 = l2;
		while (l1 <= i){
			i -= l1;
			l1 >>= 1;
		}
		i += l1;
	}

	/* Compute the FFT */
	u = 0;
	l2 = 1;
	for(l=0;l<8;l++){
		l1 = l2;
		l2 <<= 1;
		for(j=0;j<l1;j++){
			for(i=j;i<256;i+=l2){
				i1 = i + l1;
				t1 = (u1[u]*real[i1] - u2[u]*imag[i1])/32; 
				t2 = (u1[u]*imag[i1] + u2[u]*real[i1])/32;
				real[i1] = real[i]-t1;
				imag[i1] = imag[i]-t2;
				real[i] += t1;
				imag[i] += t2;
			}
			u++;
		}
	}

	/* Find the highest amplitude value */
	/* start at index 1 because 0 can hold high values */
	j=1;
	l=0;	   	
	for ( i=1; i<(128); i++ ) {
		l1 = square[CEILING(ABS(real[i]))]+square[CEILING(ABS(imag[i]))];
		if (l1 > l) {
			j = i;
			l = l1;
		}
	}
	return (j);
}

void main() 
{
	const signed int u1[] = {
 32,
 32,
  0,
 32,
 22,
  0,
-22,
 32,
 29,
 22,
 12,
  0,
-12,
-22,
-29,
 32,
 31,
 29,
 26,
 22,
 17,
 12,
  6,
  0,
 -6,
-12,
-17,
-22,
-26,
-29,
-31,
 32,
 31,
 31,
 30,
 29,
 28,
 26,
 24,
 22,
 20,
 17,
 15,
 12,
  9,
  6,
  3,
  0,
 -3,
 -6,
 -9,
-12,
-15,
-17,
-20,
-22,
-24,
-26,
-28,
-29,
-30,
-31,
-31,
 32,
 31,
 31,
 31,
 31,
 31,
 30,
 30,
 29,
 28,
 28,
 27,
 26,
 25,
 24,
 23,
 22,
 21,
 20,
 19,
 17,
 16,
 15,
 13,
 12,
 10,
  9,
  7,
  6,
  4,
  3,
  1,
  0,
 -1,
 -3,
 -4,
 -6,
 -7,
 -9,
-10,
-12,
-13,
-15,
-16,
-17,
-19,
-20,
-21,
-22,
-23,
-24,
-25,
-26,
-27,
-28,
-28,
-29,
-30,
-30,
-31,
-31,
-31,
-31,
-31,
 32,
 31,
 31,
 31,
 31,
 31,
 31,
 31,
 31,
 31,
 31,
 30,
 30,
 30,
 30,
 29,
 29,
 29,
 28,
 28,
 28,
 27,
 27,
 27,
 26,
 26,
 25,
 25,
 24,
 24,
 23,
 23,
 22,
 22,
 21,
 20,
 20,
 19,
 19,
 18,
 17,
 17,
 16,
 15,
 15,
 14,
 13,
 12,
 12,
 11,
 10,
 10,
  9,
  8,
  7,
  7,
  6,
  5,
  4,
  3,
  3,
  2,
  1,
  0,
  0,
  0,
 -1,
 -2,
 -3,
 -3,
 -4,
 -5,
 -6,
 -7,
 -7,
 -8,
 -9,
-10,
-10,
-11,
-12,
-12,
-13,
-14,
-15,
-15,
-16,
-17,
-17,
-18,
-19,
-19,
-20,
-20,
-21,
-22,
-22,
-23,
-23,
-24,
-24,
-25,
-25,
-26,
-26,
-27,
-27,
-27,
-28,
-28,
-28,
-29,
-29,
-29,
-30,
-30,
-30,
-30,
-31,
-31,
-31,
-31,
-31,
-31,
-31,
-31,
-31,
-31
};
	signed int zero[256] = {0};
	signed int freq = optfft(u1, zero);
	printf("Frequency from fft: %d\n", freq);
}