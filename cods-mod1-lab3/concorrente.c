// Solução concorrente para calcular o valor de pi

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "timer.h"

typedef long long int ll;

ll n;
int nthreads;
double somaConcorrente;

void * tarefa (void * arg){
    ll id = (ll) arg;
    ll tamBloco = n/nthreads;
    ll inicio = id * tamBloco;
    ll fim;
    if (id == nthreads - 1) fim = n;
    else fim = inicio + tamBloco;

    double *somaLocal; 
    somaLocal = (double *) malloc(sizeof(double));
    if (somaLocal == NULL){
        fprintf(stderr, "Erro--malloc\n"); //print de erro do sistema
        exit(1);
    }
    *somaLocal = 0;
    
    for (double i = inicio; i < fim; i++){
        *somaLocal += (1.0/((i*2) + 1) * pow(-1, i));
    }

    pthread_exit((void *) somaLocal);
}

int main(int argc, char* argv[]){
    double *retorno;
    double inicio, fim;
    double somaConcorrente = 0;
    pthread_t *tid;
    
    if (argc < 3){
        fprintf(stderr, "Digite: %s <Numero de elementos>\n", argv[0]);
        return 1;
    }

    n = atoll(argv[1]);
    nthreads = atoi(argv[2]);

    if (nthreads > n) nthreads = n;

qqqqq
    tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    GET_TIME(inicio);
    for (ll i = 0; i < nthreads; i++){
        if (pthread_create(tid + i, NULL, tarefa, (void *) i)){
            fprintf(stderr, "Erro--pthread_create\n");
            return 3;
        }
    }

    for (ll i = 0; i < nthreads; i++){
        if (pthread_join(*(tid + i), (void **) &retorno)){
            fprintf(stderr, "ERRO--pthread_join\n");
            return 4;
        }
        //soma global
        somaConcorrente += *retorno;
        free(retorno);
    }
    somaConcorrente *= 4;
    GET_TIME(fim);
    printf("Executando com [%lli] elementos e [%d] threads\n\n", n, nthreads);

    printf("Tempo de execução concorrente: %lf\n", fim - inicio);
    printf("Valor de pi sequencial: %.15lf\n", somaConcorrente);
    printf("Valor de pi da biblioteca: %.15lf\n", M_PI);
    printf("Erro: %.15lf\n\ns", fabs(somaConcorrente - M_PI));
    free(tid);
    return 0;

}