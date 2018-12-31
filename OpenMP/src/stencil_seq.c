#include <stdio.h>
#include <omp.h>
#include "matrix_utils.h"

//copy the values from a to b
void copy(double a[M_SIZE][M_SIZE], double b[M_SIZE][M_SIZE]){
    for(int i=0; i<M_SIZE; i++)
        for(int j=0; j<M_SIZE; j++)
            b[i][j]=a[i][j];
}

int main(){
    double c[5], start_time, end_time;
    static double g1[M_SIZE][M_SIZE], g2[M_SIZE][M_SIZE];

    initiateMask(c);
    initiateMatrix(g1);
    initiateMatrix(g2);
    
    start_time = omp_get_wtime();

    for(int it=0; it<ITERATIONS; it++){
        //one iteration
        for(int i=STENCIL_P; i<M_SIZE-STENCIL_P; i++)
            for(int j=STENCIL_P; j<M_SIZE-STENCIL_P; j++){
                g1[i][j]= c[0]*g2[i][j];
                for(int k=1; k <= STENCIL_P; k++){
                    g1[i][j]+= c[k]*g2[i][j+k];
                    g1[i][j]+= c[k]*g2[i][j-k];
                    g1[i][j]+= c[k]*g2[i+k][j];
                    g1[i][j]+= c[k]*g2[i-k][j];
                }
            }
        copy(g1,g2);
    }

    end_time = omp_get_wtime() - start_time;

    //printResults(g1);

    printf("Execution Time: %f s\n",end_time);

    return 0;
}
