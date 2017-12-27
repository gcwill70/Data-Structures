#include "graph.h"
#define INT_MAX 2147483647

int getDist(int x_1, int y_1, int x_2, int y_2) {
  return (int)sqrt(pow(x_1-x_2,2) + pow(y_1-y_2,2));
}
void freeTable(int** arr, int size) {
  int i;
  for(i = 0; i < size; i++) {
    free(arr[i]);
  }
  free(arr);
}
void push(int node1, int node2, node** map, int** pos) {
  node* temp = map[node1];
  map[node1] = malloc(sizeof(node));
  map[node1]->vertex = node2;
  map[node1]->dist = getDist(pos[node1][0], pos[node1][1], pos[node2][0], pos[node2][1]);
  map[node1]->next = temp;
}
void freeMap(node** map, int vertices) {
  int i;
  for(i = 0; i < vertices; i++) {
    freeList(map[i]);
  }
  free(map);
}
void freeList(node* head) {
  if(!head) {return;}
  freeList(head->next);
  free(head);
}
node** loadMap(char* file, int* vertices) {
  FILE* fptr = fopen(file, "r");
  if(!fptr) {
   printf("\nError: can't open file %s.\nExiting...\n", file);
   return NULL;
  }
  //read in vertices and edges
  int edges;
  fscanf(fptr, "%d %d\n", vertices, &edges);
  //malloc LL array
  int i, j;
  node** map = malloc(sizeof(node*)*(*vertices));
  for(i = 0; i < *vertices; i++) {
    map[i] = NULL;
  }
  //read in positions
  int nodeNum, x, y;
  int** pos = malloc(sizeof(int*)*(*vertices));;
  for(i = 0; i < *vertices; i++) {
    //read in an index and x-y coordinates
    fscanf(fptr, "%d %d %d\n", &nodeNum, &x, &y);
    //record coordinates
    pos[nodeNum] = malloc(sizeof(int)*2);
    pos[nodeNum][0] = x;
    pos[nodeNum][1] = y;
  }
  //record edges
  while(!feof(fptr)) {
    fscanf(fptr, "%d %d\n", &i, &j);
    push(i, j, map, pos);
    push(j, i, map, pos);
  }
  //free positions array
  freeTable(pos, *vertices);

  fclose(fptr);
  return map;
}
void initialize_single_source(int vertices, int** dist_addr, int** pred_addr, int start) {
  int* dist = *dist_addr;
  int* pred = *pred_addr;
  int i;
  for(i = 0; i < vertices; i++) {dist[i] = INT_MAX;}
  memset(pred, -1, vertices*sizeof(int));
  dist[start] = 0;
}
void insert(Aux* queue, int vertex) {
  if(!(queue->head)) {
    queue->head = malloc(sizeof(queueNode));
    queue->head->vertex = vertex;
    queue->head->prev = NULL;
    queue->head->next = NULL;
    queue->tail = queue->head;
    return;
  }
  //insert at end
  queue->tail->next = malloc(sizeof(queueNode));
  queue->tail->next->vertex = vertex;
  queue->tail->next->next = NULL;
  queue->tail->next->prev = queue->tail;
  queue->tail = queue->tail->next;
}
int extractMin(Aux* queue, int* dist) {
  if(!(queue->head)) {return -1;}
  queueNode* minNode = queue->head;
  queueNode* cur = queue->head;
  while(cur) {
    if(dist[minNode->vertex] > dist[cur->vertex]) {
      minNode = cur;
    }
    cur = cur->next;
  }
  //remove minNode
  if(minNode == queue->head) {
    queue->head = minNode->next;
  } else {
    minNode->prev->next = minNode->next;
    if(minNode->next) {
      minNode->next->prev = minNode->prev;
    }
  }
  int ret = minNode->vertex;
  free(minNode);
  return ret;
}
int getW(int source, int dest, node** map) {
  node* cur;
  if(source <= dest) {
    cur = map[source];
    while(cur->vertex != dest) {cur = cur->next;}
  } else {
    cur = map[dest];
    while(cur->vertex != source) {cur = cur->next;}
  }
  return cur->dist;
}
int clip(int x1, int x2) {
  if((double)x1 + (double)x2 > INT_MAX) {
    return INT_MAX;
  } else {
    return x1 + x2;
  }
}
void relax(int source, int dest, node** map, int** dist, int** pred) {
  //maxes out newDist to be INT_MAX when necessary
  int newDist = clip((*dist)[source], getW(source, dest, map));
  //assign if necessary
  if((*dist)[dest] > newDist || (*dist)[dest] == -1) {
    (*dist)[dest] = newDist;
    (*pred)[dest] = source;
  }
}
void printNodes(int cur, int start, int end, int* pred) {
  if(cur == start) {
    printf("%d ", start);
    return;
  }
  printNodes(pred[cur], start, end, pred);
  printf("%d", cur);
  if(cur != end) {
    printf(" ");
  } else {
    printf("\n");
  }
}
void Dijkstra(node** map, int vertices, int start, int end) {
  if(start >= vertices || end >= vertices) { //if query is out of bounds
    printf("INF\n");
    printf("%d %d\n", start, end);
    return;
  }
  int* dist = malloc(sizeof(int)*vertices); //distances array
  int* pred = malloc(sizeof(int)*vertices); //predecessors array
  initialize_single_source(vertices, &dist, &pred, start);
  Aux* queue = malloc(sizeof(Aux)); //auxiliary queue pointer
  queue->head = NULL;
  queue->tail = NULL;
  int i;
  for(i = 0; i < vertices; i++) {
    insert(queue, i);
  }
  int currentMin;
  while(queue->head) {
    currentMin = extractMin(queue, dist); //dequeue the vertex with the min shortest path estimate
    node* cur = map[currentMin];
    while(cur) { //for each vertex leaving currentMin
      relax(currentMin, cur->vertex, map, &dist, &pred);
      cur = cur->next;
    }
  }
  if(dist[end] == INT_MAX) { //nodes are disconnected
    printf("INF\n");
    printf("%d %d\n", start, end);
  } else {
    printf("%d\n", dist[end]);
    printNodes(end, start, end, pred);
  }

  free(queue);
  free(dist);
  free(pred);
}
void printPaths(node** map, int vertices, char* file) {
  FILE* fptr = fopen(file, "r");
  if(!fptr) {
   printf("\nError: can't open file %s.\n", file);
   return;
  }
  //read in number of queries
  int numSearches;
  if(fscanf(fptr, "%d\n", &numSearches) != 1) {
    printf("\nError: %s is not formatted properly.\n", file);
    return;
  }
  //get actual queries
  int i, start, end;
  for(i = 0; i < numSearches; i++) {
    if(fscanf(fptr, "%d %d\n", &start, &end) != 2) {
      printf("\nError: %s is not formatted properly.\n", file);
      return;
    }
    Dijkstra(map, vertices, start, end);
  }

  fclose(fptr);
}
