/** Muuo. In God I trust :-) **/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fftw3.h>
#include <sndfile.h>
#include "pa_io.h"

#define N 1024

void printbin(fftw_complex *b,int len)
{
	int x;
	for(x=0;x<len;x++)
	//	printf("%fHz - %f+%fj\n",x*SAMPLE_RATE/(float)N,b[x][0],b[x][1]);
		printf("%Le ",((long double)b[x][0]*(long double)b[x][0])+((long double)b[x][1]*(long double)b[x][1]));
	printf("\n");
}

void copya(double *a, double *b,int size)
{
	int x;
	for(x=0;x<size;x++)
		b[x]=a[x];
}

int greatest(fftw_complex *b)
{
	int x=0,gind=-1;
	long double greatest=0,mag;
	for(x=0;x<(N/2+1);x++)
	{
		mag=((long double)b[x][0]*(long double)b[x][0])+((long double)b[x][1]*(long double)b[x][1]);
		if(mag>greatest)
		{
			greatest=mag;
			gind=x;
		}
	}
	//printf("Greatest mag is %Lf\n",greatest);
	return gind;
}

int suba(SAMPLE *arr, SAMPLE *sub,int start, int stop)
{
	int x,y;
	//sub=(double*) malloc(sizeof(double) * (stop-start))
	for(x=start,y=0;x<stop;x++,y++)
		sub[y]=arr[x];
	return 0;
}

int mfft(double *dat, fftw_complex *out)
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
int writefile(SAMPLE *recording,int totalFrames, int c)
{
		
	SNDFILE	*file ;
	SF_INFO	sfinfo ;
	
	//memset (&sfinfo, 0, sizeof (sfinfo)) ;

	sfinfo.samplerate	= SAMPLE_RATE ;
	sfinfo.frames		= totalFrames ;
	sfinfo.channels		= c ;
	sfinfo.format		= (SF_FORMAT_WAV | SF_FORMAT_PCM_32) ;
	
	if (! (file = sf_open ("pyplot/recorded.wav", SFM_WRITE, &sfinfo)))
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
	//writefile(recording,frames,2);
#if 0
	{
		int x;
		for(x=10;x<=15;x++)
			printf(""PRINTF_S_FORMAT"\n",recording[x]);
	}		
#endif
	SAMPLE *left,*right,*mono;
	left=(SAMPLE *)malloc(frames*sizeof(SAMPLE));
	right=(SAMPLE *)malloc(frames*sizeof(SAMPLE));
	mono=(SAMPLE *)malloc(frames*sizeof(SAMPLE));
	int x,y=0,state=1,lx=0,rx=0;
	for(x=0;x<frames;x++)
	{
		if(y>3)
		{
			y=0;
			state=abs(state-1);
		}
		if(state)
		{
			left[lx]=recording[x];
			lx++;
		}
		else
		{
			right[rx]=recording[x];
			rx++;
		}
		y++;
	}
	for(x=0;x<frames/2;x++)
		mono[x]=(left[x]+right[x])/2.0;
	free(left);free(right);	
	
	writefile(mono,frames/2,1);
	
	int numWindows = frames/(N*2);x=0;
	SAMPLE section[N];
	fftw_complex *bins;
	bins=(fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (N/2+1));
	
	int lind;
	for(x=0;x<numWindows;x++)
	{	
		suba(mono,section,x*N,(x*N)+N);
		mfft((double*)section,bins);
//		printbin(bins,513);
		if ((lind=greatest(bins))!=-1)
			printf("The fundamental frequency is %f\n", lind*SAMPLE_RATE/(float)N);
		else
			printf("Bummer!\n");
	}

	/*Place code to process bins here*/

	fftw_free(bins);
	free(recording);
	free(mono);
done:
	return 0;
}
