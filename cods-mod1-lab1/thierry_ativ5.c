// Questão: Implementar um programa concorrente, com duas threads (alem da thread principal), para incrementar de 1 cada elemento de um vetor de 10000 elementos.

//Roteiro:
    // 1. Comece pensando em como dividir a tarefa de incrementar todos os elementos do
    //vetor entre duas threads. Ambas as threads deverao executar a mesma funçao.

    // 2. Na função main, faça  a inicialização do vetor; crie as duas threads; aguarde o
    //termino da execução das threads criadas e verifique se os valores finais do vetor
    //estão corretos.

    // 3. Execute o programa várias vezes e verifique se ele está funcionando corretamente.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS 2
#define NELEM 10000

int vetor[NELEM];

typedef struct parte_vetor {
    int comeco;
    int fim;
} pv;

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
        vetor[i] = -1;
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

    for (i = 0; i < NELEM; i++){
        printf("%d ", vetor[i]);
    }
    putchar('\n');

    printf(("Término da thread principal!\n"));
    return 0;
}