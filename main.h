#include <stdio.h>
#include <stdlib.h>
#include <fftw3.h>
#include "pa_io.h"

int mfft(double *dat, fftw_complex *out,int N);
int readfile(SAMPLE *recording, int totalFrames);
int main(void);
