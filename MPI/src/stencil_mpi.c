#include<stdlib.h>
#include "matrix_utils.h"

int main( int argc, char *argv[]) {
    
    int rank, no_procs, rows_per_proc, remaining_rows, begin, last_matrix=0, work_load = M_SIZE - 2*STENCIL_P, excess;
    double c[STENCIL_P+1], start_time, end_time;
    static double g[M_SIZE][M_SIZE];

    initiateMask(c);
    initiateMatrix(g);

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &no_procs);
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    
    start_time = MPI_Wtime();

    rows_per_proc = work_load/(no_procs-1);         //truncates result -> by defect
    remaining_rows = work_load % (no_procs-1);      //remaining rows
    excess = rank <= remaining_rows;

    double temp[2][rows_per_proc+2*STENCIL_P+excess][M_SIZE];

    if (rank == 0) {
        
        begin = 0;
        for(int i=1; i<no_procs; i++, begin+=rows_per_proc+excess) {
            excess = i <=remaining_rows;
            MPI_Send(g[begin], (rows_per_proc+2*STENCIL_P+excess)*M_SIZE, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
        }
        
        begin=STENCIL_P;
        for(int i=1; i<no_procs; i++, begin+=rows_per_proc+excess){
            excess = i <=remaining_rows;
            MPI_Recv( g[begin], rows_per_proc*M_SIZE, MPI_DOUBLE, i, 1, MPI_COMM_WORLD, &status);
        }
    }else{
        rows_per_proc += excess;
        MPI_Recv( temp, (rows_per_proc + 2*STENCIL_P)*M_SIZE, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status );
        copy(temp[last_matrix],0,temp[!last_matrix],0,rows_per_proc + 2*STENCIL_P);

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
            if(rank!=1)
                MPI_Send( temp[last_matrix][STENCIL_P], STENCIL_P*M_SIZE, MPI_DOUBLE, rank-1, 2, MPI_COMM_WORLD);

            if(rank!=no_procs-1)
                MPI_Recv( temp[last_matrix][rows_per_proc+STENCIL_P], STENCIL_P*M_SIZE, MPI_DOUBLE, rank+1, 2, MPI_COMM_WORLD, &status );

            if(rank!=no_procs-1)
                MPI_Send( temp[last_matrix][rows_per_proc], STENCIL_P*M_SIZE, MPI_DOUBLE, rank+1, 2, MPI_COMM_WORLD);

            if(rank!=1)
                MPI_Recv( temp[last_matrix], STENCIL_P*M_SIZE, MPI_DOUBLE, rank-1, 2, MPI_COMM_WORLD, &status );
        }
        MPI_Send( temp[last_matrix][STENCIL_P], rows_per_proc*M_SIZE, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
    }

    end_time = MPI_Wtime();
    
    MPI_Finalize();
    
    if(rank==0) printResults(g);

    //printf("Execution Time: %f s\n",end_time-start_time);

    return 0;
}
