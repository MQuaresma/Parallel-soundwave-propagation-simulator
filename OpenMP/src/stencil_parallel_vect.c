#include <stdio.h>
#include <omp.h>
#include <inttypes.h>
#include "matrix_utils.h"

int main(int argc, const char* argv[]){
    
    if(argc==2){
        int numThreads = strtoimax(argv[1], NULL, 0);
        
        if(numThreads>0){

            double c[5], start_time, end_time, temp;
            static double g[2][M_SIZE][M_SIZE];
            int last_matrix=0;

            initiateMask(c);
            initiateMatrix(g[0]);
            initiateMatrix(g[1]);
            
            omp_set_num_threads(numThreads);

            start_time=omp_get_wtime();
            
            for(int it=0; it<ITERATIONS; it++){
                //one iteration
                #pragma omp parallel for private(temp) schedule(static)
                for(int i=STENCIL_P; i<M_SIZE-STENCIL_P; i++){
                    for(int j=STENCIL_P; j<M_SIZE-STENCIL_P; j++){
                        temp= c[0]*g[last_matrix][i][j];
                        #pragma omp simd aligned(c, g: 32) reduction(+: temp)
                        for(int k=1; k < 5; k++){
                            temp+= c[k]*g[last_matrix][i][j+k];
                            temp+= c[k]*g[last_matrix][i][j-k];
                            temp+= c[k]*g[last_matrix][i+k][j];
                            temp+= c[k]*g[last_matrix][i-k][j];
                        }
                        g[!last_matrix][i][j]=temp;
                    }
                }
                last_matrix=!last_matrix;
            }
            
            end_time = omp_get_wtime() - start_time;
            
            //printResults(g[last_matrix]);

            printf("Execution Time: %f s\n",end_time);
        }else{
            printf("Wrong number of threads!\n");
        }   
    }else{
        printf("Wrong number of arguments. Put number of threads!\n");
    } 
    return 0;
}
