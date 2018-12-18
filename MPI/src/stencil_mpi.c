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
    
    int rank, no_procs, rows_per_proc, total_rows_per_proc, begin, end, work_load_rounded, last_matrix=0, work_load = M_SIZE - 2*STENCIL_P;
    double c[STENCIL_P+1], start_time, end_time, aux[STENCIL_P][M_SIZE];
    static double g[M_SIZE][M_SIZE];

    initiateMask(c);
    initiateMatrix(g);

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &no_procs);
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    
    start_time = MPI_Wtime();

    rows_per_proc = work_load/(no_procs-1);                                 //truncates result -> by defect

    if(rank==no_procs-1){
        work_load_rounded = rows_per_proc*(no_procs-1);                     //handle cases when no_procs is not a multiple of work_load
        rows_per_proc = rows_per_proc + (work_load - work_load_rounded);    //adjust value
    }
    if(rank == 0){
        work_load_rounded = rows_per_proc*(no_procs-1);                                         //handle cases when no_procs is not a multiple of work_load
        total_rows_per_proc = rows_per_proc + (work_load - work_load_rounded) + 2*STENCIL_P;    //adjust value
    }else
        total_rows_per_proc = rows_per_proc + 2*STENCIL_P;
    
    double temp[2][total_rows_per_proc][M_SIZE];

    if (rank == 0) {
        begin = 0;
        end = rows_per_proc+2*STENCIL_P;

        for(int i=1; i<no_procs-1; i++, begin += rows_per_proc, end  += rows_per_proc) {
            fillToSend(g, temp[0], begin, end);
            MPI_Send(temp[0], (rows_per_proc+2*STENCIL_P)*M_SIZE, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        begin = end-total_rows_per_proc;
        end = M_SIZE;
        MPI_Send(temp[0], total_rows_per_proc*M_SIZE, MPI_INT, no_procs-1, 0, MPI_COMM_WORLD);

        
        for(int i=1; i<no_procs-1; i++){
            MPI_Recv( temp[0], rows_per_proc*M_SIZE, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
            copy(temp[0],g,rows_per_proc,i*rows_per_proc);
        }

        MPI_Recv( temp[0], (total_rows_per_proc-2*STENCIL_P)*M_SIZE, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
        copy(temp[0],g,M_SIZE-total_rows_per_proc,total_rows_per_proc-2*STENCIL_P);
    }else{
        MPI_Recv( temp[last_matrix], (rows_per_proc + 2*STENCIL_P)*M_SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, &status );
        
        for(int i = 0; i < ITERATIONS; i++){
            for(int j = STENCIL_P; j < rows_per_proc+STENCIL_P; j ++)
                for(int k = STENCIL_P; k < M_SIZE-STENCIL_P-1; k++){
                    temp[!last_matrix][j][k] = temp[last_matrix][j][k]*c[0];
                    for(int w = 1; w <= STENCIL_P; w ++){
                        temp[!last_matrix][j][k] += temp[last_matrix][j][k+w]*c[w];
                        temp[!last_matrix][j][k] += temp[last_matrix][j][k-w]*c[w];
                        temp[!last_matrix][j][k] += temp[last_matrix][j-w][k]*c[w];
                        temp[!last_matrix][j][k] += temp[last_matrix][j+w][k]*c[w];
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
