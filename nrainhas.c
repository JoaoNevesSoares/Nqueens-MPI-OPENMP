#include <omp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool **tabuleiro_geral;
bool resolvido = false;
int n;

bool atacando(bool **tabuleiro, int linha, int coluna) {
  int i, j;

  // checa diagonais
  for (i = linha, j = coluna; i >= 0 && j >= 0; i--, j--) {
    if (tabuleiro[i][j])
      return true;
  }

  // checa diagonais
  for (i = linha, j = coluna; i < n && j >= 0; i++, j--) {
    if (tabuleiro[i][j])
      return true;
  }

  // checa linha
  for (i = 0; i < n; i++) {
    if (tabuleiro[linha][i])
      return true;
  }

  return false;
}

bool resposta(bool **tabuleiro, int coluna, int linha) {
  if (coluna >= n)
    return true;

  if (resolvido)
    return false;

  for (int l = linha; l < n; l++) {
    if (!atacando(tabuleiro, l, coluna)) {
      tabuleiro[l][coluna] = true;

      if (resposta(tabuleiro, coluna + 1, 0)) {
        return true;
      }

      if (coluna == 0) {
        // printf("finalizou uma árvore\n");
        return false;
      }
      tabuleiro[l][coluna] = false;
    }
  }

  return false;
}

/*void print_tabuleiro(bool **tabuleiro) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (tabuleiro[j][i])
        printf("Q ");
      else
        printf(". ");
    }
    printf("\n");
  }
}*/

int main(int argc, char **argv) {
  bool **tabuleiro;
  n = atoi(argv[1]);
  int t;
  t = atoi(argv[2]);
  
  if (n == 2 || n == 3){
    printf("\nNao existe solucao para n rainhas com n = 2 ou n = 3, digite outro valor para n.\n\n");
  }
  
  double start, end;
  omp_set_dynamic(0);
  omp_set_num_threads(t);

  tabuleiro_geral = (bool **)malloc(n * sizeof(bool *));

  for (int i = 0; i < n; i++) {
    tabuleiro_geral[i] = (bool *)calloc(n, sizeof(bool));
  }

  bool encontrou_solucao = false;

  start = omp_get_wtime();

#pragma omp parallel for private(tabuleiro)
  for (int linha = 0; linha < n; linha++) {

    tabuleiro = (bool **)malloc(n * sizeof(bool *));

    for (int i = 0; i < n; i++) {
      tabuleiro[i] = (bool *)calloc(n, sizeof(bool));
    }
    encontrou_solucao = resposta(tabuleiro, 0, linha);

#pragma omp critical
    if (!resolvido && encontrou_solucao) {
      {
        resolvido = true;

        tabuleiro_geral = tabuleiro;

        //print_tabuleiro(tabuleiro_geral);
        end = omp_get_wtime();
        printf("Tempo: %f\n", end - start); // prita o tempo decorrido
      }
    }
  }

  for (int i = 0; i < n; i++)
    free(tabuleiro_geral[i]);
  free(tabuleiro_geral);
  return 0;
}