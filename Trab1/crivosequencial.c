/* Aluno: Thierry Pierre Dutoit */
/* Disciplina: Computação Concorrente */
/* Trabalho 1 */

#include <stdio.h>
#include <stdlib.h>
#include "timer.h" // Biblioteca usada para medições de tempo de execução
#include <pthread.h>

int n;      // Numero de elementos passado na linha de comando
int *v;     // Vetor da lista de ímpares que serão riscados até sobrarem somente primos

int main(int argc, char *argv[]){ 
    double inicio, fim; // para calcular o tempo de execução
    int p;              // variável que armazena qual fator será usado para eliminar valores compostos

    if (argc < 2){
        fprintf(stderr, "Digite: %s <Numero de elementos>\n", argv[0]);
        return 1;
    }
    n = atoi(argv[1]);

    // inicializando estrutura de dados

    v = (int *) malloc(sizeof(int) * (n-1)/2);      // (n-1)/2 pois só consideramos números ímpares
    for (int i = 0; i < (n-1)/2; i++){              // Iniciamos todos com 1 pois ainda não foram eliminados
        v[i] = 1;
    } 

    // começa o crivo
    GET_TIME(inicio)
    p = 3;      // iniciamos do valor 3
    while ((p*p) <= n){
        if (v[(p-1)/2] == 0){   // verifica se já foi eliminado
            p += 2;
        } else {
            int t = p*p;        // podemos começar a riscar os múltiplos de p começando de p²
            do{
                v[(t-1)/2] = 0; // elimina
                t += 2*p;       // pula 2*p pois não temos valores pares
            } while (t <= n);
            p += 2;
        }
    }
    GET_TIME(fim);
    printf("Tempo de execução da lista de primos com [%d] elementos: %lf segundos\n", n, fim - inicio);
    
    // verifica resultado

    printf("2 "); // Não podemos nos esquecer do valor 2

    for (int i = 1; i < (n-1)/2; i++){ // printa os valores que no vetor continuam 1
        if (v[i] != 0){
            printf("%d ", 2*i + 1);    
        }
        
    }
    putchar('\n');
    

    return 0;
}
