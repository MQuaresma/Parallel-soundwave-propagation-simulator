#include<stdio.h>
#include "matrix_utils.h"

void initiateMask(double *C){
    C[0]=5.f;
    C[1]=-1.f;
    C[2]=0.f;
    C[3]=-1.f;
    C[4]=0.f;
}

void initiateMatrix(int m_size, double m[M_SIZE][M_SIZE]){
    for(int i=0; i<m_size; i++)
        for(int j=0; j<m_size; j++)
            m[i][j]=1.f;
}

void printResults(int m_size, double m[M_SIZE][M_SIZE]){
    for(int i=0; i<m_size; i++){
        for(int j=0; j<m_size; j++)
            printf("%15.4f \t",m[i][j]);
        printf("\n");
    }
}
