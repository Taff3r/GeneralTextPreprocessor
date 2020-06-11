CC = gcc
CFLAGS = -g -O3 -Wall -pedantic-errors

PROGS = mdpp test_hash_map

mdpp: main.o hash_map.o util.o process_file.o
	$(CC) main.c hash_map.c util.c process_file.c -o mdpp

test: test/test.o hash_map.o
	$(CC) $(CFLAGS) test/test.c hash_map.c -o test_hash_map

all: 
	mdpp	
test:
	mdpp test
clean: 
	rm -f *.o $(PROGS)
