#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

#define VEC_SIZE 1000
#define SAMPLE_SIZE 10000

double latency_test(MPI_Status, int);
double bandwitdh_test(double, MPI_Status, int);

int main(int argc, char *argv[]){
    int rank, no_procs;
    double latency, bandwitdh;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &no_procs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    latency = latency_test(status, rank);
    bandwitdh = bandwitdh_test(latency, status, rank);

    MPI_Finalize();
    if(!rank){
        printf("Average Latency: %12.8f us \n", latency); 
        printf("Average BW: %12.8f us/byte\n", bandwitdh); 
    }
}


double latency_test(MPI_Status status, int rank){
    double avg_time = 0.f, delta;
    
    if(!rank)
        avg_time =  MPI_Wtime();
    for(int i = 0; i < SAMPLE_SIZE; i++){
        if(!rank){
            MPI_Send(NULL, 0, MPI_INT, 1, 1, MPI_COMM_WORLD);
            MPI_Recv(NULL, 0, MPI_INT, 1, 1, MPI_COMM_WORLD, &status);
        }else{
            MPI_Recv(NULL, 0, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
            MPI_Send(NULL, 0, MPI_INT, 0, 1, MPI_COMM_WORLD);
        }    
    }
    
    MPI_Barrier(MPI_COMM_WORLD);
    if(!rank)
        avg_time = MPI_Wtime() - avg_time;

    avg_time = avg_time/SAMPLE_SIZE*1.0e6;
    return avg_time;
}

double bandwitdh_test(double latency, MPI_Status status, int rank){
    double avg_time = 0.f, delta;
    int *vec = (int*)calloc(VEC_SIZE, sizeof(int));
    
    if(!rank)
        avg_time = MPI_Wtime();
    for(int i = 0; i < SAMPLE_SIZE; i++){
        if(!rank){
            MPI_Send(vec, VEC_SIZE, MPI_INT, 1, 1, MPI_COMM_WORLD);
            MPI_Recv(vec, VEC_SIZE, MPI_INT, 1, 1, MPI_COMM_WORLD, &status);
        }else{
            MPI_Recv(vec, VEC_SIZE, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
            MPI_Send(vec, VEC_SIZE, MPI_INT, 0, 1, MPI_COMM_WORLD);
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if(!rank)
        avg_time = MPI_Wtime() - avg_time;
    avg_time = avg_time/SAMPLE_SIZE*1.0e6-latency;

    return avg_time/(2*VEC_SIZE*sizeof(int));
}