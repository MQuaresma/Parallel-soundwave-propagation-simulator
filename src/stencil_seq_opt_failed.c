#include <stdio.h>
#include <omp.h>
#include "matrix_utils.h"

int main(){
    double c[5], start_time, end_time, temp;
    static double g[2][M_SIZE][M_SIZE];
    int last_matrix=0;

    initiateMask(c);
    initiateMatrix(M_SIZE,g[0]);
    initiateMatrix(M_SIZE,g[1]);

    start_time=omp_get_wtime();

    for(int it=0; it<ITERATIONS; it++){
        //one iteration
        for(int i=1; i<M_SIZE-1; i++)
            for(int j=1; j<M_SIZE-1; j++){
                int k_i_max = (M_SIZE-1-i<4 ? M_SIZE-1-i:4);
                int k_i_min = (i<4 ? i:4); 
                int k_j_max = (M_SIZE-1-j<4 ? M_SIZE-1-j:4);
                int k_j_min = (j<4 ? j:4);
                temp = c[0]*g[last_matrix][i][j];

                while(k_i_min){
                    temp += c[k_i_min]*g[last_matrix][i-k_i_min][j];
                    k_i_min --;
                }
                while(k_i_max){
                    temp += c[k_i_max]*g[last_matrix][i+k_i_max][j];
                    k_i_max--;
                }

                while(k_j_min){
                    temp += c[k_j_min]*g[last_matrix][i][j-k_j_min];
                    k_j_min--;
                }
                while(k_j_max){
                    temp += c[k_j_max]*g[last_matrix][i][j+k_j_max];
                    k_j_max--;
                }
                
                g[!last_matrix][i][j] = temp;
            }
        last_matrix=!last_matrix;
    }
    
    end_time = omp_get_wtime() - start_time;
    
    //printResults(M_SIZE,g[last_matrix]);

    printf("Execution Time: %f s\n",end_time);
    
    return 0;
}
