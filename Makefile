all: first
first: first.o
	gcc -o first first.c
first.o: first.c first.h
	gcc -Wall -Werror -c first.c
clean:
	rm -f first first.o
