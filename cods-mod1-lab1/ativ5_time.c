#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NTHREADS 100
#define NELEM 1000000000 // 1e9

int vetor[NELEM];

typedef struct parte_vetor {
    int comeco;
    int fim;
} pv;

int checa_valores (void){
    int i;
    int resul = 0;
    for (i = 0; i < NELEM; i++){
        if (vetor[i] != 1){
            resul = 1;
            break;
        }
    }
    return resul;
}

void * incremento (void * arg){
    pv *thread = (pv *) arg;
    int j;
    for (j = thread->comeco; j < thread->fim; j++){
        vetor[j]++;
    }
    
    pthread_exit(NULL);
}
int main(void){
    int i;
    pthread_t tid[NTHREADS];
    for (i = 0; i < NELEM; i++){
        vetor[i] = 0;
    }
    pv *thread = (pv *) malloc(NTHREADS * sizeof(pv));
    if (thread == NULL){
        printf("--ERRO: malloc()\n"); 
        exit(-1);
    }
    for (i = 0; i < NTHREADS; i++){
        thread[i].comeco = i*(NELEM/NTHREADS);
        thread[i].fim = (i+1)*(NELEM/NTHREADS);
    }
    clock_t begin = clock();
    for (i = 0; i < NTHREADS; i++){
        if (pthread_create(&tid[i], NULL, incremento, (void *) &thread[i])){
            printf("--ERRO: pthread_create()\n"); 
            exit(-1);
        }

    }

    for (i = 0; i < NTHREADS; i++){
        if (pthread_join(tid[i], NULL)) {
            printf("--ERRO: pthread_join() \n"); 
            exit(-1); 
        } 
    }
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC; // calcula o tempo gasto
    printf("Tempo de execução: %f\n", time_spent);

    free(thread);

    if (checa_valores()) printf("Resultado Incorreto\n");
    else printf("Resultado Correto!\n");

    printf("Término da thread principal! Foram utilizadas %d threads!\n", NTHREADS);
    return 0;
}