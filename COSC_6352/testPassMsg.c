#include <stdio.h> 
#include <mpi.h> 

int main(int argc, char *argv[]) 
{ 
    int rank, size; 
    MPI_Init(&argc, &argv); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &size); 

    if (rank == 0) 
    { 
        int message = 42; 
        MPI_Send(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD); 
    } 
    else if (rank == 1) 
    { 
        int received_message; 
        MPI_Recv(&received_message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
        printf("Process 1 received message: %d\n" ,received_message); 
    } 

    MPI_Finalize(); 
    return 0; 
} 
