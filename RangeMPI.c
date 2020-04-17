#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <math.h>

int main(int argc, char * argv [])
{
	int N, MyID,NumProc, h;
	double sum = 0;
	double rbuf;
	double tstart, tfinish;
	int ierror = MPI_Init(&argc, &argv);
	if(ierror != MPI_SUCCESS)
		printf("MPI initialization error");
	MPI_Comm_size(MPI_COMM_WORLD, &NumProc);
	MPI_Comm_rank(MPI_COMM_WORLD, &MyID);
	MPI_Status status;
	
	if (MyID == 0)
	{
		
		tstart = MPI_Wtime();
		N = atoi(argv[1]);
		
		for(int i = 1; i < NumProc; i++)
		{	
			MPI_Send(&N, 1, MPI_INT, i, i+777, MPI_COMM_WORLD);
		}
		
		
		h = N/NumProc;
		
		if(MyID < N%NumProc)
			h = h + 1;
	
		for(int i = 1; i < h; i++ )
		{
			sum = sum + 1/ ((i+1)*pow(i,1.0/6));
		}
		
		for(int i = 1; i < NumProc; i++)
		{	
			MPI_Recv(&rbuf, 1, MPI_DOUBLE, i, i+777, MPI_COMM_WORLD, &status);
			sum += rbuf;	
		}
		printf("Result : %lf\n", sum);
		printf("Time : %lf", MPI_Wtime() - tstart);
	}
	
	if (MyID > 0)
	{
		
		MPI_Recv(&N, 1, MPI_INT, 0, MyID + 777, MPI_COMM_WORLD, &status);
		h = N/NumProc;
		
		if(MyID < N%NumProc)
			h = h + 1;
		
		for(int i = MyID * h; i < h * (MyID+1); i++ )
		{
			sum = sum + 1/ ((i+1)*pow(i,1.0/6));
		}
		MPI_Send(&sum,1,MPI_DOUBLE,0,777+MyID,MPI_COMM_WORLD);
		
	}
	
	MPI_Finalize();
	return 0;
}
