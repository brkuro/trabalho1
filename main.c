#include <stdio.h>
#include "ordenacao.h"

int main() {
    int tam = 1000;
    unsigned int seed = 42;

    // --- TESTE 1: Algoritmo que RETORNA as métricas (ex: Seleção) ---
    r *vet1 = geraAleatorios(tam, seed); // 
    met *m1 = selecao(vet1, tam);         // 

    printf("--- Resultados: Selecao ---\n");
    imprimeMetricas(m1);                 // 

    liberaVetor(vet1);                   // 
    liberaMetricas(m1);                  // 


    // --- TESTE 2: Algoritmo que RECEBE as métricas por parâmetro (ex: MergeSort) ---
    r *vet2 = geraAleatorios(tam, seed); // 
    met *m2 = alocaMetricas();           // 

    // O MergeSort ordena in-place e atualiza a struct m2
    mergeSort(vet2, 0, tam - 1, m2);     // 

    printf("\n--- Resultados: MergeSort ---\n");
    imprimeMetricas(m2);                 // 

    liberaVetor(vet2);                   // 
    liberaMetricas(m2);                  // 

    return 0;
}