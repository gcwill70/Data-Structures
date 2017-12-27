#include "graph.h"

int main(int argc, char** argv) {
	if(argc != 3) {
		printf("Error: wrong number of command line arguments.\n");
		return 1;
	}
	int vertices;
	node** map = loadMap(argv[1], &vertices);
	if(!map) {return 1;}
	printPaths(map, vertices, argv[2]);
	freeMap(map, vertices);
	return 0;
}
