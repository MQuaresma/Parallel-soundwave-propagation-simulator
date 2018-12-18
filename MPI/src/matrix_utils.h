#include <stdio.h>
#include <mpi.h>

#define M_SIZE 100
#define STENCIL_P 4

#define ITERATIONS 10
void initiateMask(double *C);
void initiateMatrix(double m[M_SIZE][M_SIZE]);
void printResults(double m[M_SIZE][M_SIZE]);
void copy(double temp[][M_SIZE], int orig_offset, double g[M_SIZE][M_SIZE], int dest_offset, int rows);
