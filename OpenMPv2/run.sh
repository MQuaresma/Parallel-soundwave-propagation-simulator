#!/bin/sh

make clean
make

mkdir times

NUM=0
while [ $NUM -lt 15 ]; do
    ./bin/stencil_seq >> times/stencil_seq.txt
    let NUM=NUM+1
done

NUM=0
while [ $NUM -lt 15 ]; do
    ./bin/stencil_seq_opt >> times/stencil_opt.txt
    let NUM=NUM+1
done

OMP_NUM_THREADS=2
while [ $OMP_NUM_THREADS -lt 9 ]; do
    NUM=0
	while [ $NUM -lt 15 ]; do
		./bin/stencil_parallel $OMP_NUM_THREADS >> times/parallel_res.txt
        let NUM=NUM+1
    done
	let OMP_NUM_THREADS=OMP_NUM_THREADS*2
done
