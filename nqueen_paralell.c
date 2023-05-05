#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <pthread.h>
#include <omp.h>
#include <mpi.h>
#include <time.h>
#include <unistd.h>

int count = 0;

void print(int n, int board[]) {

  int i, j;
  ++count;
  printf("\nSolução %d:\n\n", count);

  for (i = 0; i < n; ++i) {
    printf("\t%d", i+1);
  }

  for (i = 0; i < n; ++i) {

    printf("\n\n%d", i+1);

    for (j = 0; j < n; ++j) {

      if (board[i] == j)

        printf("\tQ"); // queen at i,j position

      else

        printf("\t-");
    }
  }
  printf("\n");
}
int place(int row, int column, int board[]) {

  int i;

  for (i = 0; i < row; ++i) {

    // checking column and digonal conflicts

    if (board[i] == column)
      return 0;
    else {
      if (abs(board[i] - column) == abs(i - row))
        return 0;
    }
  }
  return 1; // no conflicts
}
void queen(int row, int col, int n, int board[]) {

  int column;
  for (column = col; column < n; ++column) {

    if (place(row, column, board)) {

      board[row] = column; // no conflicts so place queen
      if (row == n-1) {
        #pragma omp atomic
        ++count;
        int solution[n];

        MPI_Request request;

        memcpy(solution, board, n * sizeof(int));
        MPI_Send(solution, n, MPI_INT, 0, 0, MPI_COMM_WORLD);
      } else {
        queen(row + 1, 0, n, board);
        if (row == 0)
        {
          break;
        }
      }
    }
  }
}
int main(int argc, char **argv) {

  int rank, size;
  
  int n = atoi(argv[1]);
  int omp_threads = atoi(argv[2]);
  int num_solution = 0;

  int **resultados = (int **)malloc(sizeof(int*));
  resultados[0] = malloc(sizeof(int)*n);

  omp_set_dynamic(0);
  omp_set_num_threads(omp_threads);
  
  
  int *board;

  
  clock_t begin = clock();
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  MPI_Comm_size(MPI_COMM_WORLD, &size);

  MPI_Request request;
  MPI_Status status;
  int value[n];
  if (rank == 0) {
    // printf("Rainhas: %d - %d threads - %d processos", n, omp_threads, size);
    int i;
    for(i = 0; i < n; ++i){
      value[i] = i;
      int processo = i%(size-1)+1;

      MPI_Isend(&(value[i]), 1, MPI_INT, processo, 0, MPI_COMM_WORLD, &request);
    }
    int tamanho = 1;
    for(i = 0; i < n;){

      int recv[n];
      MPI_Recv(recv, n, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

      if(status.MPI_TAG == 0){
        ++num_solution;
        memcpy(resultados[tamanho-1], recv, n * sizeof(int));

        ++tamanho;
        resultados = (int **) realloc(resultados, tamanho * sizeof(int*));
        resultados[tamanho-1] = malloc(sizeof(int)*n);

      }else{
        // printf("");
        // printf("Finalizou uma coluna: %d de %d\n", i+1, n);
        ++i;
      }
      
    }

    for (int i = 0; i < size; ++i)
    {
      // Finalizou o código
      int fim = 0;
      MPI_Send(&fim, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
    }
    
  }else{
    while(1)
    {
      int coluna;

      MPI_Recv(&coluna, 1, MPI_INT, 0, MPI_ANY_SOURCE, MPI_COMM_WORLD, &status);

      if (status.MPI_TAG == 1)
      {
        break;
      }

      board = malloc(sizeof(int)*n);

      #pragma omp task
      queen(0, coluna, n, board);
      #pragma omp taskwait

      MPI_Send(&coluna, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    // printf("%d: Finalizando processo...\n", rank);
    return 0;
  }

  MPI_Finalize();
  
  clock_t end = clock();

  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  // printf("Printando resultados\n");
  // for(int i = 0; i < num_solution; i++){
  //   print(n, resultados[i]);
  // }


  printf("%d Soluções\n%f\n", num_solution, time_spent);
  // printf("---------------------------------------\n");

  for(int i = 0; i < num_solution; i++){
    free(resultados[i]);
  }
  free(resultados);

  return 0;
}