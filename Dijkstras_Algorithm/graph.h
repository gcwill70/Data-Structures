#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

typedef struct node {
  int vertex;
  int dist;
  struct node* next;
} node;
typedef struct queueNode {
  int vertex;
  struct queueNode* prev;
  struct queueNode* next;
} queueNode;
typedef struct Aux {
  queueNode* head;
  queueNode* tail;
} Aux;
node** loadMap(char* file, int* vertices);
void printPaths(node** map, int vertices, char* file);
int getDist(int x1, int y1, int x2, int y2);
void freeTable(int** arr, int size);
void Dijkstra(node** map, int vertices, int start, int end);
void initialize_single_source(int vertices, int** dist_addr, int** pred_addr, int start);
void insert(Aux* queue, int vertex);
int extractMin(Aux* queue, int* dist);
void relax(int source, int dest, node** map, int** dist, int** pred);
node* makeNode(int vertex);
int getW(int source, int dest, node** map);
void printNodes(int cur, int start, int end, int* pred);
int clip(int x1, int x2);
void push(int node1, int node2, node** map, int** pos);
void freeList(node* head);
void freeMap(node** map, int vertices);
