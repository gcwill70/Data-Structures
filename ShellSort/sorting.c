#include "sorting.h"

//additional function declarations
static int get_seq(int max, int** k_seq);

long *Load_From_File(char *Filename, int *Size) {
	//1. Open the file
	FILE* inFile_b = fopen(Filename, "r");
	if (inFile_b == NULL) {
		printf("\nError: can't open file %s.\nExiting...\n", Filename);
		return NULL;
	}

	//2. Get the size of the file
	fseek(inFile_b, 0L, SEEK_END);
	*Size = (int)( ftell(inFile_b)/sizeof(long) );
	rewind(inFile_b);
	long* ret = malloc(sizeof(long)*(*Size));
	
	//3. Scan in the array to ret
	fread(ret, sizeof(long), *Size, inFile_b);
	fclose(inFile_b);
	return ret;
}

int Save_To_File(char *Filename, long *Array, int Size) {
	int numStored = 0;
	//1. Open the file
	FILE* outFile_b = fopen(Filename, "w");
	if (outFile_b == NULL) {
		printf("\nError: can't open file %s.\nExiting...\n", Filename);
		return numStored;
	}

	//2. Write to the file
	int i;
	for(i = 0; i < Size; i++) {
		fwrite(&(Array[i]), sizeof(long), 1, outFile_b);
		numStored++;
	}
	fclose(outFile_b);
	return numStored;
}

void Shell_Insertion_Sort(long *Array, int Size, double *N_Comp, double *N_Move) {
	*N_Comp = 0;
	*N_Move = 0;
	if(Size < 2) {return;}
	
	int* k_seq;
	int num_k = get_seq(Size, &k_seq);

	int i, j, k, temp;
	for(k = num_k-1; k >= 0; k--) { //for each k value
		for (j = k_seq[k]; j <= Size-1; j++) {
			temp = Array[j];
			(*N_Move)++;
			i = j;
			while (i>=k_seq[k] && Array[i-k_seq[k]]>temp) {
				(*N_Comp)++;
				Array[i] = Array[i-k_seq[k]];
				(*N_Move)++;
				i = i-k_seq[k];
			}
			(*N_Comp)++;
			Array[i] = temp;
			(*N_Move)++;
		}
	}
	free(k_seq);
}

void Shell_Selection_Sort(long *Array, int Size, double *N_Comp, double *N_Move) {
	*N_Comp = 0;
	*N_Move = 0;
	if(Size < 2) {return;}
	
	int* k_seq;
	int num_k = get_seq(Size, &k_seq);
	
	int i, j, k, min_index, temp;
	for(k = num_k-1; k >= 0; k--) { //for each k value
		for(i = k_seq[k]; i < Size; i++) { //second element in each sub-array
			min_index = i - k_seq[k]; //first element in sub-array
			j = i;
			while(j < Size) { //for each element in the sub-array
				(*N_Comp)++;
				if(Array[j] < Array[min_index]) {
					min_index = j;
				}
				j += k_seq[k]; //step size is k
			}
			//swap min_index with the left-most position in sub-array
			temp = Array[i-k_seq[k]];
			Array[i-k_seq[k]] = Array[min_index];
			Array[min_index] = temp;
			(*N_Move) += 3;
		}
	}
	free(k_seq);
}

int Print_Seq(char *Filename, int Size) {
	//1. Open the file
	FILE* outFile_t = fopen(Filename, "w");
	if (outFile_t == NULL) {
		printf("\nError: can't open file %s.\nExiting...\n", Filename);
		return 0;
	}

	//2. Get k sequence
	int* k_seq;
	int num_k = get_seq(Size, &k_seq);
	int i;
	
	//3. Print to the text file
	for(i = 0; i < num_k; i++) { //for each k value
		fprintf(outFile_t, "%d\n", k_seq[i]);
		
	}
	
	//4. Cleanup
	free(k_seq);
	fclose(outFile_t);
	return num_k;
}

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
