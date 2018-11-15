#include <omp.h>
#include <stdio.h>
#include "auxFunctions.h"

#define N 9
#define ITERATIONS 1

//copy the values from a to b
void copy(double a[N][N], double b[N][N]){
    for(int i=0; i<N; i++)
        for(int j=0; j<N; j++)
            b[i][j]=a[i][j];
}

int main(){
    double C[5], G1[N][N], G2[N][N], start_time, end_time;
    initiateMask(C);
    initiateMatrix(N,G2);
    
    start_time = omp_get_wtime();

    for(int it=0; it<ITERATIONS; it++){
        //one iteration
        for(int i=1; i<N-1; i++)
            for(int j=1; j<N-1; j++){
                G1[i][j]= C[0]*G2[i][j];
                for(int k=1; k < 5; k++){
                    if(j+k < N) G1[i][j]+= C[k]*G2[i][j+k];
                    if(j-k >= 0) G1[i][j]+= C[k]*G2[i][j-k];
                    if(i+k < N) G1[i][j]+= C[k]*G2[i+k][j];
                    if(i-k >= 0) G1[i][j]+= C[k]*G2[i-k][j];
                }
            }
        copy(G1,G2);
    }

    end_time = omp_get_wtime() - start_time;

    printResults(N,G1);

    printf("Execution Time: %f s\n",end_time);

    return 0;
}
