#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int board[20], count = 0;

void print(int n) {

  int i, j;

  printf("\n\nSolution %d:\n\n", count);

  for (i = 1; i <= n; ++i) {
    printf("\t%d", i);
  }

  for (i = 1; i <= n; ++i) {

    printf("\n\n%d", i);

    for (j = 1; j <= n; ++j) {

      if (board[i] == j)

        printf("\tQ"); // queen at i,j position

      else

        printf("\t-");
    }
  }
}
int place(int row, int column) {

  int i;

  for (i = 1; i <= row - 1; ++i) {

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
void queen(int row, int n) {

  int column;
  for (column = 1; column <= n; ++column) {

    if (place(row, column)) {

      board[row] = column; // no conflicts so place queen
      if (row == n) {
        // #pragma omp atomic
        count++;
        // print(n);
      } else {
        // #pragma omp task untied
        //  #pragma omp task
        queen(row + 1, n);
        // #pragma omp taskwait
      }
    }
  }
}
int main(int argc, char **argv) {
  int n = atoi(argv[1]);
  //   printf(" - N Queens Problem Using Backtracking -");
  //   printf("\n\nEnter number of Queens:");
  //   scanf("%d", &n);
  clock_t begin = clock();
  queen(1, n);
  clock_t end = clock();
  double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
  printf("%f\n", time_spent);

  return 0;
}