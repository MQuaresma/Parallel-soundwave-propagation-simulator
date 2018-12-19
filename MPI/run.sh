#!/bin/sh
#
#PBS -N Teste
#PBS -l walltime=20:00
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

OMP_NUM_THREADS=2
while [ $OMP_NUM_THREADS -lt 65 ]; do
    NUM=0
    echo $OMP_NUM_THREADS 
	while [ $NUM -lt 15 ]; do
		mpirun --map-by core --oversubscribe -np $OMP_NUM_THREADS -mca btl self,vader,tcp bin/a.out >> times/mpi_res_core.txt
        let NUM=NUM+1
    done
    let OMP_NUM_THREADS=$OMP_NUM_THREADS*2
done

OMP_NUM_THREADS=2
while [ $OMP_NUM_THREADS -lt 65 ]; do
    NUM=0
    echo $OMP_NUM_THREADS
	while [ $NUM -lt 15 ]; do
		mpirun --map-by node --oversubscribe -np $OMP_NUM_THREADS -mca btl self,vader,tcp bin/a.out >> times/mpi_res_node.txt
        let NUM=NUM+1
    done
    let OMP_NUM_THREADS=$OMP_NUM_THREADS*2
done

OMP_NUM_THREADS=2
while [ $OMP_NUM_THREADS -lt 65 ]; do
    NUM=0
    echo $OMP_NUM_THREADS
	while [ $NUM -lt 15 ]; do
		mpirun --map-by slot --oversubscribe -np $OMP_NUM_THREADS -mca btl self,vader,tcp bin/a.out >> times/mpi_res_slot.txt
        let NUM=NUM+1
    done
    let OMP_NUM_THREADS=$OMP_NUM_THREADS*2
done
