#include <stdio.h>
#include <mpi.h>

#define M_SIZE 700
#define STENCIL_P 4

#define ITERATIONS 100
void initiateMask(double *C);
void initiateMatrix(double m[M_SIZE][M_SIZE]);
void printResults(double m[M_SIZE][M_SIZE]);
