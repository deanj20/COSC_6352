 #include <stdio.h> 
#include <mpi.h> 

int main(void) 
{ 
    int rank, size, tag, rc, i;
    
	MPI_Status status;

	char message[40] ;

	rc = MPI_Init (NULL, NULL) ;
	rc = MPI_Comm_size (MPI_COMM_WORLD, &size) ;
	rc = MPI_Comm_rank (MPI_COMM_WORLD, &rank) ;
	tag = 100;

	if (rank == 0) {
	sprintf (message, "Hello, world created by process %d", rank);
	for (i=1; i < size; i++)
		rc = MPI_Send(message, 40, MPI_CHAR, i, tag, MPI_COMM_WORLD);
	}
	else{
		rc = MPI_Recv(message, 40, MPI_CHAR, 0, tag, MPI_COMM_WORLD, &status);
	
		printf("node %d: %s \n", rank, message);
	}
		rc = MPI_Finalize();
		return 0;
	
} 
