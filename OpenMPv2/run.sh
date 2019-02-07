#!/bin/sh

run() {
    NUM=0
    while [ $NUM -lt 15 ]; do
        ./bin/stencil_seq >> times/stencil_seq_$1_$2.txt
        let NUM=NUM+1
    done

    NUM=0
    while [ $NUM -lt 15 ]; do
        ./bin/stencil_seq_opt >> times/stencil_opt_$1_$2.txt
        let NUM=NUM+1
    done

    OMP_NUM_THREADS=2
    while [ $OMP_NUM_THREADS -lt 9 ]; do
        NUM=0
        while [ $NUM -lt 15 ]; do
            ./bin/stencil_parallel $OMP_NUM_THREADS >> times/parallel_res_$1_$2.txt
            let NUM=NUM+1
        done
        let OMP_NUM_THREADS=OMP_NUM_THREADS*2
    done
}

mkdir times

M_SIZE=("32" "128" "512" "2048")
IT=("64" "256")

for it in "${IT[@]}"; do

    sed -i "s/#define ITERATIONS[^\n]*/#define ITERATIONS $it/" src/matrix_utils.h

    for ms in "${M_SIZE[@]}"; do
        make clean
        sed -i "s/#define M_SIZE[^\n]*/#define M_SIZE $ms/" src/matrix_utils.h
        make
        run $it $ms
    done
done
