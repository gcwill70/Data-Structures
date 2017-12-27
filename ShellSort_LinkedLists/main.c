#include "sorting.h"

int main(int argc, char **argv) {
	//1. Get sequence and file size
	clock_t startLoad = clock();
  Node* head = Load_From_File(argv[1]);
	clock_t stopLoad = clock();
	if (head == NULL) { //invalid file was given
		return 0;
	}

	//2. Sort
	clock_t startSort = clock();
	head = Shell_Sort(head);
	clock_t stopSort = clock();

	//3. Screen dump
	clock_t startSave = clock();
	Save_To_File(argv[2], head);
	clock_t stopSave = clock();

	double ioCycles = (double)(stopLoad-startLoad) + (double)(stopSave-startSave);
	double ioTime = ioCycles/CLOCKS_PER_SEC;
	printf("I/O time: %le\n", ioTime);

	double sortCycles = (double)(stopSort-startSort);
	double sortTime = sortCycles/CLOCKS_PER_SEC;
	printf("Sorting time: %le\n", sortTime);

	return 0;
}
