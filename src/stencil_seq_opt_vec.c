#include <stdio.h>
#include <omp.h>
#include "matrix_utils.h"

int main(){
    double c[5], start_time, end_time, temp;
    static double g[2][M_SIZE][M_SIZE];
    int last_matrix=0;
    
    double vector1[4]  __attribute__((aligned(16)));
    double vector2[4]  __attribute__((aligned(16)));

    initiateMask(c);
    initiateMatrix(M_SIZE,g[0]);
    initiateMatrix(M_SIZE,g[1]);

    start_time=omp_get_wtime();

    for(int it=0; it<ITERATIONS; it++){
        //one iteration
        for(int i=1; i<M_SIZE-1; i++)
            for(int j=1; j<M_SIZE-1; j++){
                temp= c[0]*g[last_matrix][i][j];
                
                for(int k=1; k<5; k++){
                    vector1[k-1]=c[k];
                    if(j+k < M_SIZE) vector2[k-1]=g[last_matrix][i][j+k];
                    else vector2[k-1]=0;
                }

                for(int k=0; k<4; k++){
                    temp+=vector1[k]*vector2[k];
                }
                
                for(int k=1; k<5; k++){
                    if(j-k >=0) vector2[k-1]=g[last_matrix][i][j-k];
                    else vector2[k-1]=0;
                }

                for(int k=0; k<4; k++){
                    temp+=vector1[k]*vector2[k];
                }

                for(int k=1; k<5; k++){
                    if(i+k < M_SIZE) vector2[k-1]=g[last_matrix][i+k][j];
                    else vector2[k-1]=0;
                }

                for(int k=0; k<4; k++){
                    temp+=vector1[k]*vector2[k];
                }
                
                for(int k=1; k<5; k++){
                    if(i-k >= 0) vector2[k-1]=g[last_matrix][i-k][j];
                    else vector2[k-1]=0;
                }

                for(int k=0; k<4; k++){
                    temp+=vector1[k]*vector2[k];
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
