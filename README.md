# Parallel Soundwave Propagation Simulator
Soundwave propagation algorithms fit in the category of [stencil kernels](https://en.wikipedia.org/wiki/Stencil_code). 
As such, the possibility for improving performance via parallelization is to be considered. The current project, developed as 
an assignment for the **Parallel Computing Paradigms course** at Universidade do Minho aims to present two different approaches 
for such optimizations:
- a shared memory approach via an [OpenMP implementation](OpenMP)
- a distributed memory approach via an [(Open)MPI implementation](MPI)

Further more, each implementation will comprehend a scalability analysis.
