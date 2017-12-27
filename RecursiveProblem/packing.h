#include <stdio.h>
#include <stdlib.h>

//structure(s)
typedef struct treeNode {
  char cut;
  double x;
  double y;
  double height;
  double width;
  int ID; //identifying number (height if a parent node)
  struct treeNode* left;
  struct treeNode* right;
  struct treeNode* parent;
} treeNode;
typedef struct treeStack {
  treeNode* tree;
  struct treeStack* next;
} treeStack;

//Required Functions
treeNode* Load_binary_tree_from_file(char* filename);
void Perform_packing(treeNode* root);
void Save_packing_to_file(char* filename, treeNode* root);

//Auxiliary Functions
double max(double a, double b);
treeNode* makeTreeNode(char cut, int id, double width, double height);
treeStack* makeStackNode(treeNode* tree);
void push(treeStack* toInsert, treeStack** head);
treeNode* pop(treeStack** head);
void findMaxNode(treeNode* ptr, treeNode** maxNode);
void freeTree(treeNode* ptr);
void printTree(FILE* fptr, treeNode* root);
