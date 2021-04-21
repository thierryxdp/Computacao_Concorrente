/* Disciplina: Computacao Concorrente */
/* Aluno: Thierry Pierre Dutoit */
/* Laboratório: 4 ; Atividade: 4*/

/* Objetivo: Projetar e implementar um programa concorrente onde a ordem de execução das threads é controlada no programa. */
/* Roteiro: Implemente um programa com 4 threads:
• A thread 1 imprime a frase “Volte sempre!”.
• A thread 2 imprime a frase “Fique a vontade.”
• A thread 3 imprime a frase “Sente-se por favor.”
• A thread 4 imprime a frase “Seja bem-vindo!”
A ordem de impressao das mensagens deverá ser:  
• A thread 4 deve sempre imprimir sua mensagem antes das threads 2 e 3.
• A ordem em que as threads 2 e 3 imprimem suas mensagens nao importa, mas ambas devem sempre imprimir suas mensagens antes da thread 1. */


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS 4  // Numéro de Threads no Programa

int x = 0;                  // Variável global de controle da ordem das threads
pthread_mutex_t mutex;      // variável para contornar as seções críticas
pthread_cond_t condvar;     // variável de condição da ordenação das threads

void * T1 (void * arg){     // Thread 1
    
    pthread_mutex_lock(&mutex);     // precisamos envolver a seção crítica pois acessa variável compartilhada
    while (x != 3){                 // Não podemos imprimir a última mensagem antes das outras threads terem terminado
        pthread_cond_wait(&condvar, &mutex);    // Então bloqueamos a thread
    }
    pthread_mutex_unlock(&mutex);   // saímos da seção crítica
    printf("Volte sempre!\n");      // printamos a mensagem, e ela não precisa estar na seção crítica pois é a última thread restante
    pthread_exit(NULL);             // termina a thread

}

void * T2 (void * arg){     // Thread 2
    
    pthread_mutex_lock(&mutex);     // Entramos na seção crítica
    if (x == 0) {                   // Precisamos que a mensagem inicial tenha sido impressa
        pthread_cond_wait(&condvar, &mutex);    // Caso contrário bloqueamos a thread
    }
    x++;                            // Aumentamos o valor de x pois podemos printar a mensagem
    printf("Fique a vontade.\n");   // Imprime a mensagem
    pthread_cond_signal(&condvar);  // Caso a Thread 1 esteja bloqueada mandamos um sinal desbloqueando
    pthread_mutex_unlock(&mutex);   // Seção crítica termina
    pthread_exit(NULL);             // Termina a Thread

}
void * T3 (void * arg){     // Thread 3
    pthread_mutex_lock(&mutex);     // Entramos na seção crítica
    if (x == 0) {                   // Precisamos que a mensagem inicial tenha sido impressa
        pthread_cond_wait(&condvar, &mutex);    // Caso contrário bloqueaamos a thread
    }
    x++;                            // Aumentamos o valor de x pois iremos printar a mensagem
    printf("Sente-se por favor.\n");// Imprime a mensagem
    pthread_cond_signal(&condvar);  // Caso a Thread 1 esteja bloqueada mandamos um sinal desbloqueando
    pthread_mutex_unlock(&mutex);   // Fim da seção crítica
    pthread_exit(NULL);             // Termina a Thread
}
void * T4 (void * arg){     // Thread 4

    pthread_mutex_lock(&mutex);     // Entra na seção crítica
    x++;                            // Por ser a primeira mensagem não precisamos testar condição
    printf("Seja bem-vindo!\n");    // Imprime a primeira mensagem
    pthread_cond_broadcast(&condvar); // Liberamos todas as threads que estão bloqueadas visto que elas dependem do término dessa Thread
    pthread_mutex_unlock(&mutex);   // Término da seção crítica

    pthread_exit(NULL);             // Termina a Thread
}

int main(void){         // Fluxo principal
    
    pthread_t threads[NTHREADS];    // Identificadores para as Threads

    pthread_mutex_init(&mutex, NULL);   // Inicializa a variável de lock
    pthread_cond_init (&condvar, NULL); // Inicializa a variável condicional
    
    /* Cria as threads sem verificação */
    pthread_create(&threads[0], NULL, T1, NULL);    
    pthread_create(&threads[1], NULL, T2, NULL);
    pthread_create(&threads[2], NULL, T3, NULL);
    pthread_create(&threads[3], NULL, T4, NULL);

    /* Fluxo principal espera pelo término das threads*/
    for (int i = 0; i < NTHREADS; i++){
        pthread_join(threads[i], NULL);
    }
    
    /* Não precisamos mais das variáveis de ordenação das threads*/
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condvar);


    return 0; // Fim do programa
}