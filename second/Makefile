all: second
second: second.o
	gcc -g -o second second.c
second.o: second.c second.h
	gcc -Wall -Werror -c second.c
clean:
	rm -f second second.o
