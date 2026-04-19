#include "ordenacao.h"

int main() {
    int tam = 5000;
    unsigned int seed = 123;
    char nomeArqMetricas[50] = "resultados_met.txt";
    
    // 1. Prepara os dados
    r *vet = geraAleatorios(tam, seed); 
    met *m = alocaMetricas(); 
    
    // 2. Executa o teste (exemplo com QuickSort)
    quickSort(vet, 0, tam - 1, m); 
    
    // 3. Documenta os resultados como Arquiteto de Testes
    salvaMetricas(m, nomeArqMetricas); 
    salvaVetor(vet, tam, "vetor_final_quick.txt"); 
    // 4. Limpeza obrigatória
    liberaVetor(vet); 
    return 0;
}