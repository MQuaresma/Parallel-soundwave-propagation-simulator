#include "matrix_utils.h"

int main( int argc, char *argv[]) {
    
    int rank, no_procs, rows_per_proc, remaning_rows, begin, last_matrix=0, work_load = M_SIZE - 2*STENCIL_P, excess;
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
        for(int i=1; i<no_procs; i++, begin+=rows_per_proc+excess) {
            excess = i <=remaning_rows;
            copy(g, begin, temp[0], 0, rows_per_proc+excess+2*STENCIL_P);
            MPI_Send(temp[0], (rows_per_proc+2*STENCIL_P+excess)*M_SIZE, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
        }
        
        begin=STENCIL_P;
        for(int i=1; i<no_procs; i++, begin+=rows_per_proc+excess){
            excess = i <=remaning_rows;
            MPI_Recv( temp[0], (rows_per_proc+excess)*M_SIZE, MPI_DOUBLE, i, 1, MPI_COMM_WORLD, &status);
            copy(temp[0], 0, g, begin, rows_per_proc+excess);
        }
    }else{
        rows_per_proc += excess;
        MPI_Recv( temp[last_matrix], (rows_per_proc + 2*STENCIL_P)*M_SIZE, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status );
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
            if(rank!=1){
                copy(temp[last_matrix], STENCIL_P, aux, 0, STENCIL_P);
                MPI_Send( aux, STENCIL_P*M_SIZE, MPI_DOUBLE, rank-1, 2, MPI_COMM_WORLD);
            }

            if(rank!=no_procs-1){
                MPI_Recv( aux, STENCIL_P*M_SIZE, MPI_DOUBLE, rank+1, 2, MPI_COMM_WORLD, &status );
                copy(aux, 0, temp[last_matrix], rows_per_proc+STENCIL_P, STENCIL_P);
            }

            if(rank!=no_procs-1){
                copy(temp[last_matrix], rows_per_proc, aux, 0, STENCIL_P);
                MPI_Send( aux, STENCIL_P*M_SIZE, MPI_DOUBLE, rank+1, 2, MPI_COMM_WORLD);
            }

            if(rank!=1){
                MPI_Recv( aux, STENCIL_P*M_SIZE, MPI_DOUBLE, rank-1, 2, MPI_COMM_WORLD, &status );
                copy(aux, 0, temp[last_matrix], 0, STENCIL_P);
            }
        }
        
        double tempToSend[rows_per_proc][M_SIZE];
        copy(temp[last_matrix], STENCIL_P, tempToSend, 0, rows_per_proc);

        MPI_Send( tempToSend, rows_per_proc*M_SIZE, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
    }

    end_time = MPI_Wtime();
    
    MPI_Finalize();
    
    if(rank==0) printResults(g);

    printf("Execution Time: %f s\n",end_time-start_time);

    return 0;
}
