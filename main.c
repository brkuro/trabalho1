#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ordenacao.h"

// enumeração dos métodos de 0 à 6
// facilita a chamada dos métodos, pois permite o uso da estrutura switch-case
typedef enum {
    BOLHAINTELIGENTE,
    SELECAO,
    INSERCAO,
    MERGE,
    QUICK,
    SHELL,
    HEAP
} Algoritmo;

// tipo definido para armazenar os resultados do benchmark
typedef struct {
    double tempo;
    unsigned long long comparacoes;
    unsigned long long movimentacoes;
    int estavel;
} Resultado;

// le as métricas do teste armazenadas em um vetor e as copia para as variáveis comp e mov
void lerMetricasArquivo(char *nomeArquivo, unsigned long long *comp, unsigned long long *mov) {
    FILE *arq = fopen(nomeArquivo, "r");
    if (!arq) {
        *comp = 0;
        *mov = 0;
        return;
    }
    fscanf(arq, "Comparacoes: %llu\n", comp);
    fscanf(arq, "Movimentacoes: %llu\n", mov);
    fclose(arq);
}

// função que verifica a estabilidade dos métodos
// se for estável retorna 1, caso contrário retorna 0
// em caso de erro de leitura do arquivo retorna -1
int verificarEstabilidade(char *nomeArquivo) {
    int id1, t1, id2, t2;
    FILE *arq = fopen(nomeArquivo, "r");

    if (!arq || (fscanf(arq, "%d %d", &id1, &t1) != 2))
        return -1;

    while (fscanf(arq, "%d %d", &id2, &t2) == 2) {
        if (id1 == id2 && t1 > t2) {
            fclose(arq);
            return 0;
        }
        id1 = id2;
        t1 = t2;
    }
    fclose(arq);
    return 1;
}

// realiza o teste e retorna o resultado
Resultado benchmark(int tam, unsigned int seed, Algoritmo alg, int cenario) {
    r *vet;
    met *m = NULL;
    Resultado res = {0, 0, 0, 0};

    // gera os vetores para os testes
    // o valor de seed, por convenção é 42
    switch (cenario) {
        case 1:
            vet = geraOrdenados(tam, seed);
            break;
        case 2:
            vet = geraDecrescente(tam, seed);
            break;
        case 3:
            // porc% definida como 10, segundo a especificação
            vet = geraQuaseOrdenados(tam, seed, 10);
            break;
        default:
            vet = geraAleatorios(tam, seed);
            break;
    }

    // realiza os testes para o metodo indicado
    // retorna as métricas, o tempo de execusão e a estabilidade do metodo por meio do tipo Resultado
    // clock() retorna o tempo em segundos, multiplica-se por 1000.0 para obter os dados em ms
    switch (alg) {
        case BOLHAINTELIGENTE: {
            clock_t inicio = clock();
            m = bolhaInteligente(vet, tam);
            clock_t fim = clock();
            res.tempo = 1000.0 * (double)(fim - inicio) / CLOCKS_PER_SEC;
            break;
        }
        case SELECAO: {
            clock_t inicio = clock();
            m = selecao(vet, tam);
            clock_t fim = clock();
            res.tempo = 1000.0 * (double)(fim - inicio) / CLOCKS_PER_SEC;
            break;
        }
        case INSERCAO: {
            clock_t inicio = clock();
            m = insercao(vet, tam);
            clock_t fim = clock();
            res.tempo = 1000.0 * (double)(fim - inicio) / CLOCKS_PER_SEC;
            break;
        }
        case MERGE: {
            clock_t inicio = clock();
            m = alocaMetricas();
            mergeSort(vet, 0, tam - 1, m);
            clock_t fim = clock();
            res.tempo = 1000.0 * (double)(fim - inicio) / CLOCKS_PER_SEC;
            break;
        }
        case QUICK: {
            clock_t inicio = clock();
            m = alocaMetricas();
            quickSort(vet, 0, tam - 1, m);
            clock_t fim = clock();
            res.tempo = 1000.0 * (double)(fim - inicio) / CLOCKS_PER_SEC;
            break;
        }
        case SHELL: {
            clock_t inicio = clock();
            m = shellSort(vet, tam);
            clock_t fim = clock();
            res.tempo = 1000.0 * (double)(fim - inicio) / CLOCKS_PER_SEC;
            break;
        }
        case HEAP: {
            clock_t inicio = clock();
            m = heapSort(vet, tam);
            clock_t fim = clock();
            res.tempo = 1000.0 * (double)(fim - inicio) / CLOCKS_PER_SEC;
            break;
        }
    }

    // salva as métricas em um arquivo .txt
    // armazenas as métricas em Resultado
    salvaMetricas(m, "metricas.txt");
    lerMetricasArquivo("metricas.txt",&res.comparacoes,&res.movimentacoes);

    // salva um vetor com as requisições em um arquivo .txt
    // verifica a estabilidade do metodo e armazena em Resultado
    salvaVetor(vet, tam, "vetor.txt");
    res.estavel = verificarEstabilidade("vetor.txt");

    liberaVetor(vet);
    liberaMetricas(m);

    return res;
}

