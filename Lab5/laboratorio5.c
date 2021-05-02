/* Autor: Thierry Pierre Dutoit */
/* Disciplina: Computação Concorrente */
/* Prof.: Silvana Rossetto */
/* Atividade: Laboratório 5: Uso de Barreira */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int n; // número de elementos
int *v; // vetor de entrada e saída

int bloqueadas = 0; // Número de threads bloqueadas

// Variáveis condicionais
pthread_mutex_t mutex;
pthread_cond_t cond;

// Função Barreira proposta pela professora Silvana Rossetto

void barreira(int n) {
    pthread_mutex_lock(&mutex); //inicio secao critica
    if (bloqueadas == n-1) { 
        //ultima thread a chegar na barreira
        pthread_cond_broadcast(&cond);
        bloqueadas=0;
    } else {
        bloqueadas++;
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex); //fim secao critica
}

void * tarefa(void * arg){
    long id = (long) arg;                       // por mais que façamos operações entre int e long, haverá conversão implícita para long
    for (long i = 1; i <= n; i *= 2){           // Valores de i incrementa em potências de 2
        int aux;                                // variável auxiliar para a soma de cada thread
        // Faz a leitura do valor e espera todas as threads lerem
        if (id - i >= 0){       
            aux = v[(id - i)];                  // pegamos o próximo valor a ser somado
            aux += v[id];                       // e somamos com o valor atual
        }

        barreira(n);                            // Espera as threads lerem para escrever, pois queremos evitar escrita em valores ainda não lidos

        // Escreve os resultados no vetor e espera todas as threads escreverem

        if (id - i >= 0){
            v[id] = aux;                        // Atualizamos o valor daquela posição do vetor
        }

        barreira(n);                            // Não podemos ler valores desatualizados
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){

    if (argc < 2){
        fprintf(stderr, "Digite: %s <Numero de elementos>\n", argv[0]);
        return 1;
    }

    n = atoi(argv[1]);

    // Checa se o valor N passado como argumento é potência de 2
    int aux = n;
    while (aux % 2 == 0) aux = aux / 2;
    if (aux != 1){
        fprintf(stderr, "Valor n passado não é potência de 2\n");
        return 2;
    }

    // Inicia as variáveis condicionais

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    // Cria vetor de entrada simples que resultará em soma prefixa do último elemento igual a N

    v = (int *) malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++) v[i] = 1;
    
    // Cria os identificadores das N threads

    pthread_t *pid;
    pid = (pthread_t *) malloc(sizeof(pthread_t) * n);

    // Cria as Threads
    
    for (long i = 0; i < n; i++){
        if (pthread_create(pid + i, NULL, tarefa, (void *) i)){
            fprintf(stderr, "Erro--pthread_create\n");
            return 3;
        }
    }

    // O fluxo da main espera o término das threads

    for (int i = 0; i < n; i++){
        if (pthread_join(*(pid + i), NULL)){
            fprintf(stderr, "ERRO--pthread_join\n");
            return 4;
        }
    }

    // Verifica o resultado... Como definimos o vetor com valores iguais a 1, teremos uma sequência de 1 a n
    int sum = 0;
    for (int i = 0 ; i < n; i++){
        sum += 1;
        if (v[i] != sum){
            printf("Rultado Incorreto!\n");
            exit(1);
        }
    }
    printf("Resultado Correto!\n");
    
    // Destrói as variáveis condicionais

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    // Libera as estruturas alocadas
    free(v);
    free(pid);
    
    return 0;
}