#include <stdio.h>
#include <omp.h>
#include "matrix_utils.h"

int main(){
    double start_time, end_time, temp;
    static double g[2][M_SIZE][M_SIZE];
    int last_matrix=0;
    
    double vector1[5]  __attribute__((aligned(16)));
    double vector2[4]  __attribute__((aligned(16)));

    initiateMask(vector1);
    initiateMatrix(M_SIZE,g[0]);
    initiateMatrix(M_SIZE,g[1]);

    start_time=omp_get_wtime();

    for(int it=0; it<ITERATIONS; it++){
        //one iteration
        for(int i=1; i<M_SIZE-1; i++)
            for(int j=1; j<M_SIZE-1; j++){
                temp=vector1[4]*g[last_matrix][i][j];
                
                for(int k=1; k<5; k++)
                    vector2[k-1] = (j+k < M_SIZE ? g[last_matrix][i][j+k] : 0.f);

                #pragma omp simd reduction(+:temp)
                for(int k=0; k<4; k++)
                    temp+=vector1[k]*vector2[k];
                
                for(int k=1; k<5; k++)
                    vector2[k-1] = (j-k >= 0 ? g[last_matrix][i][j-k] : 0.f);

                #pragma omp simd reduction(+:temp)
                for(int k=0; k<4; k++)
                    temp+=vector1[k]*vector2[k];

                for(int k=1; k<5; k++)
                    vector2[k-1] = (i+k < M_SIZE ? g[last_matrix][i+k][j] : 0.f);

                #pragma omp simd reduction(+:temp)
                for(int k=0; k<4; k++)
                    temp+=vector1[k]*vector2[k];
                
                for(int k=1; k<5; k++)
                    vector2[k-1] = (i-k >= 0 ? g[last_matrix][i-k][j] : 0.f);

                #pragma omp simd reduction(+:temp)
                for(int k=0; k<4; k++)
                    temp+=vector1[k]*vector2[k];

                g[!last_matrix][i][j]=temp;
            }
        
        last_matrix=!last_matrix;
    }
    
    end_time = omp_get_wtime() - start_time;
    
    //printResults(M_SIZE,g[last_matrix]);

    printf("Execution Time: %f s\n",end_time);
    
    return 0;
}