// realiza o benchmark para os casos de vetores aleatórios e quase ordenados
// calcula a média de 30 testes
Resultado mediaBenchmark(int tam, unsigned int seed, Algoritmo alg, int cenario) {
    int repeticoes = 30;
    Resultado soma = {0, 0, 0, 0};

    for (int i = 0; i < repeticoes; i++) {
        Resultado r = benchmark(tam, seed + i, alg, cenario);
        soma.tempo += r.tempo;
        soma.comparacoes += r.comparacoes;
        soma.movimentacoes += r.movimentacoes;
    }

    soma.tempo /= repeticoes;
    soma.comparacoes /= repeticoes;
    soma.movimentacoes /= repeticoes;

    return soma;
}

// gera tabelas (tempo, comparações, movimentações) de tipo .csv com os dados de cada teste de algoritmo
void gerarTabelas(Algoritmo alg, char *nomeAlg) {
    int tamanhos[] = {10, 100, 1000, 10000};
    int n = 4;
    unsigned int seed = 42;
    char nomeTempo[50], nomeComp[50], nomeMov[50];

    //cria as tabelas, as acessa para escrita e cria os títulos das colunas
    sprintf(nomeTempo, "%s_tempo.csv", nomeAlg);
    sprintf(nomeComp, "%s_comp.csv", nomeAlg);
    sprintf(nomeMov, "%s_mov.csv", nomeAlg);

    FILE *ft = fopen(nomeTempo, "w");
    FILE *fc = fopen(nomeComp, "w");
    FILE *fm = fopen(nomeMov, "w");

    fprintf(ft, "Teste;Tamanho;Aleatorio;Crescente;Decrescente;Quase;Estavel\n");
    fprintf(fc, "Teste;Tamanho;Aleatorio;Crescente;Decrescente;Quase;Estavel\n");
    fprintf(fm, "Teste;Tamanho;Aleatorio;Crescente;Decrescente;Quase;Estavel\n");

    // calcula estabilidade, fixo vetor de tamanho 10000
    Resultado testeEstab = benchmark(10000, seed, alg, 0);
    int estavel = testeEstab.estavel;

    // faz a chamada das funções benchmark e mediaBenchmark para obter os dados para as tabelas
    for (int i = 0; i < n; i++) {
        int tam = tamanhos[i];

        Resultado r0 = mediaBenchmark(tam, seed, alg, 0);
        Resultado r1 = benchmark(tam, seed, alg, 1);
        Resultado r2 = benchmark(tam, seed, alg, 2);
        Resultado r3 = mediaBenchmark(tam, seed, alg, 3);

        fprintf(ft, "%d;%d;%.6f;%.6f;%.6f;%.6f;%s\n",
                i + 1, tam, r0.tempo, r1.tempo, r2.tempo, r3.tempo, estavel ? "SIM" : "NAO");

        fprintf(fc, "%d;%d;%llu;%llu;%llu;%llu;%s\n",
                i + 1, tam, r0.comparacoes, r1.comparacoes, r2.comparacoes, r3.comparacoes, estavel ? "SIM" : "NAO");

        fprintf(fm, "%d;%d;%llu;%llu;%llu;%llu;%s\n",
                i + 1, tam, r0.movimentacoes, r1.movimentacoes, r2.movimentacoes, r3.movimentacoes, estavel ? "SIM" : "NAO");
    }

    fclose(ft);
    fclose(fc);
    fclose(fm);
}

int main() {
    // realiza os testes para os metodos Bolha Inteligente, Seleção, Inserção, Merge Sort, Quick Sort, Shell Sort e Heap Sort
    gerarTabelas(BOLHAINTELIGENTE, "BolhaInteligente");
    gerarTabelas(SELECAO, "Selecao");
    return 0;
}