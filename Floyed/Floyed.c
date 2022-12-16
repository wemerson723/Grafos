#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define INF 99999

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
        }
    }
    else{
        while (fscanf(file, "%d %d", &TO, &FROM) != EOF) {
            GRAFO[FROM-1][TO-1] = 1; 
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

void convert(int SIZE,int G[SIZE][SIZE], int G_[][SIZE]){
    for (int i = 0; i < SIZE; i++){
        for (int j = 0; j < SIZE; j++){
                 if (i==j)          { G_[i][j] = 0;       }
            else if (G[i][j]==0)    { G_[i][j] = INF;     }
            else                    { G_[i][j] = G[i][j]; }
        }
    }
}
void printSolution(int SIZE,int dist[][SIZE], char TXT[], int pS) {
    
    FILE *file;
    file = fopen(TXT, "w");

    if (file == NULL) {
        printf("Erro em abrir o arquivo de saída de resultado.\n");
        return;
    } 

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (dist[i][j] == INF){
		        fprintf(file, "(%2d --> %2d) = %2s\n", i+1, j+1, "NULL");
                if (pS)
		            printf("(%2d --> %2d) = %2s\n", i+1, j+1, "NULL");}
            else{
		        fprintf(file, "(%2d --> %2d) = %2d\n", i+1, j+1, dist[i][j]);
                if (pS)
		            printf("(%2d --> %2d) = %2d\n", i+1, j+1, dist[i][j]); }
        }
		fprintf(file, "\n"); 
    }
	fclose(file);    
}

void floydWarshall (int SIZE, int G[][SIZE], char TXT[], int pS) {
    int graph[SIZE][SIZE];
    convert(SIZE, G, graph);

    int dist[SIZE][SIZE];

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            dist[i][j] = graph[i][j];
    for (int k = 0; k < SIZE; k++) {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++)  {
                if (dist[i][k] + dist[k][j] < dist[i][j])
                    dist[i][j] = dist[i][k] + dist[k][j];
            }
        }
    }
    printSolution(SIZE ,dist, TXT, pS);
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
    floydWarshall(N_NODES, GRAFO, Out, pS);
}