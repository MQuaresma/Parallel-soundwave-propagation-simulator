#include <stdio.h>
#include <omp.h>
#include "auxFunctions.h"

#define N 9
#define ITERATIONS 5

int main(){
    double C[5], G[2][N][N], start_time, end_time, temp;
    int last_matrix=0;
    initiateMask(C);
    initiateMatrix(N,G[0]);
    initiateMatrix(N,G[1]);

    start_time=omp_get_wtime();

    for(int it=0; it<ITERATIONS; it++){
        //one iteration
        for(int i=1; i<N-1; i++)
            for(int j=1; j<N-1; j++){
                temp= C[0]*G[last_matrix][i][j];
                for(int k=1; k < 5; k++){
                    if(j+k < N) temp+= C[k]*G[last_matrix][i][j+k];
                    if(j-k >= 0) temp+= C[k]*G[last_matrix][i][j-k];
                    if(i+k < N) temp+= C[k]*G[last_matrix][i+k][j];
                    if(i-k >= 0) temp+= C[k]*G[last_matrix][i-k][j];
                }
                G[!last_matrix][i][j]=temp;
            }
        
        last_matrix=!last_matrix;
    }
    
    end_time = omp_get_wtime() - start_time;
    
    printResults(N,G[last_matrix]);

    printf("Execution Time: %f s\n",end_time);
    
    return 0;
}
