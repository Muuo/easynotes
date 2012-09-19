#include <stdio.h>
#include <stdlib.h>
#include <fftw3.h>
#include "pa_io.h"

int mfft(double *dat, fftw_complex *out,int N)
{
double *in;
fftw_plan p;

in = (double*) malloc(sizeof(double) * N);
out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (N/2+1));
p = fftw_plan_dft_r2c_1d(N, in, out, FFTW_ESTIMATE);

in=dat;
fftw_execute(p); /* repeat as needed */

fftw_destroy_plan(p);
fftw_free(in); fftw_free(out);
return 0;
}

/*******************************************************************/
int main(void);
int main(void)
{
	pa_io();
	return 0;
}
