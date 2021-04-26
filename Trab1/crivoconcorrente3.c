#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "timer.h"

typedef long long int ll;   // para evitar overflow porém perde-se mt desempenho com acesso à memória

ll n;               // número de elementos
ll nthreads;        // número de threads
ll *v;              // vetor que cuida dos primos restantes
ll prime = 3;       // começamos com o fator 3
ll count = 0;       // variável para controle da ordenação das threads

typedef struct {    // struct para divisão do vetor de ímpares entre as threads
    ll id;
    ll begin;
    ll end;
} tArgs;

// Variáveis para controle de exclusão mútua

pthread_mutex_t mutex;          
pthread_cond_t condvar_root;    // processo raiz
pthread_cond_t condvar_child;   // processos filhos

void * tarefa (void *arg){
    tArgs *processo = (tArgs *) arg;
    
    
    ll low_value = (processo->begin*2 + 1); // menor valor do range da thread
    ll high_value = (processo->end*2 + 1);  // maior valor do range da thread
    
    while (prime * prime <= n){
        ll first_value;                     // determina o primeiro valor da parte específica da threads
        if (prime * prime > low_value){
            first_value = prime * prime;
        } else {
            if (low_value % prime == 0) {
                first_value = low_value;
            } else {
                first_value = low_value;
                while (first_value % prime != 0) first_value += 2; // precisa melhorar o desempenho dessa solução
            }
        }

        // corta os múltiplos

        for (ll j = first_value; j < high_value; j+= 2 * prime){
            v[(j-1)/2] = 0;
        }

        
        if (processo->id == 0){ // parte específica do processo raiz

            pthread_mutex_lock(&mutex);
            while (count != nthreads - 1){
                pthread_cond_wait(&condvar_root, &mutex);
            }
            prime += 2;
            while (v[(prime-1)/2] == 0) prime += 2;
            count = 0;
            pthread_cond_broadcast(&condvar_child);
            pthread_mutex_unlock(&mutex);

        } else {                // parte específica das raízes filhas

            pthread_mutex_lock(&mutex);
            pthread_cond_signal(&condvar_root);
            count++;
            if (count != 0){
                pthread_cond_wait(&condvar_child, &mutex);
            }
            
            pthread_mutex_unlock(&mutex);
        }

    }
    
    pthread_exit(NULL);
}
int main(int argc, char *argv[]){

    double inicio, fim; // para calcular o tempo de execução
    pthread_t *tid;     // identificadores das threads
    tArgs *args;        // possui os atributos de cada thread

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condvar_root, NULL);
    pthread_cond_init(&condvar_child, NULL);

    if (argc < 3){
        fprintf(stderr, "Digite: %s <Numero de elementos> <Numero de Threads>\n", argv[0]);
        return 1;
    }

    n = atoll(argv[1]);         // converte número de elementos para inteiro
    nthreads = atoll(argv[2]);   // converte número de threads para inteiro

    if (nthreads > n) nthreads = n; // Não podemos ter mais threads que número de elementos
    
    // Inicializando as estruturas das threads
    
    tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
    if (tid == NULL){
        fprintf(stderr, "Erro--malloc\n");
        return 3;
    }

    args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
    if (args == NULL){
        fprintf(stderr, "Erro--malloc\n");
        return 3;
    }

    v = (ll *) malloc(sizeof(ll) * (n-1)/2);
    if (v == NULL){
        fprintf(stderr, "Erro--malloc\n");
        return 3;
    }

    // Define os atributos de cada Thread

    for (ll i = 0; i < nthreads; i++){
        args[i].id = i;
        args[i].begin = i*(n-1)/(2*nthreads);
        args[i].end = (i+1)*(n-1)/(2*nthreads);
    }

    /* Precisamos verificar se o processo 0 (Processo root) possui valores até sqrt(n) para que apenas ele gere os fatores.
    Não lidamos com o caso em que essa condição não é satisfeita, visto que são casos muito atípicos */

    if ((n-1)/(2*nthreads) < (int) sqrt((double) n)){
        fprintf(stderr, "Erro--Numero de processos muito grande\n");
        return 2;
    }

    // Inicializa o vetor de valores ímpares

    for (ll i = 0; i < (n-1)/2; i++){
        v[i] = 1;
    } 

    // Criando as threads
    GET_TIME(inicio);
    for (ll i = 0; i < nthreads; i++){
        if (pthread_create(tid + i, NULL, tarefa, (void *) (args + i))){
            fprintf(stderr, "Erro--pthread_create\n");
            return 4;
        }
    }

    for (ll i = 0; i < nthreads; i++){
        if (pthread_join(*(tid + i), NULL)){
            fprintf(stderr, "Erro--pthread_join\n");
            return 5;
        }
    }
    GET_TIME(fim);

    // Destrói as variáveis de controle das threads
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condvar_root);
    pthread_cond_destroy(&condvar_child);


    
    // Verifica o resultando contando os números de primos encontrados
    
    int primes = 0;
    for (int i = 0; i < (n-1)/2; i++){
        if (v[i] != 0){
            primes++;  
        }
    }

    // libera as estruturas de dados...
    free(tid);
    free(args);
    free(v);

    printf("Primos encontrados menores iguais à n: %d\n", primes);
    
    printf("Tempo de execução concorrente com [%lld] threads e [%lld] elementos: %lf\n", nthreads, n, fim - inicio);

    return 0;
}