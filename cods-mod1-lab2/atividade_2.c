// Multiplicação de matrizes

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

float *matriz1, *matriz2, *saida;
int nthreads;
int dim;

void * tarefa(void * arg){
    int id = *(int *) arg;
    for (int i = id; i < dim; i += nthreads){
        for (int j = 0; j < dim; j++){
            for (int k = 0; k < dim; k++){
                saida[i*dim + j] += matriz1[i * dim + k] * matriz2[k*dim + j];
            }
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[]){
    pthread_t *tid;
    double inicio, fim;

    GET_TIME(inicio);
    //leitura e avalicação dos parâmetros de comando [sequencial]

    if (argc < 3){
        printf("Digite: %s <dimensão da matriz> <número de threads>\n", argv[0]);
        return 1;
    }
    dim = atoi(argv[1]);
    nthreads = atoi(argv[2]);
    if (nthreads > dim) nthreads = dim;

    int id[nthreads];

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
        //alocação das estruturas
    tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    if (tid == NULL){
        puts("ERRO--malloc");
        return 2;
    }
        //criação das threads
    for (int i = 0; i < nthreads; i++){
        id[i] = i;
        if (pthread_create(tid + i, NULL, tarefa, (void *) &id[i])){
            puts("ERRO--pthread_create");
            return 3;
        }
    }

        //espera pelo término das threads
    for (int i = 0; i < nthreads; i++){
        pthread_join(*(tid + i), NULL);
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

    //liberação de memória [sequencial]
    GET_TIME(inicio);

    free(matriz1);
    free(matriz2);
    free(saida);
    free(tid);
    
    GET_TIME(fim);
    printf("Tempo finalização: %lf\n", fim - inicio);

    return 0;
}