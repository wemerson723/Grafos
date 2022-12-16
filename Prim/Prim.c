#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int READ_ADJ(int N_NODES, int GRAFO[N_NODES][N_NODES], char TXT[]){

    char C[7];
    int TO, WEIGHT = 1, FROM, ON_WEIGHT = 1;
    FILE *file;

    for (int i = 0; i < N_NODES; i++){
        for (int j = 0; j < N_NODES; j++){
            GRAFO[i][j] = 0;
        }
    }

    file = fopen(TXT, "r");
    fgets(C, 7, file);
    fgets(C, 7, file);

    for (int i = 0; i < 7; i++) {
        if (C[i]=='\n'&&i==4) {
            ON_WEIGHT = 0;
            break;
        }
    }

    file = fopen(TXT, "r");
    fgets(C, 4, file);

    if (ON_WEIGHT){
        while (fscanf(file, "%d %d %d", &FROM, &TO, &WEIGHT) != EOF) {
            GRAFO[FROM-1][TO-1] = WEIGHT;
            GRAFO[TO-1][FROM-1] = WEIGHT;
        }
    }
    else{
        while (fscanf(file, "%d %d", &TO, &FROM) != EOF) {
            GRAFO[FROM-1][TO-1] = 1; 
            GRAFO[TO-1][FROM-1] = 1; 
        }
    }
    fclose(file);
    return GRAFO[N_NODES][N_NODES];
}

void READ_HEADER(int *N_NODES, int *N_EDGES, char TXT[]){

    FILE *file;
    file = fopen(TXT, "r");

    fscanf(file, "%d %d", N_NODES, N_EDGES);

    if (file == NULL) {
        printf("Erro em abrir o arquivo de entrada.\n");
    }
    fclose(file);
}

int minKey(int SIZE, int key[], bool mstSet[]){
	int min = INT_MAX, min_index;

	for (int v = 0; v < SIZE; v++)
		if (mstSet[v] == false && key[v] < min)
			min = key[v], min_index = v;

	return min_index;
}

int printMST(int SIZE, int parent[], int graph[][SIZE], char TXT[], int pS){
	FILE *file;
    file = fopen(TXT, "w");

    if (file == NULL) {
        printf("Erro em abrir o arquivo de saída de resultado.\n");
        return 0;
    } 

	fprintf(file, "Arestas da Árvore:\n");
	if (pS)
		printf("Arestas da Árvore:\n");

	for (int i = 1; i < SIZE; i++){
		fprintf(file, "(%d - %d) ", i, parent[i]); 
		if (pS)
			printf("(%d - %d) ", i, parent[i]);}
			
	fprintf(file, "\n");
	if (pS)
		printf("\n");

	fclose(file);    
}

void primMST(int SIZE,int graph[][SIZE], char TXT[], int pS){
	int parent[SIZE];
	int key[SIZE];
	bool mstSet[SIZE];

	for (int i = 0; i < SIZE; i++)
		key[i] = INT_MAX, mstSet[i] = false;

	key[0] = 0;
	parent[0] = -1; 

	for (int count = 0; count < SIZE - 1; count++) {
		int u = minKey(SIZE, key, mstSet);

		mstSet[u] = true;

		for (int v = 0; v < SIZE; v++)
			if (graph[u][v] && mstSet[v] == false
				&& graph[u][v] < key[v])
				parent[v] = u, key[v] = graph[u][v];
	}
	printMST(SIZE, parent, graph, TXT, pS);
}
void main(int argc, char **argv){
	
    FILE *file;
    int frist = 1;
    int last = 0;
    int pS = 0;
    int N_NODES, N_EDGES;
    char *in = malloc(sizeof(char));
    char *Out= malloc(sizeof(char));
    in = realloc(in, strlen("In.txt"));
    strcpy(in, "In.txt");
    Out = realloc(Out, strlen("Out.txt"));
    strcpy(Out, "Out.txt");

    for (int i = 0; i < argc; i++){
        if ( strcmp(argv[i], "-h") == 0){
        printf("\n\t%s\t\t%s\n\t%s\t%s\n\t%s\t%s\n\t%s\t\t%s\n\t%s\t%s\n\t%s\t%s\n\n\tAVISOS:\n\t\tTenha certeza que entrada esteja após o parâmetro.\n\t\tEXEMPLO: -o TXT.txt\n\n\t\tTenha certeza que não há arestas repetidas na entrada de dados.\n\n"
            ,"-h",            ": mostra o help"
            ,"-o <arquivo>",  ": redireciona a saida para o ‘‘arquivo’’"
            ,"-f <arquivo>",  ": indica o ‘‘arquivo’’ que contém o grafo de entrada"
            ,"-s",            ": mostra a solução (em ordem crescente)"
            ,"-i <int>",      ": vértice inicial (dependendo do algoritmo)"
            ,"-l <int>",      ": vértice final (dependendo do algoritmo)");

        }
        else if ( strcmp(argv[i], "-o") == 0){
            Out = realloc(Out, strlen(argv[i+1]));
            strcpy(Out, argv[i+1]);
            file = fopen(Out, "r");
            if (file == NULL) {
                printf("Erro em abrir o arquivo de saída.\n");
                return;
            }
            i++;
        }
        else if ( strcmp(argv[i], "-f") == 0){
            in = realloc(in, strlen(argv[i+1]));
            strcpy(in, argv[i+1]);
            file = fopen(in, "r");
            if (file == NULL) {
                printf("Erro em abrir o arquivo de entrada.\n");
                return;
            }
            i++;
        }
        else if ( strcmp(argv[i], "-s") == 0){
            pS = 1;
        }
        else if ( strcmp(argv[i], "-i") == 0){
            frist = atoi(argv[i+1]);
            i++;
        }
        else if ( strcmp(argv[i], "-l") == 0){
            last = atoi(argv[i+1]);
            i++;
        }
    }

    READ_HEADER(&N_NODES, &N_EDGES, in);
    int GRAFO[N_NODES][N_NODES];
    READ_ADJ(N_NODES, GRAFO, in);
    primMST(N_NODES, GRAFO, Out, pS);
}