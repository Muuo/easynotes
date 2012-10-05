#include <stdio.h>
#include <mgl/mgl_c.h>
#include <stdlib.h>
#include <fftw3.h>
#include "pa_io.h"

void printbin(fftw_complex *b,int len);
void copya(double *a,double b, int size);
int greatest(fftw_complex* bins);
int suba(SAMPLE *arr, SAMPLE *sub, int start, int stop);
int mfft(double *dat, fftw_complex *out);
int writefile(SAMPLE * recording,int totalFrames,int c);
int readfile(SAMPLE *recording, int totalFrames);
int main(void);

