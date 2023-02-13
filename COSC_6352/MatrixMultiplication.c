#include <stdlib.h> 
#include <stdio.h> 
#include <mpi.h> 
#include <time.h> 
#include <sys/time.h> 

#define N 4

MPI_Status status;
double matrix_a[N][N], matrix_b[N][N], matrix_c[N][N];
    
int main(int argc, char **argv) 
{
    int size, rank, size1, source, dest, rows, offset;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    size1 = size - 1;

    //=======================================tasks of master process (process 0)============================================
    //===generate two matrices A and B======================================================================================

    if (rank == 0){

        //Matrix A and Matrix B both will be filled with random numbers
        srand (time(NULL));
        for (int i=0;i<N;i++){
            for (int j=0;j<N;j++){
                matrix_a[i][j]=rand()%10;
                matrix_b[i][j]=rand()%10;
            }
        }

        printf("\n\t\tMatrix - Matrix Multiplication using MPI\n");

        //Print Matrix A
            printf("\nMatrix A\n\n");
            for(int i=0;i<N;i++){
                for(int j=0;j<N;j++){
                    printf("%.0f\t",matrix_a[i][j]);
                }
                    printf("\n");
            }

        //Print Matrix B
            printf("\nMatrix B\n\n");
            for(int i=0;i<N;i++){
                for(int j=0;j<N;j++){
                    printf("%.0f\t",matrix_b[i][j]);
                }
                    printf("\n");
            }
    
    //==========================================send one row of A and B to each of the slave processes=======================


        rows = N/size1;
        offset=0;

        for(dest=1;dest<=size1;dest++){
//printf("\nStart Send rows block\n\n");
//printf("\nStart MPI_Send(&offset,1,MPI_INT,dest,1,MPI_COMM_WORLD);\n\n");
            MPI_Send(&offset,1,MPI_INT,dest,1,MPI_COMM_WORLD);
//printf("\nStart MPI_Send(&rows,1,MPI_INT,dest,1,MPI_COMM_WORLD););\n\n");
            MPI_Send(&rows,1,MPI_INT,dest,1,MPI_COMM_WORLD);
//printf("\nStart MPI_Send(&matrix_a[offset][0],rows*N,MPI_DOUBLE,dest,1,MPI_COMM_WORLD);\n\n");            
            MPI_Send(&matrix_a[offset][0],rows*N,MPI_DOUBLE,dest,1,MPI_COMM_WORLD);
//printf("\nStart MPI_Send(&matrix_b,N*N,MPI_DOUBLE,dest,1,MPI_COMM_WORLD);\n\n");            
            MPI_Send(&matrix_b,N*N,MPI_DOUBLE,dest,1,MPI_COMM_WORLD);


            //Offset is modified according to number of rows sent to each process 
            offset = offset + rows;            
        }
        //======receive C[row,:]===========================================================
            for (int i= 1; i<=size1;i++)
            {                
//printf("\nStart Receive Rows Loop %i\n\n",i);            
                source=i;
//printf("\nMPI_Recv(&offset,1,MPI_INT,source,2,MPI_COMM_WORLD,&status);\n\n");            
                MPI_Recv(&offset,1,MPI_INT,source,2,MPI_COMM_WORLD,&status);
//printf("\nMPI_Recv(&rows,1,MPI_INT,source,2,MPI_COMM_WORLD,&status);\n\n");            
                MPI_Recv(&rows,1,MPI_INT,source,2,MPI_COMM_WORLD,&status);
//printf("\nMPI_Recv(&matrix_c[offset][0],rows*N,MPI_DOUBLE,source,2,MPI_COMM_WORLD,&status);\n\n");            
                MPI_Recv(&matrix_c[offset][0],rows*N,MPI_DOUBLE,source,2,MPI_COMM_WORLD,&status);
            }
    //=========================Print the result matrix======================================
        printf("\nResult Matrix C = Matrix A * Matrix B:\n\n");
               for(int i=0;i<N;i++){
                for(int j=0;j<N;j++)
                        printf("%.0f\t",matrix_c[i][j]);
                    printf("\n");
               
            printf("\n");
        }   

}
    //=========================Tasks of slave processes=====================================
    //==each of the processes for generating a column (if the number of the slave processes is 4)==)
        
        if (rank>0){
            source=0;       //source process

            MPI_Recv(&offset,1,MPI_INT,source,1,MPI_COMM_WORLD,&status);
            MPI_Recv(&rows,1,MPI_INT,source,1,MPI_COMM_WORLD,&status);            
            MPI_Recv(&matrix_a,rows*N,MPI_DOUBLE,source,1,MPI_COMM_WORLD,&status);            
            MPI_Recv(&matrix_b,N*N,MPI_DOUBLE,source,1,MPI_COMM_WORLD,&status); 

            for(int k=0;k<N;k++){
                for (int i=0;i<rows;i++){
                    matrix_c[i][k]=0.0;
                    for(int j=0;j<N;j++)
                    matrix_c[i][k]=matrix_c[i][k]+matrix_a[i][j]*matrix_b[j][k];
                    
                }
            }
        

        MPI_Send(&offset,1,MPI_INT,0,2,MPI_COMM_WORLD);
        MPI_Send(&rows,1,MPI_INT,0,2,MPI_COMM_WORLD);
        MPI_Send(&matrix_c,rows*N,MPI_DOUBLE,0,2,MPI_COMM_WORLD);
        }
    //====================================================================================

       MPI_Finalize();

    
}