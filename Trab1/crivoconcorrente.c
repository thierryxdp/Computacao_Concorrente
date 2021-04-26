#include <stdio.h>
#include <stdlib.h>
#include "timer.h"
#include <pthread.h>

int n;      // número de elementos
int *v;     // vetor de ímpares
int nthreads;

void * tarefa (void * arg){         // função que as threads executarão
    int vi = *(int *) arg;          // recebe fator inicial para intercalação de valores
    while (vi* vi <= n){            // enquanto fator for menor que raiz(n)
        if (v[(vi - 1)/2] == 0){    // Se já estiver riscado, pula para o próximo fator
            vi += 2 * nthreads;     // Precisamos pular as outras threads, e visto que são apenas ímpares, multiplicamos por 2
        } else {
            int t = vi * vi;        // Começa a riscar os múltiplo, assim como no sequencial
            do{
                v[(t-1)/2] = 0;
                t += 2 * vi;
            } while (t <= n);
            vi += 2 * nthreads;
        }
    }

   pthread_exit(NULL);
}

int main(int argc, char *argv[]){ 
    double inicio, fim; // para calcular o tempo de execução
    pthread_t *tid;     // identificadores das threads

    if (argc < 3){
        fprintf(stderr, "Digite: %s <Numero de elementos> <Numero de Threads>\n", argv[0]);
        return 1;
    }

    n = atoi(argv[1]);         // converte número de elementos para inteiro
    nthreads = atoi(argv[2]);   // converte número de threads para inteiro

    if (nthreads > n) nthreads = n; // Não podemos ter mais threads que número de elementos

    int begin[nthreads];    // Armazena o fator inicial para cada thread

    // inicializando estrutura de dados

    tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads); 

    v = (int *) malloc(sizeof(int) * (n-1)/2);
    for (int i = 0; i < (n-1)/2; i++){
        v[i] = 1;
    } 

    // Criação das Threads
    GET_TIME(inicio);
    for (int i = 0; i < nthreads; i++){
        begin[i] = (i+1)*2 + 1; // Começamos a partir do 3
        if (pthread_create(tid + i, NULL, tarefa, (void *) &begin[i])){
            fprintf(stderr, "Erro--pthread_create\n");
            return 3;
        }
    }
    
    // Espera pelo término das threads
    for (int i = 0; i < nthreads; i++){
        if (pthread_join(*(tid + i), NULL)){
            fprintf(stderr, "ERRO--pthread_join\n");
            return 4;
        }
    }
    GET_TIME(fim);

    // verifica resultado contando os números primos achados
    
    int count = 0;
    for (int i = 0; i < (n-1)/2; i++){
        if (v[i] != 0){
            count++;  
        }
    }
    printf("Primos encontrados menores iguais à n: %d\n", count);
    
    printf("Tempo de execução concorrente com [%d] threads e [%d] elementos: %lf\n", nthreads, n, fim - inicio);

    return 0;
}