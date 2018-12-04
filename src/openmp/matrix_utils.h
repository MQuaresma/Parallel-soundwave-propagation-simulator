#define M_SIZE 700
#define STENCIL_P 4
#define PADDED_SIZE M_SIZE + STENCIL_P

#define ITERATIONS 100
void initiateMask(double *C);
void initiateMatrix(int size, int pad, double m[PADDED_SIZE][PADDED_SIZE]);
void printResults(int size, int pad, double m[PADDED_SIZE][PADDED_SIZE]);
