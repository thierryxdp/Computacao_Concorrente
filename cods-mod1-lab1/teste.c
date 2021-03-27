#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define NELEM 1000000000 // 1e9
int main(void){
    clock_t begin = clock();
    int *vetor = (int *) malloc (NELEM*sizeof(int));
    int i;
    for (i = 0; i < NELEM; i++){
        vetor[i] = 0;
    }

    for (i = 0; i < NELEM; i++){
        vetor[i]++;
    }

    
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Tempo de execução: %f\n", time_spent);
    printf("Término da thread principal! Foi usada apenas a thread principal.\n");
    return 0;
}
