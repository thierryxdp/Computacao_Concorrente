// Multiplicação de matrizes

#include <stdio.h>
#include <stdlib.h>
#include "timer.h"
#include <pthread.h>

int main(int argc, char* argv[]){

    double inicio, fim;
    float *matriz1, *matriz2, *saida;
    int dim;
    GET_TIME(inicio);
    //leitura e avalicação dos parâmetros de comando [sequencial]

    if (argc < 2){
        printf("Digite: %s <dimensão da matriz>\n", argv[0]);
        return 1;
    }
    dim = atoi(argv[1]);
    

    //alocação de memória para as estruturas de dados [sequencial]

    matriz1 = (float *) malloc(sizeof(float) * dim * dim);
    if (matriz1 == NULL){
        puts("ERRO--malloc\n");
        return 2;
    }
    matriz2 = (float *) malloc(sizeof(float) * dim * dim);
    if (matriz2 == NULL){
        puts("ERRO--malloc\n");
        return 2;
    }
    saida = (float *) malloc(sizeof(float) * dim * dim);
    if (saida == NULL){
        puts("ERRO--malloc\n");
        return 2;
    }

    //inicialização das estruturas de dados [sequencial]
    for (int i = 0; i < dim; i++){
        for (int j = 0; j < dim; j++){
            matriz1[i*dim + j] = 1;
            matriz2[i*dim + j] = 1;
            saida[i*dim + j] = 0;
        }
    }
    GET_TIME(fim);
    printf("Tempo inicialização: %lf\n", fim - inicio);

    //multiplicação de matrizes [concorrente]

    GET_TIME(inicio);
        
    for (int i = 0; i < dim; i++){
        for (int j = 0; j < dim; j++){
            for (int k = 0; k < dim; k++){
                saida[i*dim + j] += matriz1[i * dim + k] * matriz2[k*dim + j];
            }
        }
    }
        
    GET_TIME(fim);
    printf("Tempo multiplicação: %lf\n", fim - inicio);

    //exibição dos resultados [sequencial]

    /*
    puts("Vetor de saída:");
    for (int i = 0; i < dim; i++){
        for (int j = 0; j < dim; j++){
            printf("%.1f ", saida[i*dim + j]);
        }
        puts("");
    }
    puts("");
    */

    //Verificação dos resultados [sequencial]
    
    for (int i = 0; i < dim; i++){
        for (int j = 0; j < dim; j++){
            if (saida[i*dim + j] != dim){
                printf("Resultado Incorreto\n");
                break;
            }
        }
    }

    //liberação de memória [sequencial]
    GET_TIME(inicio);

    free(matriz1);
    free(matriz2);
    free(saida);
    
    GET_TIME(fim);
    printf("Tempo finalização: %lf\n", fim - inicio);

    return 0;
}