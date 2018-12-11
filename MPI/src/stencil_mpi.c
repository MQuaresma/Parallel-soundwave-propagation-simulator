#include "matrix_utils.h"

void copy(double temp[][M_SIZE], double g[M_SIZE][M_SIZE], int rows, int begin){
    for(int i=0; i<rows; i++)
        for(int j=0; j<M_SIZE; j++)
            g[begin+i][j] = temp[i][j];
}

void copyTo(double aux[STENCIL_P][M_SIZE], double temp[][M_SIZE], int ext){
    for(int i=0; i<STENCIL_P; i++){
        for(int j=0; j<M_SIZE; j++)
            temp[i+ext][j]=aux[i][j];
    }
}

void copyFrom(double aux[STENCIL_P][M_SIZE], double temp[][M_SIZE], int ext){
    for(int i=0; i<STENCIL_P; i++){
        for(int j=0; j<M_SIZE; j++)
            aux[i][j]=temp[i+ext][j];
    }
}

void fillToSend(double g[][M_SIZE], double temp[][M_SIZE], int begin, int end){
    int w=0;
    for(int i = begin; i < end; i++){
        for(int j=0; j < M_SIZE; j++)
            temp[w][j] = g[i][j];
        w++;
    }   
}

int main( int argc, char *argv[]) {
    
    int rank, nP, rows, begin, end, rowsM;
    double c[5], start_time, end_time;
    static double g[M_SIZE][M_SIZE];

    initiateMask(c);
    initiateMatrix(g);

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nP);
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    
    start_time = MPI_Wtime();

    rows = (M_SIZE-2*STENCIL_P)/(nP-1);
    if(rank==nP-1){
        rowsM = rows*(nP-1) + STENCIL_P;
        if(rowsM<M_SIZE) rows += M_SIZE - rowsM;
        else rows -= rowsM-M_SIZE;
        rows -= STENCIL_P;
    }
    
    double temp[rows+2*STENCIL_P][M_SIZE];

    if (rank == 0) {
        for(int i=1; i<nP; i++) {
            begin = i*rows - STENCIL_P;
            if(i!=nP-1) end = (i+1)*rows + STENCIL_P;
            else end = M_SIZE;

            fillToSend(g, temp, begin, end);

            MPI_Send(temp, (end-begin)*M_SIZE, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        
        for(int i=1; i<nP; i++){
            if(i==nP-1){
                rowsM = rows*(nP-1) + STENCIL_P;
                if(rowsM<M_SIZE) rows += M_SIZE - rowsM;
                else rows -= rowsM-M_SIZE;
                rows -= STENCIL_P;           
            }
            
            MPI_Recv( temp, rows*M_SIZE, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
            if(i==nP-1) copy(temp,g,rows,M_SIZE-rows-1);
            else copy(temp,g,rows,i*rows);
        }
    }else{
        double aux[STENCIL_P][M_SIZE];
        
        MPI_Recv( temp, (rows + 2*STENCIL_P)*M_SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, &status );
        
        for(int i = 0; i < ITERATIONS; i++){
            //TODO: calculos
            
            if(rank!=1){
                copyFrom(aux,temp,0);
                MPI_Send( aux, STENCIL_P*M_SIZE, MPI_INT, rank-1, 2, MPI_COMM_WORLD);
            }
            
            if(rank!=nP-1){
                MPI_Recv( aux, STENCIL_P*M_SIZE, MPI_INT, rank+1, 2, MPI_COMM_WORLD, &status );
                copyTo(aux,temp,rows+STENCIL_P);
            }   
            
            if(rank!=nP-1){
                copyFrom(aux,temp,rows+STENCIL_P);
                MPI_Send( aux, STENCIL_P*M_SIZE, MPI_INT, rank+1, 2, MPI_COMM_WORLD);
            }
            
            if(rank!=1){
                MPI_Recv( aux, STENCIL_P*M_SIZE, MPI_INT, rank-1, 2, MPI_COMM_WORLD, &status );
                copyTo(aux,temp,0);
            }
        }
        
        double tempToSend[rows][M_SIZE];
        fillToSend(temp, tempToSend, STENCIL_P, rows);

        MPI_Send( tempToSend, rows*M_SIZE, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }

    end_time = MPI_Wtime();
    
    MPI_Finalize();

    printf("Execution Time: %f s\n",end_time-start_time);

    return 0;
}
