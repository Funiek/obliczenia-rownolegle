#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "mandel.h"

int rank,size;
char **Image;
const int imgWidth=4096;
const int imgHeight=4096;
char Row[imgWidth];

void Master()
{
	int NextRow,ActiveSlaves;
	MPI_Status Status;

	
	for(int i=0;i<size-1;i++)
		MPI_Send(&i,1,MPI_INT,i+1,0,MPI_COMM_WORLD);
	ActiveSlaves=size-1;
	NextRow=rank;
	do {
		MPI_Recv(Row,imgWidth,MPI_CHAR,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&Status);
		ActiveSlaves--;
		int ReceivedRow=Status.MPI_TAG;
		int Slave=Status.MPI_SOURCE;
		memcpy(Image[ReceivedRow],Row,sizeof(char)*imgWidth);
		if (NextRow<imgHeight) {
			MPI_Send(&NextRow,1,MPI_INT,Slave,0,MPI_COMM_WORLD);
			ActiveSlaves++;
			NextRow++;
		} else {
			int minusOne=-1;
			MPI_Send(&minusOne,1,MPI_INT,Slave,0,MPI_COMM_WORLD);
		}
	} while (ActiveSlaves>0);
}

void Slave()
{
	int RowNum;
	MPI_Status Status;
	while(1) {
		MPI_Recv(&RowNum,1,MPI_INT,0,0,MPI_COMM_WORLD,&Status);
		if (RowNum==-1) 
			return;
		CalcImgRow(Row,RowNum,imgWidth,imgHeight);
		MPI_Send(Row,imgWidth,MPI_CHAR,0,RowNum,MPI_COMM_WORLD);
	}
}

int main(int argc,char *argv[])
{
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	if (size<2) {
		printf("At least 2 processes are needed\n");
		MPI_Finalize();
		exit(-1);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	if (rank==0) { 
		AllocMem(imgWidth,imgHeight);
		double t1=MPI_Wtime();
		Master();
		double t2=MPI_Wtime();
		printf("Execution time: %5.3f msecs\n",(t2-t1)*1000.0);
		WritePGM("mandel.pgm",imgWidth,imgHeight);
		FreeMem(imgWidth,imgHeight);
	} else
		Slave();	
	MPI_Finalize();
}
