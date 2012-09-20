/** Muuo. In God I trust :-) **/
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

int readfile(SAMPLE *recording, int totalFrames)
{
	FILE  *fid;
        fid = fopen("recorded.raw", "r");
        if( fid == NULL )
        {
        	printf("Could not open file.");
		return -1;
        }
        else
        {
        	fread( recording, NUM_CHANNELS * sizeof(SAMPLE), totalFrames, fid );
        	fclose( fid );
        	printf("Read data from 'recorded.raw'\n");
		return 0;
        }
}

/*******************************************************************/
int main(void)
{
	int frames=pa_io();
	SAMPLE *recording;
	if(frames != -1)
	{
		int numSamples = frames * NUM_CHANNELS;
    		int numBytes = numSamples * sizeof(SAMPLE);
    		recording = (SAMPLE *) malloc( numBytes ); /* From now on, recordedSamples is initialised. */
    		if( recording == NULL )
    		{
        		printf("Could not allocate record array.\n");
        		goto done;
    		}
		int i;
    		for( i=0; i<numSamples; i++ ) recording[i] = 0;	
	}
	readfile(recording,frames);
#if 1
	{
		int x;
		for(x=10;x<=15;x++)
			printf(""PRINTF_S_FORMAT"\n",recording[x]);
	}		
#endif
		

done:
	return 0;
}
