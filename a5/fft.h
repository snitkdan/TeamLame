

/* COMPLEX STRUCTURE */

typedef struct {
	 float real, imag;
} COMPLEX;

void fft(COMPLEX *x, COMPLEX *w, unsigned int m);