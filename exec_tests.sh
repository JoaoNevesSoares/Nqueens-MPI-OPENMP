#!/bin/bash

#./exec_tests.sh 6 8 16 5
procs=$1;
omp_threads=$2;
queens=$3
qnt_testes=$4
mpicc -fopenmp -O3 -lpthread nqueen_paralell.c -o run;

touch exec_results.txt;

for (( i=2; i<=procs; i++ ))
do
    for (( j=1; j<=omp_threads; j*=2 ))
    do
        for (( k=4; k<=queens; k++ ))
        do
            echo "$k rainhas - $j omp threads - $i processos" >> exec_results.txt;
            echo "executado: ${i} processo, ${k} rainhas, ${j} omp threads";
            for (( l=0; l<qnt_testes; l++ ))
            do
                mpirun --hostfile hostfile -np ${i} run ${k} ${j} >> exec_results.txt;
            done
        done
    done
    echo ""
done