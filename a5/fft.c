
#ifdef  _C196_
#if _ARCHITECTURE_ != 'KD'
/* This will generate a warning, please check model() control. */
#pragma model(KD)
#endif
#endif
#include        <stdio.h>

#include		<float.h>
//#include		<fpal96.h>
#include		<math.h>

#include 		"fft.h"



/**************************************************************************

fft - In-place radix 2 decimation in time FFT

x:  pointer to complex array of samples
w:  pointer to temp complex array with size of (n/2)-1
m:	 FFT N length in power of 2, N = 2**m = fft length

void fft(COMPLEX *x, COMPLEX *w, int m)

*************************************************************************/

void fft(COMPLEX *x, COMPLEX *w, unsigned int m)
{
	 COMPLEX u,temp,tm;
	 COMPLEX *xi,*xip,*xj,*wptr;

	 unsigned int n;

	 unsigned int i,j,k,l,le,windex;

	 float arg,w_real,w_imag,wrecur_real,wrecur_imag,wtemp_real;

	 /* n = 2**m = fft length */

	 n = 1 << m;
	 le = n/2;


	 /* calculate the w values recursively */

	 arg = 4.0*atan(1.0)/le;         /* PI/le calculation */
	 wrecur_real = w_real = cos(arg);
	 wrecur_imag = w_imag = -sin(arg);
	 xj = w;
	 for (j = 1 ; j < le ; j++) {
		xj->real = (float)wrecur_real;
		xj->imag = (float)wrecur_imag;
		xj++;
		wtemp_real = wrecur_real*w_real - wrecur_imag*w_imag;
		wrecur_imag = wrecur_real*w_imag + wrecur_imag*w_real;
		wrecur_real = wtemp_real;
	 }


/* start fft */

	 le = n;
    windex = 1;
    for (l = 0 ; l < m ; l++) {
        le = le/2;

/* first iteration with no multiplies */

        for(i = 0 ; i < n ; i = i + 2*le) {
            xi = x + i;
            xip = xi + le;
            temp.real = xi->real + xip->real;
            temp.imag = xi->imag + xip->imag;
            xip->real = xi->real - xip->real;
            xip->imag = xi->imag - xip->imag;
            *xi = temp;
        }

/* remaining iterations use stored w */

        wptr = w + windex - 1;
        for (j = 1 ; j < le ; j++) {
            u = *wptr;
            for (i = j ; i < n ; i = i + 2*le) {
                xi = x + i;
                xip = xi + le;
                temp.real = xi->real + xip->real;
                temp.imag = xi->imag + xip->imag;
                tm.real = xi->real - xip->real;
					 tm.imag = xi->imag - xip->imag;
                xip->real = tm.real*u.real - tm.imag*u.imag;
                xip->imag = tm.real*u.imag + tm.imag*u.real;
                *xi = temp;
            }
            wptr = wptr + windex;
        }
        windex = 2*windex;
    }            

/* rearrange data by bit reversing */

    j = 0;
    for (i = 1 ; i < (n-1) ; i++) {
        k = n/2;
        while(k <= j) {
            j = j - k;
            k = k/2;
        }
        j = j + k;
        if (i < j) {
            xi = x + i;
            xj = x + j;
            temp = *xj;
            *xj = *xi;
            *xi = temp;
        }
    }
}


