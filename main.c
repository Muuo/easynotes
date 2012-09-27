/** Muuo. In God I trust :-) **/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fftw3.h>
#include <sndfile.h>
#include "pa_io.h"

void copya(double *a, double *b,int size)
{
	int x;
	for(x=0;x<size;x++)
		b[x]=a[x];
}

int suba(SAMPLE *arr, SAMPLE *sub,int start, int stop)
{
	int x,y;
	//sub=(double*) malloc(sizeof(double) * (stop-start))
	for(x=start,y=0;x<stop;x++,y++)
		sub[y]=arr[x];
	return 0;
}

int mfft(double *dat, fftw_complex *out,int N)
{
	double *in;
	fftw_plan p;

	in = (double*) fftw_malloc(sizeof(double) * N);
	//out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (N/2+1));
	p = fftw_plan_dft_r2c_1d(N, in, out, FFTW_ESTIMATE);
	
	copya(dat,in,N);
	//fftw_free(in);
	//in=dat;
	fftw_execute(p); /* repeat as needed */

	fftw_destroy_plan(p);
	fftw_free(in);
	//fftw_free(in); fftw_free(out);
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
int writefile(SAMPLE *recording,int totalFrames)
{
		
	SNDFILE	*file ;
	SF_INFO	sfinfo ;
	
	//memset (&sfinfo, 0, sizeof (sfinfo)) ;

	sfinfo.samplerate	= SAMPLE_RATE ;
	sfinfo.frames		= totalFrames ;
	sfinfo.channels		= NUM_CHANNELS ;
	sfinfo.format		= (SF_FORMAT_WAV | SF_FORMAT_PCM_24) ;
	
	if (! (file = sf_open ("recorded.wav", SFM_WRITE, &sfinfo)))
	{	printf ("Error : Not able to open output file.\n") ;
		return 1 ;
	}
	
	if (sf_writef_float (file, (float *)recording, totalFrames) != totalFrames)
		puts (sf_strerror (file)) ;
	
	sf_close (file) ;
	return	 0 ;	
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
	writefile(recording,frames);
#if 0
	{
		int x;
		for(x=10;x<=15;x++)
			printf(""PRINTF_S_FORMAT"\n",recording[x]);
	}		
#endif
	int numWindows = frames/1024,x=0;
	SAMPLE section[1024];
	fftw_complex *bins;
	bins=(fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (1024/2+1));

	for(x=0;x<numWindows;x++)
	{	
		suba(recording,section,x*1024,(x*1024)+1024);
		mfft((double*)section,bins,1024);
	}
	fftw_free(bins);
done:
	return 0;
}
