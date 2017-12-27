#include "sorting.h"

int main(int argc, char **argv) {
	if (argc != 5) {
    		printf("Sent %d arguments but expected 5.", argc);
		return EXIT_FAILURE;
	}
	
	//rename command line arguments for readability
	char* sortType = argv[1];
	char* inFile_b = argv[2];
	char* outFile_t = argv[3];
	char* outFile_b = argv[4];
	
	//initialize necessary tracking variables
	int* Size = malloc(sizeof(int));
	double* N_Comp = malloc(sizeof(double));
	double* N_Move = malloc(sizeof(double));

	//1. Get sequence and file size
	clock_t startLoad = clock();
	long* Array = Load_From_File(inFile_b, Size);
	clock_t stopLoad = clock();
	if (Array == NULL) { //invalid file was given
		free(Size);
		free(N_Comp);
		free(N_Move);
		free(Array);
		return EXIT_FAILURE;
	}

	//2. Call appropriate sort
	clock_t startSort = clock();
	if (*sortType == 'i') {
		Shell_Insertion_Sort(Array, *Size, N_Comp, N_Move);
	} else if (*sortType == 's') {
		Shell_Selection_Sort(Array, *Size, N_Comp, N_Move);
	} else {
		printf("\nError: an invalid sorting algorithm was specified. The second argument should be an 'i' (for insertion sort) or an 's' (for selection sort).\nExiting...\n");
		free(Size);
		free(N_Comp);
		free(N_Move);
		free(Array);
		return EXIT_FAILURE;
	}
	clock_t stopSort = clock();

	//3. Screen dump
	printf("Number of long integers read: %d\n", *Size);

	clock_t startSave = clock();
	int numStored = Save_To_File(outFile_b, Array, *Size);
	clock_t stopSave = clock();
	printf("Number of long integers stored: %d\n", numStored);

	clock_t startSeq = clock();
	int seqLength = Print_Seq(outFile_t, *Size);
	clock_t stopSeq = clock();
	printf("Length of sequence: %d\n", seqLength);

	printf("Number of comparisons: %le\n", *N_Comp);
	printf("Number of moves: %le\n", *N_Move);

	double ioCycles = (double)(stopLoad-startLoad) + (double)(stopSave-startSave) + (double)(stopSeq-startSeq);
	double ioTime = ioCycles/CLOCKS_PER_SEC;
	printf("I/O time: %le\n", ioTime);
	
	double sortCycles = (double)(stopSort-startSort);
	double sortTime = sortCycles/CLOCKS_PER_SEC;
	printf("Sorting time: %le\n", sortTime);

	//4. Cleanup
	free(Size);
	free(N_Comp);
	free(N_Move);
	free(Array);

	return 0;
}
