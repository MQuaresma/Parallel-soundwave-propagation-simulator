#!/bin/sh
#
#PBS -N Teste
#PBS -l walltime=10:00
#PBS -l nodes=1:r641:ppn=16
#PBS -q mei

module load gcc/5.3.0
#module load gcc/7.2.0

make

export OMP_NUM_THREADS=2

echo "Sequencial: "
./bin/stencil_seq_opt

echo "Parallel: "
./bin/stencil_par1
