
#include <stdio.h>
#include "mandel.h"

void AllocMem(int Width,int Height) 
{
	Image=new char *[Height];
	for(int i=0;i<Height;i++)
		Image[i]=new char[Width];
}

void FreeMem(int Width,int Height) 
{
	for(int i=0;i<Height;i++)
		delete []  Image[i];
	delete [] Image;
}

void WritePGM(const char *fname,int Width,int Height)
{
	FILE *file=fopen(fname,"wt");
	if (fname==NULL) return;
	fprintf(file,"P5 %d %d 255\n",Width,Height);
	for(int i=0;i<Height;i++)
		fwrite(Image[i],sizeof(char),Width,file);
	fclose(file);
}

int CalcPixel (float x,float y)
{
	double zre,zim,zoldre=0.0f,zoldim=0.0f;
	int i;
	for(i=0;i<255;i++) {
		zre=zoldre*zoldre-zoldim*zoldim+x;
		zim=2.0f*zoldre*zoldim+y;
		if (zim*zim+zre*zre>=4.0f) return i;
		zoldre=zre; zoldim=zim;
	}
	return i;	
}

void CalcImgRow(char *Row,int num,int Width,int Height)
{
	float y=-2.0f+4.0f*num/Height;
	for(int i=0;i<Width;i++)
	{
		float x=-2.0f+4.0f*i/Width;
		Row[i]=(char)(255-CalcPixel(x,y));
	}
}


