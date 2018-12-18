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
    
    int rank, no_procs, rows_per_proc, begin, end, m_size_rounded, last_matrix=0;
    double c[5], start_time, end_time;
    static double g[M_SIZE][M_SIZE];

    initiateMask(c);
    initiateMatrix(g);

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &no_procs);
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    
    start_time = MPI_Wtime();

    rows_per_proc = (M_SIZE-2*STENCIL_P)/(no_procs-1);                                  //rounds by defect
    if(rank==no_procs-1){
        m_size_rounded = rows_per_proc*(no_procs-1) + 2*STENCIL_P;                      //handle cases when no_procs is not a multiple of the work load size
        rows_per_proc = rows_per_proc + M_SIZE - m_size_rounded;                        //adjust value
    }
    
    double temp[2][rows_per_proc+2*STENCIL_P][M_SIZE];

    if (rank == 0) {
        for(int i=1; i<no_procs; i++) {
            begin = i*rows_per_proc - STENCIL_P;
            if(i!=no_procs-1) end = (i+1)*rows_per_proc + STENCIL_P;
            else end = M_SIZE;

            fillToSend(g, temp, begin, end);

            MPI_Send(temp, (end-begin)*M_SIZE, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        
        for(int i=1; i<no_procs; i++){
            if(i==no_procs-1){
                m_size_rounded = rows_per_proc*(no_procs-1) + 2*STENCIL_P;                      //handle cases when no_procs is not a multiple of the work load size
                rows_per_proc = rows_per_proc + M_SIZE - m_size_rounded;                        //adjust value
            }
            
            MPI_Recv( temp, rows_per_proc*M_SIZE, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
            if(i==no_procs-1) copy(temp,g,rows_per_proc,M_SIZE-rows_per_proc-1);
            else copy(temp,g,rows_per_proc,i*rows_per_proc);
        }
    }else{
        double aux[STENCIL_P][M_SIZE];
        
        MPI_Recv( temp[last_matrix], (rows_per_proc + 2*STENCIL_P)*M_SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, &status );
        
        for(int i = 0; i < ITERATIONS; i++){
            for(int j = 0; j < rows_per_proc; j ++)
                for(int k = STENCIL_P; k < M_SIZE-STENCIL_P-1; k++){
                    temp[!last_matrix][i][j] = temp[last_matrix][i][j]*c[0];
                    for(int w = 1; w <= STENCIL_P; w ++){
                        temp[!last_matrix][i][j] += temp[last_matrix][i][j+w];
                        temp[!last_matrix][i][j] += temp[last_matrix][i][j-w];
                        temp[!last_matrix][i][j] += temp[last_matrix][i-w][j];
                        temp[!last_matrix][i][j] += temp[last_matrix][i+w][j];
                    }
                }
            last_matrix = !last_matrix;

            if(rank!=1){
                copyFrom(aux,temp[last_matrix],0);
                MPI_Send( aux, STENCIL_P*M_SIZE, MPI_INT, rank-1, 2, MPI_COMM_WORLD);
            }
            
            if(rank!=no_procs-1){
                MPI_Recv( aux, STENCIL_P*M_SIZE, MPI_INT, rank+1, 2, MPI_COMM_WORLD, &status );
                copyTo(aux,temp[!last_matrix],rows_per_proc+STENCIL_P);
            }   
            
            if(rank!=no_procs-1){
                copyFrom(aux,temp[last_matrix],rows_per_proc+STENCIL_P);
                MPI_Send( aux, STENCIL_P*M_SIZE, MPI_INT, rank+1, 2, MPI_COMM_WORLD);
            }
            
            if(rank!=1){
                MPI_Recv( aux, STENCIL_P*M_SIZE, MPI_INT, rank-1, 2, MPI_COMM_WORLD, &status );
                copyTo(aux,temp[!last_matrix],0);
            }
        }
        
        double tempToSend[rows_per_proc][M_SIZE];
        fillToSend(temp[last_matrix], tempToSend, STENCIL_P, rows_per_proc);

        MPI_Send( tempToSend, rows_per_proc*M_SIZE, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }

    end_time = MPI_Wtime();
    
    MPI_Finalize();

    printf("Execution Time: %f s\n",end_time-start_time);

    return 0;
}
