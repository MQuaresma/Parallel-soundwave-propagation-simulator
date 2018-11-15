#include<stdio.h>

void initiateMask(double *C){
    C[0]=5.f;
    C[1]=-1.f;
    C[2]=0.f;
    C[3]=-1.f;
    C[4]=0.f;
}

void initiateMatrix(int N, double M[N][N]){
    for(int i=0; i<N; i++)
        for(int j=0; j<N; j++)
            M[i][j]=1.f;
}

void printResults(int N, double M[N][N]){
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++)
            printf("%15.4f \t",M[i][j]);
        printf("\n");
    }   
}
