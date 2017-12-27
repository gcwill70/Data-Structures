GCC = gcc -Werror -Wall -Wshadow

all:
	$(GCC) -O3 sorting.c main.c -o proj1

main.o: sorting.o
	$(GCC) -c main.c

sorting.o:
	$(GCC) -c sorting.c

debug:
	$(GCC) -g -pg sorting.c main.c -o debug.o
	ddd debug.o --args ./debug.o s io_files/15.b io_files/15_seq.t io_files/15s.b 

test: all
	#Test Case 1: array size = 15
	#Part 1: Shell Insertion Sort
	./proj1 i io_files/15.b io_files/15_seq.t io_files/15s.b
	diff io_files/15s.b pa1_samples/15s.b
	diff io_files/15_seq.t pa1_samples/15_seq.t
	#Part 2: Shell Selection Sort
	./proj1 s io_files/15.b io_files/15_seq.t io_files/15s.b
	diff io_files/15s.b pa1_samples/15s.b
	diff io_files/15_seq.t pa1_samples/15_seq.t
	#Test Case 2: array size = 1,000
	#Part 1: Shell Insertion Sort
	./proj1 i io_files/1000.b io_files/1000_seq.t io_files/1000s.b
	diff io_files/1000s.b pa1_samples/1000s.b
	diff io_files/1000_seq.t pa1_samples/1000_seq.t
	#Part 2: Shell Selection Sort
	./proj1 s io_files/1000.b io_files/1000_seq.t io_files/1000s.b
	diff io_files/1000s.b pa1_samples/1000s.b
	diff io_files/1000_seq.t pa1_samples/1000_seq.t
	#Test Case 3: array size = 10,000
	#Part 1: Shell Insertion Sort
	./proj1 i io_files/10000.b io_files/10000_seq.t io_files/10000s.b
	#Part 2: Shell Selection Sort
	./proj1 s io_files/10000.b io_files/10000_seq.t io_files/10000s.b
	#Test Case 4: array size = 100,000
	#Part 1: Shell Insertion Sort
	./proj1 i io_files/100000.b io_files/100000_seq.t io_files/100000s.b
	#Part 2: Shell Selection Sort
	./proj1 s io_files/100000.b io_files/100000_seq.t io_files/100000s.b
	#Test Case 5: array size = 1,000,000
	#Part 1: Shell Insertion Sort
	./proj1 i io_files/1000000.b io_files/1000000_seq.t io_files/1000000s.b
	#Part 2: Shell Selection Sort
	./proj1 s io_files/1000000.b io_files/1000000_seq.t io_files/1000000s.b

memory: all
	valgrind -v --tool=memcheck --show-reachable=yes ./proj1 s io_files/100000.b io_files/100000_seq.t io_files/100000s.b 

clean:
	/bin/rm -f *.o proj1 proj1.prof *.out io_files/*s.b io_files/*_seq.t
	/bin/rm -f *.gcda *.gcno gmon.out *.gcov
