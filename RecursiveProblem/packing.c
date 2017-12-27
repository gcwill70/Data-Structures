#include "packing.h"

double max(double a, double b){
  return (a >= b ? a : b);
}
void freeTree(treeNode* ptr) {
  if(!ptr) {return;}
  freeTree(ptr->left);
  freeTree(ptr->right);
  free(ptr);
}
void push(treeStack* toInsert, treeStack** head) {
  treeStack* temp = *head;
  *head = toInsert;
  (*head)->next = temp;
}
treeNode* pop(treeStack** head) {
  treeStack* stackNode = *head;
  treeNode* ret = NULL;
  if(stackNode) {
    *head = stackNode->next;
    ret = stackNode->tree;
    free(stackNode);
  }
  return ret;
}
treeStack* makeStackNode(treeNode* tree) {
  treeStack* ret = malloc(sizeof(treeStack));
  ret->tree = tree;
  ret->next = NULL;
  return ret;
}
treeNode* makeTreeNode(char cut, int id, double width, double height) {
  treeNode* ret = malloc(sizeof(treeNode));
  ret->cut = cut;
  ret->x = -1;
  ret->y = -1;
  ret->height = height;
  ret->width = width;
  ret->ID = id;
  ret->left = NULL;
  ret->right = NULL;
  ret->parent = NULL;
  return ret;
}
void printTree(FILE* fptr, treeNode* ptr) {
  if(!ptr) {return;}
  printTree(fptr, ptr->left);
  if(!(ptr->cut)) {
    fprintf(fptr, "%d %le %le %le %le\n", ptr->ID, ptr->width, ptr->height, ptr->x, ptr->y);
  }
  printTree(fptr, ptr->right);
}
void findMaxNode(treeNode* ptr, treeNode** maxNode) {
  if(!ptr) {return;}
  if(ptr->ID > (*maxNode)->ID) {
    *maxNode = ptr;
  }
  findMaxNode(ptr->left, maxNode);
  findMaxNode(ptr->right, maxNode);
}
treeNode* Load_binary_tree_from_file(char* filename) {
  //open file
  FILE* fptr = fopen(filename, "r");
  if(!fptr) {
		printf("\nError: can't open file %s.\nExiting...\n", filename);
  	return NULL;
  }
  fpos_t* filepos = malloc(sizeof(fpos_t)); //stores current file position
  int rectNum;
  char cut;
  double width;
  double height;
  treeStack* temp;
  treeStack* head = NULL;
	while(!feof(fptr)) //while not at the end of the file
	{
		fgetpos(fptr, filepos); //get current place in file
		if(fscanf(fptr, "%d(%le,%le)\n", &rectNum, &width, &height) == 3) { //try to read a rectangle
      //make a treeStack with appropriate values
      temp = makeStackNode(makeTreeNode(0, rectNum, width, height));
		}
		else { //reading in a cut
      //go back to where we were in the file before trying to read in a rectangle
      fsetpos(fptr, filepos);
      //read in the cut
      fscanf(fptr, "%c\n", &cut);
      //create subtree to be pushed
      temp = makeStackNode(makeTreeNode(cut, -1, -1, -1));
      //assign right child & update parent field
      temp->tree->right = pop(&head);
      temp->tree->right->parent = temp->tree;
      //assign left child & update parent field
      temp->tree->left = pop(&head);
      temp->tree->left->parent = temp->tree;
      //update height and width of parent node
      if(temp->tree->cut == 'V') { //vertical cut
        temp->tree->height = max(temp->tree->right->height, temp->tree->left->height);
        temp->tree->width = temp->tree->right->width + temp->tree->left->width;
      }
      else { //horizontal cut
        temp->tree->width = max(temp->tree->right->width, temp->tree->left->width);
        temp->tree->height = temp->tree->right->height + temp->tree->left->height;
      }
    }
    //in either case, push the node that was just made back onto the stack
    push(temp, &head);
  }
  //close file
  fclose(fptr);
  //pop off the final node
  treeNode* root = pop(&head);
  root->x = 0;
  root->y = 0;
  free(head);
  free(filepos);
  return root;
}
void Perform_packing(treeNode* ptr) {
  if(!ptr) {return;}
  //update children
  if(ptr->cut == 'V') {
    //left side
    ptr->left->x = ptr->x;
    ptr->left->y = ptr->y;
    //right side
    ptr->right->x = ptr->x + ptr->left->width;
    ptr->right->y = ptr->y;
  } else {
    //top
    ptr->left->x = ptr->x;
    ptr->left->y = ptr->y + ptr->right->height;
    //bottom
    ptr->right->x = ptr->x;
    ptr->right->y = ptr->y;
  }
  //visit left (only if it's a parent node)
  if(ptr->left->cut) {Perform_packing(ptr->left);}
  //visit right (only if it's a parent node)
  if(ptr->right->cut) {Perform_packing(ptr->right);}
}
void Save_packing_to_file(char* filename, treeNode* root) {
  FILE* fptr = fopen(filename, "w");
  if(!fptr) {
		printf("\nError: can't open file %s.\nExiting...\n", filename);
  	return;
  }
  printTree(fptr, root);
  treeNode* maxNode = root;
  findMaxNode(root, &maxNode);
  printf("Width: %le\n", root->width);
  printf("Height: %le\n", root->height);
  printf("X-coordinate: %le\n", maxNode->x);
  printf("Y-coordinate: %le\n", maxNode->y);
  freeTree(root);
  fclose(fptr);
}
