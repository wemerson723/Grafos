#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

struct Edge {
  int u;  
  int v;  
  int w;  
};

struct Graph {
  int V;        
  int E;        
  struct Edge *edge;  
};

void display(int arr[], int size, int src, char TXT[], int pS) {

  FILE *file;
  file = fopen(TXT, "w");

  if (file == NULL) {
    printf("Erro em abrir o arquivo de saída de resultado.\n");
    return;
  } 

  for (int i = 0; i < size; i++) {
		fprintf(file, "(%d --> %d) = %d\n", src+1, i+1 , arr[i]);
    if (pS)
      printf("(%d --> %d) = %d\n", src+1, i+1 , arr[i]);
  }
	fprintf(file, "\n");
  if (pS)
    printf("\n");

	fclose(file);      
}

void bellmanford(struct Graph *g, int src, char TXT[], int pS) {

  int i, j, u, v, w;
  int tV = g->V;
  int tE = g->E;
  int d[tV];
  int p[tV];

  for (i = 0; i < tV; i++) {
    d[i] = INF;
    p[i] = 0;
  }

  d[src] = 0;

  for (i = 1; i <= tV - 1; i++) {
    for (j = 0; j < tE; j++) {
      u = g->edge[j].u;
      v = g->edge[j].v;
      w = g->edge[j].w;

      if (d[u] != INF && d[v] > d[u] + w) {
        d[v] = d[u] + w;
        p[v] = u;
      }
    }
  }

  for (i = 0; i < tE; i++) {

    u = g->edge[i].u;
    v = g->edge[i].v;
    w = g->edge[i].w;
    
    if (d[u] != INF && d[v] > d[u] + w) {
      printf("Ciclo de peso negativo encontrado.\n");
      return;
    }
  }
  display(d, tV, src, TXT, pS);
}

int main(int argc, char **argv) {

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
        return 0;
      }
      i++;
    }
    else if ( strcmp(argv[i], "-f") == 0){
      in = realloc(in, strlen(argv[i+1]));
      strcpy(in, argv[i+1]);
      file = fopen(in, "r");
      if (file == NULL) {
        printf("Erro em abrir o arquivo de entrada.\n");
        return 0;
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

  struct Graph *g = (struct Graph *)malloc(sizeof(struct Graph));
  g->V = N_NODES;  
  g->E = N_EDGES;  

  g->edge = (struct Edge *)malloc(g->E * sizeof(struct Edge));


  char C[7];
  int TO, WEIGHT = 1, FROM, ON_WEIGHT = 1;

  file = fopen(in, "r");
  fgets(C, 7, file);
  fgets(C, 7, file);

  for (int i = 0; i < 7; i++) {
      if (C[i]=='\n'&&i==4) {
        ON_WEIGHT = 0;
        break;
      }
  }

  file = fopen(in, "r");
  fgets(C, 4, file);
  int i = 0;
  if (ON_WEIGHT){
    while (fscanf(file, "%d %d %d", &FROM, &TO, &WEIGHT) != EOF) {
      g->edge[i].u = FROM-1;
      g->edge[i].v = TO-1;
      g->edge[i].w = WEIGHT;
      i++;
    }
  }
  else{
    while (fscanf(file, "%d %d", &TO, &FROM) != EOF) {
      g->edge[i].u = FROM-1;
      g->edge[i].v = TO-1;
      g->edge[i].w = WEIGHT; 
      i++;
    }
  }    
  fclose(file);

  bellmanford(g, frist-1, Out, pS);  
  return 0;
}