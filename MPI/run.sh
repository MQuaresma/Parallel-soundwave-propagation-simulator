#!/bin/sh
#
#PBS -N Teste
#PBS -l walltime=10:00
#PBS -l nodes=1:r641:ppn=32
#PBS -q mei

module load gcc/5.3.0
#module load gcc/7.2.0

cd PCP/StencilSim
rm Teste.*
make clean
make

mkdir times

cd bin

NUM_P=3
while [ $NUM_P -lt 18 ]; do
    NUM=0
	while [ $NUM -lt 15 ]; do
		mpirun -np $NUM_P a.out >> ../times/mpi_res.txt
        let NUM=NUM+1
    done
    let "NUM_P=(($NUM_P-1)*2)+1"
done
