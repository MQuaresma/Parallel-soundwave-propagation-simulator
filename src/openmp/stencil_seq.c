#include <stdio.h>
#include <omp.h>
#include "matrix_utils.h"

//copy the values from a to b
void copy(double a[PADDED_SIZE][PADDED_SIZE], double b[PADDED_SIZE][PADDED_SIZE]){
    for(int i=0; i<M_SIZE; i++)
        for(int j=0; j<M_SIZE; j++)
            b[i][j]=a[i][j];
}

int main(){
    double c[5], start_time, end_time;
    static double g1[PADDED_SIZE][PADDED_SIZE], g2[PADDED_SIZE][PADDED_SIZE];

    initiateMask(c);
    initiateMatrix(M_SIZE, STENCIL_P, g1);
    initiateMatrix(M_SIZE, STENCIL_P, g2);
    
    start_time = omp_get_wtime();

    for(int it=0; it<ITERATIONS; it++){
        //one iteration
        for(int i=1; i<M_SIZE-1; i++)
            for(int j=1; j<M_SIZE-1; j++){
                g1[i][j]= c[0]*g2[i][j];
                for(int k=1; k < 5; k++){
                    if(j+k < M_SIZE) g1[i][j]+= c[k]*g2[i][j+k];
                    if(j-k >= 0) g1[i][j]+= c[k]*g2[i][j-k];
                    if(i+k < M_SIZE) g1[i][j]+= c[k]*g2[i+k][j];
                    if(i-k >= 0) g1[i][j]+= c[k]*g2[i-k][j];
                }
            }
        copy(g1,g2);
    }

    end_time = omp_get_wtime() - start_time;

    //printResults(M_SIZE,g1);

    printf("Execution Time: %f s\n",end_time);

    return 0;
}