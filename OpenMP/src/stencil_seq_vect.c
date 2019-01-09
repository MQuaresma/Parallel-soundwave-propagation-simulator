#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "matrix_utils.h"

int main(){
    double start_time, end_time, temp;
    double __attribute__((aligned(32))) c[5];
    double __attribute__((aligned(32))) g[2][M_SIZE][M_SIZE];
    int last_matrix=0;

    initiateMask(c);
    initiateMatrix(g[0]); 
    initiateMatrix(g[1]);

    start_time=omp_get_wtime();

    for(int it=0; it<ITERATIONS; it++){
        //one iteration
        for(int i=STENCIL_P; i<M_SIZE-STENCIL_P; i++)
            for(int j=STENCIL_P; j<M_SIZE-STENCIL_P; j++){
                temp = c[0]*g[last_matrix][i][j];
                for(int k=1; k < STENCIL_P; k++){
                    temp += g[last_matrix][i][j+k]*c[k];
                    temp += g[last_matrix][i][j-k]*c[k];
                    temp += g[last_matrix][i+k][j]*c[k];
                    temp += g[last_matrix][i-k][j]*c[k];
                }
                g[!last_matrix][i][j]=temp;
            }
        last_matrix=!last_matrix;
    }
    
    end_time = omp_get_wtime() - start_time;
    
    //printResults(g[last_matrix]);

    printf("Execution Time: %f s\n",end_time);
    
    return 0;
}
