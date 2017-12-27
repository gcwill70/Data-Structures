#include "sorting.h"

//additional function declarations
static int get_seq(int max, int** k_seq) {
	int minValue = 2; //value of branch on the left (form is (2^p)*(3^0))
	int upperLevel = 2; //first level that k CANNOT be on
	int num_k = 0; //approximate number of k values

	//1. Find the first invalid tree level and its leftmost value
	while(minValue <= max) {
		minValue *= 2;
		upperLevel++;
	}

	//2. Malloc enough space for the array
	int l; //current level number
	for(l = upperLevel-1; l > 0; l--) { //for each valid level
		num_k += l; //the number of k values in each level is equal to the level number
	}
	*k_seq = malloc(sizeof(int)*num_k);

	//3. Traverse through upperLevel-1 to 1
	int n; //index for current value in a level
	int k; //current k value
	int left = 1; //current leftmost value in the k sequence tree
	int i = 0; //index for current k value in the sequence
	for(l = 1; l < upperLevel; l++) { //for each valid level
		k = left; //initialized the k value to the leftmost value in the level
		for(n = 0; n < l; n++) { //for each value in the level
			if(k < max) { //if current k value is less than the max
				(*k_seq)[i++] = k;
			}
			else {break;}
			k = (k*3)/2; //get the k value to the right in the level "l"
		}
		left *= 2; //go up one level
	}

	return i;
}
static Node* popNode(Node** head) {
  Node* ret = *head;
  if(ret) {
    *head = ret->next;
  }
  return ret;
}
static Node* pushNode(Node* head, Node* toInsert) {
  Node* temp = head;
  head = toInsert;
  head->next = temp;
  return head;
}
static Node* makeNode(long val) {
  Node* ret = malloc(sizeof(Node));
  ret->next = NULL;
  ret->value = val;
  return ret;
}
static void freeList(List* list) {
	List* temp;
	while(list) {
		temp = list;
		list = list->next;
		free(temp);
	}
}
static List* genArrays(Node* head, int k) {
  List* ret = malloc(sizeof(List));
  ret->node = NULL;
  ret->next = NULL;
  List* curList = ret;

  //create list of List*
  int i;
  for(i = 1; i < k; i++) {
    curList->next = malloc(sizeof(List));
    curList->next->node = NULL;
    curList->next->next = NULL;
    curList = curList->next;
  }

  //populate lists
  curList = ret;
  while(head) { //iterate through original array
    if(!curList) {curList = ret;} //start at the beginning if we reach the end
    curList->node = pushNode(curList->node, popNode(&head)); //remove node from original list and add to List*
    curList = curList->next; //go to next list
  }

  return ret;
}
static Node* assemble(List* list) {
  if(!list) {return NULL;}
  Node* ret = popNode(&(list->node));
  List* curList = list->next;
  Node* toInsert = ret; //pointer to location to insert next node at
  int noNodes = 0;
  do {
    noNodes = 1; //assume there are no nodes left for this pass
    while(curList) { //iterate through each list
      if(curList->node) { //if there is a node list
        toInsert->next = popNode(&(curList->node));
        toInsert = toInsert->next; //go to next node in assembled list
        noNodes = 0;
      }
      curList = curList->next; //go to next node in current list
    }
    curList = list;
  }while(!noNodes);
  freeList(list);

  return ret;
}
static Node* insertionSort(Node* head) {
	//check if there is one or zero elements
	if(!head || !(head->next)) {return head;}
	Node** cur;
	Node** check;
	Node* temp1;
	Node* temp2;
	cur = &head;
	while((*cur)->next) { //last node in sorted part
		//edge case: insert at head
		if(head->value > (*cur)->next->value) {
			temp1 = (*cur)->next;
			temp2 = head;
			(*cur)->next = temp1->next;
			temp1->next = temp2;
			head = temp1;
		}
		else { //didn't insert at head
			check = &head;
			while((*check)->next->value <= (*cur)->next->value &&
						(*check)->next != (*cur)->next) {
					check = &((*check)->next);
			}
			//swap
			//check if insertion is necessary
			if((*check)->next != (*cur)->next) {
				temp1 = (*cur)->next;
				temp2 = (*check)->next;
				(*cur)->next = temp1->next;
				(*check)->next = temp1;
				temp1->next = temp2;
			}
			else { //did not swap any nodes
				cur = &((*cur)->next);
			}
		}
	}

	return head;
}
static void freeLL(Node* node) {
	Node* temp;
	while(node) {
		temp = node;
		node = node->next;
		free(temp);
	}
}
//required functions
Node* Load_From_File(char* Filename) {
  FILE* fp = fopen(Filename, "r");
  if(!fp) {
		printf("\nError: can't open file %s.\nExiting...\n", Filename);
  	return NULL;
  }
  Node* head = NULL;
  Node* temp = NULL;
  long val = 0;
  while(fread(&val, sizeof(long), 1, fp) == 1) {
    temp = head;
    head = makeNode(val);
    head->next = temp;
  }
  fclose(fp);

  return head;
}
int Save_To_File(char *Filename, Node* list) {
	int numStored = 0;
	if(!list) {return numStored;}
	//1. Open the file
	FILE* outFile_b = fopen(Filename, "w");
	if(!outFile_b) {
		printf("\nError: can't open file %s.\nExiting...\n", Filename);
		return numStored;
	}

  Node* cur = list;
  while(cur) {
		fwrite(&(cur->value), sizeof(long), 1, outFile_b);
    cur = cur->next;
		numStored++;
  }
	fclose(outFile_b);
	freeLL(list);

	return numStored;
}
Node* Shell_Sort(Node* list) {
	//1. Get size of list
	Node* cur = list;
	int size = 1;
	while(cur->next) {
		size++;
		cur = cur->next;
	}

	//2. Generate sequence
	int* k_seq;
	int num_k = get_seq(size, &k_seq);

  //3. Sort
  List* subArrays; //list of sub-arrays
  List* curList; //current sub-array
  int k;
  for(k = num_k-1; k >= 0; k--) { //for each k value
    //generate sub-arrays
    subArrays = genArrays(list, k_seq[k]);
    curList = subArrays;
    while(curList) { //for each sub-array
      //sort the sub-array
      curList->node = insertionSort(curList->node);
      curList = curList->next;
    }
    //reassemble array
    list = assemble(subArrays);
  }
	free(k_seq);

  return list;
}
