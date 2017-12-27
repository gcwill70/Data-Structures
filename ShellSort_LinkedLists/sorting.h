#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Node {
  long value;
  struct Node* next;
} Node;

typedef struct List {
  Node* node;
  struct List* next;
} List;

Node *Load_From_File(char *Filename);
int Save_To_File(char *Filename, Node* list);
Node* Shell_Sort(Node* list);
