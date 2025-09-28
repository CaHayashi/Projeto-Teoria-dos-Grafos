/*
 * Projeto - Parte 1
 * Modelagem de Bairro como Grafo
 * Integrantes: 
     Carlos Eduardo Hayashi - 10419790
     Edson Luiz Cardoso Ohira - 10419663
     Vinicius Oliveira Piccazzio - 10419471
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_VERTICES 40
#define MAX_LOCALIDADES 25
#define INF 99999

/*
 * Estruturas para Grafo
 */
typedef struct a {
    int extremo2;
    struct a *prox;
} Arest;

typedef struct v {
    int nome;
    int cor;
    Arest *prim;
} Vert;

/*
 * Estrutura de Localidade
 */
typedef struct {
    char nome[50];
    int v1, v2;   // vértices adjacentes
    int isCasa;   // 1 se for a casa
    int melhorVertice;
    int distancia;
} Localidade;

/*
 * Funções de manipulação do Grafo
 */
void criaGrafo(Vert **G, int ordem) {
    *G = (Vert*) malloc(sizeof(Vert) * ordem);
    for (int i = 0; i < ordem; i++) {
        (*G)[i].nome = i;
        (*G)[i].cor = 0;
        (*G)[i].prim = NULL;
    }
}

void destroiGrafo(Vert **G, int ordem) {
    for (int i = 0; i < ordem; i++) {
        Arest *a = (*G)[i].prim;
        while (a != NULL) {
            Arest *n = a->prox;
            free(a);
            a = n;
        }
    }
    free(*G);
}

int acrescentaAresta(Vert G[], int ordem, int v1, int v2) {
    if (v1 < 0 || v1 >= ordem) return 0;
    if (v2 < 0 || v2 >= ordem) return 0;

    Arest *A1 = (Arest*) malloc(sizeof(Arest));
    A1->extremo2 = v2;
    A1->prox = G[v1].prim;
    G[v1].prim = A1;

    if (v1 != v2) {
        Arest *A2 = (Arest*) malloc(sizeof(Arest));
        A2->extremo2 = v1;
        A2->prox = G[v2].prim;
        G[v2].prim = A2;
    }

    return 1;
}

void imprimeGrafo(Vert G[], int ordem) {
    printf("\nOrdem: %d", ordem);
    printf("\nLista de Adjacencia:\n");

    for (int i = 0; i < ordem; i++) {
        printf("\n    v%d: ", i);
        Arest *aux = G[i].prim;
        for (; aux != NULL; aux = aux->prox)
            printf("  v%d", aux->extremo2);
    }
    printf("\n\n");
}

/*
 * BFS - calcula distâncias e caminhos
 */
void bfs(Vert G[], int ordem, int origem, int *dist, int *pai) {
    int visitado[MAX_VERTICES] = {0};
    for (int i = 0; i < ordem; i++) {
        dist[i] = INF;
        pai[i] = -1;
    }

    int fila[MAX_VERTICES];
    int ini = 0, fim = 0;

    visitado[origem] = 1;
    dist[origem] = 0;
    fila[fim++] = origem;

    while (ini < fim) {
        int atual = fila[ini++];
        Arest *a = G[atual].prim;
        while (a != NULL) {
            int viz = a->extremo2;
            if (!visitado[viz]) {
                visitado[viz] = 1;
                dist[viz] = dist[atual] + 1;
                pai[viz] = atual;
                fila[fim++] = viz;
            }
            a = a->prox;
        }
    }
}

void imprimeCaminho(int destino, int *pai) {
    if (destino == -1) return;
    if (pai[destino] != -1) {
        imprimeCaminho(pai[destino], pai);
    }
    printf("v%d ", destino);
}

/*
 * Função para ordenar localidades por distância
 */
int compareLocalidades(const void *a, const void *b) {
    Localidade *la = (Localidade*)a;
    Localidade *lb = (Localidade*)b;
    return la->distancia - lb->distancia;
}

/*
 * PROGRAMA PRINCIPAL
 */
