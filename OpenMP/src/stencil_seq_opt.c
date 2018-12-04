#include <stdio.h>
#include <omp.h>
#include "matrix_utils.h"

int main(){
    double c[5], start_time, end_time, temp;
    static double g[2][PADDED_SIZE][PADDED_SIZE];
    int last_matrix=0;

    initiateMask(c);
    initiateMatrix(M_SIZE, STENCIL_P, g[0]); //fill non-padding area
    initiateMatrix(M_SIZE, STENCIL_P, g[1]); //fill non padding area

    start_time=omp_get_wtime();

    for(int it=0; it<ITERATIONS; it++){
        //one iteration
        for(int i=STENCIL_P; i<M_SIZE-STENCIL_P-1; i++)
            for(int j=STENCIL_P; j<M_SIZE-STENCIL_P-1; j++){
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
    
    //printResults(M_SIZE,g[last_matrix]);

    printf("Execution Time: %f s\n",end_time);
    
    return 0;
}
