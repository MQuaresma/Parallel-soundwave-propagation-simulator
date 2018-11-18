#!/bin/sh
#
#PBS -N Teste
#PBS -l walltime=10:00
#PBS -l nodes=1:r641:ppn=16
#PBS -q mei

module load gcc/5.3.0
#module load gcc/7.2.0

cd PCP/StencilSim
rm Teste.*
make clean
make

NUM=0

while [ $NUM -lt 15 ]; do
    OMP_NUM_THREADS=2
	while [ $OMP_NUM_THREADS -lt 17 ]; do
		echo "=== #THREADS: " $OMP_NUM_THREADS  " ==="
		echo "Sequencial: "
		./bin/stencil_seq

		echo "Parallel: "
		./bin/stencil_parallel
		let OMP_NUM_THREADS=OMP_NUM_THREADS+2
	done
	let NUM=NUM+1
done
