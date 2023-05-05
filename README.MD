# Problema das N rainhas
> Como rodar:
- Para a versão MPI com OpenMP:
    `mpicc -fopenmp -O3 -lpthread nqueen_paralell.c -o run`
    
    `mpirun --hostfile hostfile -np num_de_processos run num_rainhas num_omp_threads`
    
- Para a versão serial:
    - No termial, asseguresse de estar na pasta do programa nrainhassingle.c e digite:
        `gcc nqueen_single.c -o run_single`
    - Com o programa compilado digite:
        `./run_single n`
    - Com n sendo o número N(número de rainhas e tamanho do tabuleiro).
