GCC = gcc -Werror -Wall -Wshadow -lm

ref_master:
	git fetch origin
	git reset --hard origin/master

ref_opt:
	git fetch origin
	git reset --hard origin/opt

all:
	$(GCC) -O3 graph.c main.c -o proj4

main.o: graph.o
	$(GCC) -c main.c

graph.o:
	$(GCC) -c graph.c

debug:
	$(GCC) -g graph.c main.c -o debug.o
	lldb debug.o -- io_files/PA_map.txt io_files/PA_query.txt

test: all
	./proj4 io_files/map5x5.txt io_files/query5x5.txt
	./proj4 io_files/usa.txt io_files/usa10.txt
	./shortestpath io_files/usa.txt io_files/usa10.txt

memory: all
	valgrind -v --tool=memcheck --show-reachable=yes --leak-check=full ./proj4 io_files/usa.txt io_files/usa1.txt

clean:
	rm -f *.o proj4 proj4.prof *.out io_files/*.flr
	rm -r -f *.dSYM
