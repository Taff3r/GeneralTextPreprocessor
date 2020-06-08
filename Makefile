CC = gcc
CFLAGS = -g -O3 -Wall -pedantic-errors


PROGS = mdpp test_hash_map

mdpp: main.o hash_map.o util.o
	$(CC) main.c hash_map.c util.c -o mdpp

test: test/test.o hash_map.o
	$(CC) $(CFLAGS) test/test.c hash_map.c -o test_hash_map

all: 
	$(PROGS)
clean: 
	rm -f *.o $(PROGS)
