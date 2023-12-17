#include <stdio.h>
#include <time.h>
#include "mandel.h"

#ifdef _OPENMP
#include <omp.h>
#endif

const int imgWidth=4096;
const int imgHeight=4096;
char **Image;

void CalcImage(int Width,int Height)
{
#pragma omp parallel for  schedule(dynamic)
	for(int i=0;i<Height;i++)
		CalcImgRow(Image[i],i,Width,Height);
}

int main() 
{
#ifdef _OPENMP
	omp_set_nested(0);
#pragma omp parallel
	{
#pragma omp single
		{
			printf("OpenMP version running with %d threads\n",omp_get_num_threads());
		}

	}
#else
	printf("Single threaded version\n");
#endif
	AllocMem(imgWidth,imgHeight);
	timespec tp1,tp2;
	clock_gettime(CLOCK_MONOTONIC,&tp1);
	CalcImage(imgWidth,imgHeight);
	clock_gettime(CLOCK_MONOTONIC,&tp2);

	double extime=(double)tp2.tv_sec+1e-9*(double)tp2.tv_nsec-(double)tp1.tv_sec-1e-9*(double)tp1.tv_nsec;
	printf("Execution time: %5.3f msecs\n",extime*1000.0);
	WritePGM("mandel.pgm",imgWidth,imgHeight);
	FreeMem(imgWidth,imgHeight);
	return 0;
}
