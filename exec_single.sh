#!/bin/bash
queens=$1
qnt_testes=$2

gcc nqueen_single.c -o run_single

for (( k=4; k<=queens; k++ ))
do
    echo "$k rainhas - single" >> exec_single_results.txt;
    echo "executado: ${k} rainhas";
    for (( l=0; l<qnt_testes; l++ ))
    do
        ./run_single $k >> exec_single_results.txt;
    done
done
./calc_medias.sh exec_single_results.txt medias_single.txt
sudo shutdown -h now