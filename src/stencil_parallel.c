#include <stdio.h>
#include <omp.h>
#include "matrix_utils.h"

int main(){
    double c[5], start_time, end_time, temp, sum;
    static double g[2][M_SIZE][M_SIZE];
    int last_matrix=0;

    initiateMask(c);
    initiateMatrix(M_SIZE,g[0]);
    initiateMatrix(M_SIZE,g[1]);

    start_time=omp_get_wtime();
    
    for(int it=0; it<ITERATIONS; it++){
        //one iteration
        #pragma omp parallel for collapse(2) private(temp, sum)
        for(int i=1; i<M_SIZE-1; i++){
            for(int j=1; j<M_SIZE-1; j++){
                temp= c[0]*g[last_matrix][i][j];
                for(int k=1; k < 5; k++){
                    sum = (j+k < M_SIZE ? g[last_matrix][i][j+k] : 0.f);
                    sum += (j-k >= 0 ? g[last_matrix][i][j-k] : 0.f);
                    sum += (i+k < M_SIZE ? g[last_matrix][i+k][j] : 0.f);
                    sum += (i-k >= 0 ? g[last_matrix][i-k][j]: 0.f);
                    temp += sum*c[k];
                }
                g[!last_matrix][i][j]=temp;
            }
        }
        last_matrix=!last_matrix;
    }
    
    end_time = omp_get_wtime() - start_time;
    
    //printResults(M_SIZE,g[last_matrix]);

    printf("Execution Time: %f s\n",end_time);
    
    return 0;
}
