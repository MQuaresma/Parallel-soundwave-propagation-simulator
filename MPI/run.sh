#!/bin/sh
#
#PBS -N Teste
#PBS -l walltime=10:00
#PBS -l nodes=2:r641:ppn=32
#PBS -q mei

module load gcc/5.3.0
module load gnu/openmpi_eth/2.0.0
#module load gcc/7.2.0

cd PCP/StencilSim
rm Teste.*
make clean
make

mkdir times

OMP_NUM_THREADS=3
while [ $OMP_NUM_THREADS -lt 66 ]; do
    NUM=0
	while [ $NUM -lt 15 ]; do
		mpirun -bycore --oversubscribe -np $OMP_NUM_THREADS -mca btl self,vader,tcp bin/a.out >> times/mpi_res.txt
        let NUM=NUM+1
    done
    let OMP_NUM_THREADS="(($OMP_NUM_THREADS-1)*2)+1"
done

OMP_NUM_THREADS=3
while [ $OMP_NUM_THREADS -lt 66 ]; do
    NUM=0
	while [ $NUM -lt 15 ]; do
		mpirun -bynode --oversubscribe -np $OMP_NUM_THREADS -mca btl self,vader,tcp bin/a.out >> times/mpi_res.txt
        let NUM=NUM+1
    done
    let OMP_NUM_THREADS="(($OMP_NUM_THREADS-1)*2)+1"
done

OMP_NUM_THREADS=3
while [ $OMP_NUM_THREADS -lt 66 ]; do
    NUM=0
	while [ $NUM -lt 15 ]; do
		mpirun -byslot --oversubscribe -np $OMP_NUM_THREADS -mca btl self,vader,tcp bin/a.out >> times/mpi_res.txt
        let NUM=NUM+1
    done
    let OMP_NUM_THREADS="(($OMP_NUM_THREADS-1)*2)+1"
done
