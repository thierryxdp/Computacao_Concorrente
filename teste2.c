/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Codigo: Threads escrevem e leem uma variavel global: exemlo de programa com condicao de corrida */

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define NTHREADS 3

int x = 0; //variavel compartilhada entre as threads
int y = 0;

//pthread_mutex_t mutex; 

//funcao executada pela thread 1
void *t1 (void *arg) {
    int a = 0;
    while (a < 2){
        //pthread_mutex_lock(&mutex);
        x++;
        x--;
        if (x==0){
            printf("x=%d\n", x);
        }
        //pthread_mutex_unlock(&mutex);
        a++;
        printf("a=%d\n", a);
    }
    pthread_exit(NULL);
}

//funcao executada pela thread 2
void *t2 (void *arg) {
    int a = 2;
    while (a > 0){
        //pthread_mutex_lock(&mutex);
        x++;
        x--;
        if (x==0){
            printf("x=%d\n", x);
        }
        //pthread_mutex_unlock(&mutex);
        a--;
        printf("a=%d\n", a);
    }
    pthread_exit(NULL);
}

//funcao executada pela thread 3
void *t3 (void *arg) {
    //pthread_mutex_lock(&mutex);
    x--;
    x++;
    //pthread_mutex_unlock(&mutex);
    y++;
    pthread_exit(NULL);
}


//fluxo principal
int main(int argc, char *argv[]) {
    pthread_t tid[NTHREADS];
    long int id; 
    //pthread_mutex_init(&mutex, NULL);
    //cria as 3 threads
    id=3;
    if (pthread_create(&tid[2], NULL, t3, (void *)id)) return 1;
    id=2;
    if (pthread_create(&tid[1], NULL, t2, (void *)id)) return 1; 
    id=1;
    if (pthread_create(&tid[0], NULL, t1, (void *)id)) return 1;
    
    //pthread_mutex_destroy(&mutex);
    pthread_exit(NULL);
    
    return 0;
}
