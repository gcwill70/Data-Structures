GCC = gcc -Werror -Wall -Wshadow

all:
	$(GCC) -O3 sorting.c main.c -o proj2

main.o: sorting.o
	$(GCC) -c main.c

sorting.o:
	$(GCC) -c sorting.c

debug:
	$(GCC) -g sorting.c main.c -o debug.o
	ddd debug.o --args ./debug.o io_files/15.b io_files/15s.b

test: all
	./proj2 io_files/15.b io_files/15s.b
	diff io_files/15s.b pa1_samples/15s.b
	./proj2 io_files/1000.b io_files/1000s.b
	diff io_files/1000s.b pa1_samples/1000s.b
	./proj2 io_files/10000.b io_files/10000s.b
	./proj2 io_files/100000.b io_files/100000s.b
	./proj2 io_files/1000000.b io_files/1000000s.b

memory: all
	valgrind -v --tool=memcheck --show-reachable=yes --leak-check=full ./proj2 io_files/1000000.b io_files/1000000s.b

clean:
	/bin/rm -f *.o proj2 proj2.prof *.out io_files/*s.b io_files/*s.b
	rm -r -f debug.o.dSYM
