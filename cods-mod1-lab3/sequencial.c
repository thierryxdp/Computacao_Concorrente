// Solução sequencial para calcular o valor de pi

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "timer.h"
#include <pthread.h>

long long int n; // Numero de elementos que aceita valores grandes


int main(int argc, char *argv[]){ 
    double soma = 0; // soma que resultará numa aproximação de pi
    double inicio, fim; // para calcular o tempo de execução
    if (argc < 2){
        fprintf(stderr, "Digite: %s <Numero de elementos>\n", argv[0]);
        return 1;
    }
    n = atoll(argv[1]);

    GET_TIME(inicio);
    for (double i = 0; i < n; i++){ // calcula aproximação de valor de pi
        soma += (1.0/((i*2) + 1) * pow(-1, i));
    }
    soma *= 4;
    GET_TIME(fim);
    printf("Tempo de execução sequencial: %lf\n", fim - inicio);
    printf("Valor de pi sequencial: %.15lf\n", soma);
    printf("Valor de pi da biblioteca: %.15lf\n", M_PI);
    printf("Erro: %.15lf\n", fabs(soma - M_PI)); // valor absoluto para double
    return 0;
}