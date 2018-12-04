#include<stdio.h>
#include "matrix_utils.h"

void initiateMask(double *C){
    C[0]=5.f;
    C[1]=-1.f;
    C[2]=0.f;
    C[3]=-1.f;
    C[4]=0.f;
}


void initiateMatrix(int m_size, int pad, double m[M_SIZE+STENCIL_P][M_SIZE+STENCIL_P]){
    for(int i=pad; i<m_size-pad; i++)
        for(int j=pad; j<m_size-pad; j++)
            m[i][j]=1.f;
}

void printResults(int m_size, int pad, double m[M_SIZE+STENCIL_P][M_SIZE+STENCIL_P]){
    for(int i=pad; i<m_size-pad; i++){
        for(int j=pad; j<m_size-pad; j++)
            printf("%15.4f \t",m[i][j]);
        printf("\n");
    }
}
