#define N 50
#define ITERATIONS 5

void copy(double a[N][N], double b[N][N]){
    for(int i=0; i<N; i++)
        for(int j=0; j<N; j++)
            b[i][j]=a[i][j];
}

int main(){
    double C[5], G1[N][N], G2[N][N];

    for(int it=0; it<ITERATIONS; it++){
        //one iteration
        for(int i=1; i<N-1; i++)
            for(int j=1; j<N-1; j++)
                G1[i][j]= C[0]*G2[i][j]+
                          C[1]*G2[i][j+1]+
                          C[2]*G2[i][j+2]+
                          C[3]*G2[i][j+3]+
                          C[4]*G2[i][j+4];
        copy(G1,G2);
    }

    return 0;
}
