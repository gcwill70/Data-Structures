GCC = gcc -Werror -Wall -Wshadow

refresh:
	git fetch origin
	git reset --hard origin/master

all:
	$(GCC) -O3 packing.c main.c -o proj3

main.o: packing.o
	$(GCC) -c main.c

sorting.o:
	$(GCC) -c packing.c

debug:
	$(GCC) -g packing.c main.c -o debug.o
	ddd debug.o --args ./debug.o io_files/r6_po.txt io_files/r6.flr

test: all
	./proj3 io_files/r0_po.txt io_files/r0.flr
	diff io_files/r0.flr pa3_samples/r0.flr
	./proj3 io_files/r6_po.txt io_files/r6.flr
	diff io_files/r6.flr pa3_samples/r6.flr

memory: all
	valgrind -v --tool=memcheck --show-reachable=yes --leak-check=full ./proj3 io_files/r6_po.txt io_files/r6.flr

clean:
	/bin/rm -f *.o proj3 proj3.prof *.out io_files/*.flr
