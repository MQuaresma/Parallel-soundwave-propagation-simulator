#define M_SIZE 700
#define STENCIL_P 4
#define PADDED_SIZE M_SIZE + 2 * STENCIL_P

#define ITERATIONS 100
void initiateMask(double *C);
void initiateMatrix(double m[PADDED_SIZE][PADDED_SIZE]);
void initiateMatrixSeq(double m[M_SIZE][M_SIZE]);
void printResults(double m[PADDED_SIZE][PADDED_SIZE]);
void printResultsSeq(double m[M_SIZE][M_SIZE]);
