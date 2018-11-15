#include<stdio.h>

void initiateMask(double *C){
    C[0]=5;
    C[1]=-1;
    C[2]=0;
    C[3]=-1;
    C[4]=0;
}

void initiateMatrix(int N, double M[N][N]){
    for(int i=0; i<N; i++)
        for(int j=0; j<N; j++)
            M[i][j]=1;
}

void printResults(int N, double M[N][N]){
    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++)
            printf("%15.4f \t",M[i][j]);
        printf("\n");
    }   
}
