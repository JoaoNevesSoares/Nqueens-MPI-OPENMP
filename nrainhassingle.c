#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

bool **tabuleiro;
int n;

bool atacando(int linha, int coluna) {
    int i, j;

    // checa linha
    for (i = 0; i < n; i++) {
        if (tabuleiro[linha][i])
            return true;
    }

    // checa diagonals
    for (i = linha, j = coluna; i >= 0 && j >= 0; i--, j--) {
        if (tabuleiro[i][j])
            return true;
    }
    for (i = linha, j = coluna; i < n && j >= 0; i++, j--) {
        if (tabuleiro[i][j])
            return true;
    }

    return false;
}

bool resposta(int coluna) {
    if (coluna >= n)
        return true;

    for (int linha = 0; linha < n; linha++) {
        if (!atacando(linha, coluna)) {
            tabuleiro[linha][coluna] = true;

            if (resposta(coluna + 1))
                return true;

            tabuleiro[linha][coluna] = false;
        }
    }

    return false;
}

/*void print_tabuleiro() {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (tabuleiro[i][j])
                printf("Q ");
            else
                printf(". ");
        }
        printf("\n");
    }
}*/

int main(int argc, char **argv) {
    n = atoi(argv[1]);
    double time = 0.0;
 
    clock_t start = clock();

    tabuleiro = (bool **)malloc(n * sizeof(bool *));
    for (int i = 0; i < n; i++)
        tabuleiro[i] = (bool *)calloc(n, sizeof(bool));
    
    
    if (!resposta(0)) {
        printf("\nNao existe solucao para n rainhas com n = 2 ou n = 3, digite outro valor para n.\n\n");
    } else {
        //print_tabuleiro();
    }
    clock_t end = clock();
    time += (double)(end - start) / CLOCKS_PER_SEC;
 
    printf("Tempo: %f\n", time);
    // Limpa a memoria do tabuleiro
    for (int i = 0; i < n; i++)
        free(tabuleiro[i]);
    free(tabuleiro);
    

    return 0;
}


