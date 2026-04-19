#include <stdio.h>
<<<<<<< HEAD
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "ordenacao.h"

// Estrutura para consolidar os resultados de cada teste individual
typedef struct {
    double tempo;
    unsigned long long comparacoes;
    unsigned long long movimentacoes;
} Resultado;

// IDs para identificar os algoritmos no sistema
typedef enum {
    BOLHA = 1, SELECAO, INSERCAO, MERGE, QUICK, SHELL, HEAP, BOZO
} AlgoritmoID;

// Protótipos das funções do sistema de benchmarking
Resultado executa_teste(int tam, int cenario, unsigned int seed, AlgoritmoID alg);
void realizar_benchmark(AlgoritmoID alg, char *nomeAlg);

int main() {
    
    printf("SISTEMA DE BENCHMARKING - ALGORITMOS DE ORDENACAO\n");
    printf("Iniciando bateria de testes...\n\n");

    // Exemplo de execução para dois algoritmos
    realizar_benchmark(SELECAO, "Selecao");
    

    printf("\nBenchmark concluido com sucesso. Verifique os arquivos .csv gerados.\n");
    return 0;
}

/**
 * Executa o benchmark completo para um algoritmo em todos os cenários e tamanhos.
 */
void realizar_benchmark(AlgoritmoID alg, char *nomeAlg) {
    int tamanhos[] = {1000, 5000, 10000}; // Defina os N do seu projeto
    int n_tamanhos = 3;
    unsigned int seed_base = 2026;
    
    char nomeArquivo[100];
    sprintf(nomeArquivo, "relatorio_%s.csv", nomeAlg);
    FILE *f = fopen(nomeArquivo, "w");
    
    if (!f) return;

    // 
    fprintf(f, "Tamanho;Aleatorio (Med);Crescente;Decrescente;Quase Ordenado (Med)\n");

    for (int i = 0; i < n_tamanhos; i++) {
        int n = tamanhos[i];
        Resultado res[4]; // 0: Aleat, 1: Cresc, 2: Decresc, 3: Quase

        // --- CENÁRIO: ALEATÓRIO (Média de 30 repetições) ---
        double soma_t = 0, soma_c = 0, soma_m = 0;
        for (int j = 0; j < 30; j++) {
            Resultado r = executa_teste(n, 0, seed_base + j, alg);
            soma_t += r.tempo; soma_c += r.comparacoes; soma_m += r.movimentacoes;
        }
        res[0].tempo = soma_t / 30.0;
        res[0].comparacoes = (unsigned long long)(soma_c / 30);
        res[0].movimentacoes = (unsigned long long)(soma_m / 30);

        // --- CENÁRIO: CRESCENTE (1 repetição) ---
        res[1] = executa_teste(n, 1, seed_base, alg);

        // --- CENÁRIO: DECRESCENTE (1 repetição) ---
        res[2] = executa_teste(n, 2, seed_base, alg);

        // --- CENÁRIO: QUASE ORDENADO (Média de 30 repetições) ---
        soma_t = 0; soma_c = 0; soma_m = 0;
        for (int j = 0; j < 30; j++) {
            Resultado r = executa_teste(n, 3, seed_base + j, alg);
            soma_t += r.tempo; soma_c += r.comparacoes; soma_m += r.movimentacoes;
        }
        res[3].tempo = soma_t / 30.0;

        // Salva a linha de tempo no CSV (pode criar outros arquivos para comparacoes/movimentacoes)
        fprintf(f, "%d;%.6f;%.6f;%.6f;%.6f\n", n, res[0].tempo, res[1].tempo, res[2].tempo, res[3].tempo);
    }

    fclose(f);
}

Resultado executa_teste(int tam, int cenario, unsigned int seed, AlgoritmoID alg) {
    r *vet;
    met *m = NULL;
    Resultado res = {0, 0, 0};
    clock_t inicio, fim;

    // 1. Geração do Dataset 
    switch(cenario) {
        case 1:  vet = geraOrdenados(tam, seed); break;
        case 2:  vet = geraDecrescente(tam, seed); break;
        case 3:  vet = geraQuaseOrdenados(tam, seed, 10); break; // 10% desordem
        default: vet = geraAleatorios(tam, seed); break;
    }

    // 2. Execução conforme a Categoria da biblioteca 
    if (alg == MERGE || alg == QUICK) {
        // Categoria B: Recebem métricas por parâmetro
        m = alocaMetricas();
        inicio = clock();
        if (alg == MERGE) mergeSort(vet, 0, tam - 1, m);
        else quickSort(vet, 0, tam - 1, m);
        fim = clock();
    } else {
        // Categoria A: Retornam ponteiro de métricas
        inicio = clock();
        switch(alg) {
            case BOLHA:    m = bolhaInteligente(vet, tam); break;
            case SELECAO:  m = selecao(vet, tam); break;
            case INSERCAO: m = insercao(vet, tam); break;
            case SHELL:    m = shellSort(vet, tam); break;
            case HEAP:     m = heapSort(vet, tam); break;
            case BOZO:     m = bozoSort(vet, tam); break;
            default: break;
        }
        fim = clock();
    }

    // 3. Coleta de dados
    res.tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
    if (m) {
        res.comparacoes = m->comparacoes; // Acessível após o ajuste no .h 
        res.movimentacoes = m->movimentacoes;
    }

    // 4. Limpeza de Memória  
    liberaVetor(vet);
    liberaMetricas(m);

    return res;
=======
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
>>>>>>> b50bc7f737ea78efbef806dc89ef38ade6d649de
}