int main(int argc, char *argv[]) {
    Vert *G;
    int ordemG = 33;
    criaGrafo(&G, ordemG);

    int arestas[][2] = {
        {28,1},{1,2},{2,3},{3,4},{4,5},{5,10},{10,9},{9,8},{8,7},
        {7,6},{6,29},{29,30},{30,11},{11,12},{12,13},{13,14},{14,15},
        {15,16},{15,21},{16,22},{22,21},{21,20},{20,19},{19,18},{18,17},
        {17,31},{31,32},{32,23},{23,24},{24,25},{25,26},{26,27},{27,22},
        {16,10},{14,9},{13,8},{12,7},{11,6},{28,29},{1,6},{2,7},{3,8},
        {4,9},{30,31},{11,17},{12,18},{13,19},{14,20},{20,26},{19,25},
        {18,24},{17,23}
    };
    int numArestas = sizeof(arestas)/sizeof(arestas[0]);

    for (int i = 0; i < numArestas; i++) {
        acrescentaAresta(G, ordemG, arestas[i][0], arestas[i][1]);
    }

    printf("\n==== GRAFO CONSTRUÍDO ====\n");
    imprimeGrafo(G, ordemG);

    Localidade locais[MAX_LOCALIDADES] = {
        {"MC 1000", 1, 1, 0},
        {"Bacio Di Latte", 2, 2, 0},
        {"Metrô Consolação", 3, 4, 0},
        {"Boa Praça", 6, 6, 0},
        {"Gula Gula", 6, 7, 0},
        {"Starbucks", 4, 9, 0},
        {"Fogo de Chão", 12, 13, 0},
        {"Fortune Residence", 14, 15, 0},
        {"Studio Velocity", 22, 27, 0},
        {"Aguzzo", 20, 26, 0},
        {"Hi Pokee", 13, 19, 0},
        {"Mercure", 1, 2, 0},
        {"Ritz", 24, 25, 0},
        {"Central Park", 11, 17, 0},
        {"Peça Rara", 20, 20, 0},
        {"Piraja", 29, 29, 0},
        {"Ibis", 4, 5, 0},
        {"Renaissance", 8, 9, 0},
        {"Paroquia", 23, 24, 0},
        {"Riachuelo", 4, 4, 0},
        {"Casa", 12, 12, 1}
    };
    int numLocais = 21;

    int origem = 12;
    int dist[MAX_VERTICES];
    int pai[MAX_VERTICES];
    bfs(G, ordemG, origem, dist, pai);

    // Ajuste de distâncias para localidades entre dois vértices
    for (int i = 0; i < numLocais; i++) {
        int d1 = dist[locais[i].v1];
        int d2 = dist[locais[i].v2];

        if (locais[i].v1 == origem && locais[i].v2 != origem) {
            locais[i].distancia = 1;
            locais[i].melhorVertice = locais[i].v2;
        } else if (locais[i].v2 == origem && locais[i].v1 != origem) {
            locais[i].distancia = 1;
            locais[i].melhorVertice = locais[i].v1;
        } else {
            locais[i].distancia = (d1 < d2) ? d1 : d2;
            locais[i].melhorVertice = (d1 <= d2) ? locais[i].v1 : locais[i].v2;
        }
    }

    qsort(locais, numLocais, sizeof(Localidade), compareLocalidades);

    printf("\n===== LEGENDA (Locais Importantes) =====\n");
    for (int i = 0; i < numLocais; i++) {
        if (locais[i].v1 != locais[i].v2)
            printf("%-20s -> entre v%d e v%d\n", locais[i].nome, locais[i].v1, locais[i].v2);
        else
            printf("%-20s -> v%d\n", locais[i].nome, locais[i].v1);
    }

    printf("\n===== DISTÂNCIAS A PARTIR DA CASA (v12) =====\n\n");
    for (int i = 0; i < numLocais; i++) {
        printf("%-20s -> %2d quadras | Caminho: ", locais[i].nome, locais[i].distancia);
        imprimeCaminho(locais[i].melhorVertice, pai);
        printf("\n");
    }

// Pausa compatível Windows/Linux
#ifdef _WIN32
    system("pause");
#else
    printf("\nPressione ENTER para sair...");
    getchar();
#endif

    destroiGrafo(&G, ordemG);
    return 0;
}
