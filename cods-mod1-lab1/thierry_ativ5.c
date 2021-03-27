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

#define NTHREADS 2  // Apenas duas threads sendo utilizadas
#define NELEM 10000 // Vetor com 1e5 elementos

int vetor[NELEM]; // vetor num escopo global para que as threads o acessem facilmente

typedef struct parte_vetor { // struct para passar o início e o fim do vetor onde cada thread incrementará
    int comeco;
    int fim;
} pv;

int checa_valores (void){ // Função para verificar a corretude dos valores. Como o vetor inicia com tudo 0, espera-se que no final tenhamos tudo 1.
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

void * incremento (void * arg){ // Incrementa o vetor inteiro em 1 usando threads
    pv *thread = (pv *) arg; // faz o cast de ponteiro de void para ponteiro da struct
    int j;
    for (j = thread->comeco; j < thread->fim; j++){ // vai da posição inicio até a posição fim - 1.
        vetor[j]++;
    }
    pthread_exit(NULL);
}
int main(void){
    int i; // variável auxiliar
    pthread_t tid[NTHREADS]; // identificadores das threads
    for (i = 0; i < NELEM; i++){ // inicializa o vetor
        vetor[i] = 0;
    }
    pv *thread = (pv *) malloc(NTHREADS * sizeof(pv)); // Faz uma estrutura com início e fim para cada thread
    if (thread == NULL){
        printf("--ERRO: malloc()\n"); 
        exit(-1);
    }
    for (i = 0; i < NTHREADS; i++){ // Define o início e o fim de cada thread
        thread[i].comeco = i*(NELEM/NTHREADS);
        thread[i].fim = (i+1)*(NELEM/NTHREADS);
    }
    
    for (i = 0; i < NTHREADS; i++){ // cria as threads
        if (pthread_create(&tid[i], NULL, incremento, (void *) &thread[i])){
            printf("--ERRO: pthread_create()\n"); 
            exit(-1);
        }

    }

    for (i = 0; i < NTHREADS; i++){ // Faz com que a thread principal espere o término de todas as threads
        if (pthread_join(tid[i], NULL)) {
            printf("--ERRO: pthread_join() \n"); 
            exit(-1); 
        } 
    }

    free(thread); // libera a struct criada pois o incremento do vetor já terminou
    
    if (checa_valores()) printf("Resultado Incorreto\n"); // verifica se os valores estão corretos
    else printf("Resultado Correto!\n");

    printf(("Término da thread principal!\n")); // Não precisamos ter pthread_exit(NULL) pois temos um join antes.
    return 0;
}