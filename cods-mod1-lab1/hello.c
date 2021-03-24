/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Módulo 1 - Laboratório: 1 */
/* Codigo: "Hello World" usando threads em C */

#include <stdio.h> // para i/o
#include <stdlib.h> // talvez para dar um exit
#include <pthread.h> // para gerenciar threads

#define NTHREADS  10 //total de threads a serem criadas

//--funcao executada pelas threads
void *PrintHello (void *arg) { // por padrão a assinatura da função será retornar ponteiro para void e receber como argumento ponteiro para void

  printf("Hello World\n"); // printa "Hello, World"

  pthread_exit(NULL); // termina a execução da thread de forma manual
}

//--funcao principal do programa
int main(void) {
  pthread_t tid_sistema[NTHREADS]; //identificadores das threads no sistema
  int thread; //variavel auxiliar

  for(thread=0; thread<NTHREADS; thread++) { // vai criar NTHREADS 
    printf("--Cria a thread %d\n", thread); 
    if (pthread_create(&tid_sistema[thread], NULL, PrintHello, NULL)) { // cria a thread passando o identificador por referência, visto que a função PrintHello modificará este valor
      printf("--ERRO: pthread_create()\n"); exit(-1);
    }
  }

  printf("--Thread principal terminou\n");

  pthread_exit(NULL);
}
