/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Aluno: Thierry Pierre Dutoit */
/* Codigo: Comunicação entre threads usando variável compartilhada e sincronização com semáforos */


// Bibliotecas necessárias
#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>

// Teremos 4 mensagens cada uma feita por 1 thread, por isso nthreads = 4
#define NTHREADS 4 

sem_t cond23, cond1;     //semaforos para sincronizar a ordem de execucao das threads

//funcao executada pela thread 1
void *t1 (void *arg) {
    // esperamos as threads 2 e 3 terminarem de executar
    sem_wait(&cond1);
    sem_wait(&cond1);
    printf("Volte sempre!\n");
    pthread_exit(NULL);
}

//funcao executada pela thread 2
void *t2 (void *arg) {
    // esperamos a thread 4 mandar o sinal
    sem_wait(&cond23);
    printf("Fique a vontade.\n");
    // manda sinal para a thread 1
    sem_post(&cond1);
    pthread_exit(NULL);
}

//funcao executada pela thread 3
void *t3 (void *arg) {
    // esperamos a thread 4 mandar o sinal
    sem_wait(&cond23);
    printf("Sente-se por favor.\n");
    // manda sinal para a thread 1
    sem_post(&cond1);
    pthread_exit(NULL);
}

//funcao executada pela thread 4
void *t4 (void *arg) {
    printf("Seja bem-vindo!\n");
    // Manda dois sinais para que t2 e t3 executem. Como elas esperam o mesmo sinal, haverá condição de corrida para ver qual printa a mensagem primeiro
    sem_post(&cond23);
    sem_post(&cond23);
    pthread_exit(NULL);
}


int main(void){
    // variáveis para definir os id's das threads
    pthread_t *tid;
    long id = 0;

    //Inicializa os id's das threads
    tid = (pthread_t *) malloc(sizeof(pthread_t) * NTHREADS);
    if (tid == NULL){
        printf("--ERRO: malloc()\n"); 
        exit(-1);
    }

    //inicia os semaforos
    sem_init(&cond23, 0, 0); // inicia com 0 pois espera t4 somar em 2 o valor de cond23
    sem_init(&cond1, 0, 0); // inicia em 0 e espera t2 e t3 somarem em 2 o valor de cond1

    //cria as tres threads
    if (pthread_create((tid + id++), NULL, t2, NULL)) { printf("--ERRO: pthread_create()\n"); exit(-1); }
    if (pthread_create((tid + id++), NULL, t3, NULL)) { printf("--ERRO: pthread_create()\n"); exit(-1); }
    if (pthread_create((tid + id++), NULL, t1, NULL)) { printf("--ERRO: pthread_create()\n"); exit(-1); }
    if (pthread_create((tid + id++), NULL, t4, NULL)) { printf("--ERRO: pthread_create()\n"); exit(-1); }

    //--espera todas as threads terminarem
    for (int t=0; t<NTHREADS; t++) {
        if (pthread_join(*(tid + t), NULL)) {
            printf("--ERRO: pthread_join() \n"); exit(-1); 
        } 
    } 

    return 0;
}