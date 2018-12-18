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
    
    int rank, no_procs, rows_per_proc, remaning_rows, begin, end, last_matrix=0, work_load = M_SIZE - 2*STENCIL_P, excess;
    double c[STENCIL_P+1], start_time, end_time, aux[STENCIL_P][M_SIZE];
    static double g[M_SIZE][M_SIZE];

    initiateMask(c);
    initiateMatrix(g);

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &no_procs);
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    
    start_time = MPI_Wtime();

    rows_per_proc = work_load/(no_procs-1);         //truncates result -> by defect
    remaning_rows = work_load % (no_procs-1);      //remaining rows
    excess = rank <= remaning_rows;

    double temp[2][rows_per_proc+2*STENCIL_P+excess][M_SIZE];

    if (rank == 0) {
        begin = 0;
        end = rows_per_proc+2*STENCIL_P;

        for(int i=1; i<no_procs; i++, begin+=rows_per_proc+excess, end+=rows_per_proc+excess) {
            excess = i <=remaning_rows;
            end += excess;
            fillToSend(g, temp[0], begin, end);
            MPI_Send(temp[0], (rows_per_proc+2*STENCIL_P+excess)*M_SIZE, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
        }
        
        begin=STENCIL_P;
        for(int i=1; i<no_procs; i++, begin+=rows_per_proc+excess){
            excess = i <=remaning_rows;
            MPI_Recv( temp[0], (rows_per_proc+excess)*M_SIZE, MPI_DOUBLE, i, 1, MPI_COMM_WORLD, &status);
            copy(temp[0],g,rows_per_proc+excess,begin);
        }
    }else{
        rows_per_proc += excess;
        MPI_Recv( temp[last_matrix], (rows_per_proc + 2*STENCIL_P)*M_SIZE, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status );
        
        for(int i = 0; i < ITERATIONS; i++){
            for(int j = STENCIL_P; j < rows_per_proc+STENCIL_P; j ++)
                for(int k = STENCIL_P; k < M_SIZE-STENCIL_P; k++){
                    temp[!last_matrix][j][k] = temp[last_matrix][j][k]*c[0];
                    for(int w = 1; w <= STENCIL_P; w ++){
                        temp[!last_matrix][j][k] += c[w]*temp[last_matrix][j][k+w];
                        temp[!last_matrix][j][k] += c[w]*temp[last_matrix][j][k-w];
                        temp[!last_matrix][j][k] += c[w]*temp[last_matrix][j+w][k];
                        temp[!last_matrix][j][k] += c[w]*temp[last_matrix][j-w][k];
                    }
                }

            last_matrix = !last_matrix;
            if(rank!=1){
                copyFrom(aux,temp[last_matrix],STENCIL_P);
                MPI_Send( aux, STENCIL_P*M_SIZE, MPI_DOUBLE, rank-1, 2, MPI_COMM_WORLD);
                //MPI_Send( temp[last_matrix][STENCIL_P], STENCIL_P*M_SIZE, MPI_DOUBLE, rank-1, 2, MPI_COMM_WORLD);
            }

            if(rank!=no_procs-1){
                MPI_Recv( aux, STENCIL_P*M_SIZE, MPI_DOUBLE, rank+1, 2, MPI_COMM_WORLD, &status );
                //MPI_Recv( temp[!last_matrix][rows_per_proc+STENCIL_P], STENCIL_P*M_SIZE, MPI_DOUBLE, rank+1, 2, MPI_COMM_WORLD, &status );
                copyTo(aux,temp[last_matrix],rows_per_proc+STENCIL_P);
            }

            if(rank!=no_procs-1){
                copyFrom(aux,temp[last_matrix],rows_per_proc);
                //MPI_Send( temp[last_matrix][rows_per_proc], STENCIL_P*M_SIZE, MPI_DOUBLE, rank+1, 2, MPI_COMM_WORLD);
                MPI_Send( aux, STENCIL_P*M_SIZE, MPI_DOUBLE, rank+1, 2, MPI_COMM_WORLD);
            }

            if(rank!=1){
                //MPI_Recv( temp[!last_matrix], STENCIL_P*M_SIZE, MPI_DOUBLE, rank-1, 2, MPI_COMM_WORLD, &status );
                MPI_Recv( aux, STENCIL_P*M_SIZE, MPI_DOUBLE, rank-1, 2, MPI_COMM_WORLD, &status );
                copyTo(aux,temp[last_matrix],0);
            }
        }
        
        double tempToSend[rows_per_proc][M_SIZE];
        fillToSend(temp[last_matrix], tempToSend, STENCIL_P, rows_per_proc);

        MPI_Send( tempToSend, rows_per_proc*M_SIZE, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }

    end_time = MPI_Wtime();
    
    MPI_Finalize();
    
    if(rank==0) printResults(g);

    printf("Execution Time: %f s\n",end_time-start_time);

    return 0;
}
