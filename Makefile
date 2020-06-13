CC = gcc
CFLAGS = -g -O3 -Wall -pedantic-errors

PROGS = mdpp

mdpp: src/main.o src/hash_map.o src/util.o src/process_file.o src/string_utils.o
	$(CC) src/main.c src/hash_map.c src/util.c src/process_file.c src/string_utils.c -o mdpp

test: test/test.o hash_map.o
	$(CC) $(CFLAGS) test/test.c src/hash_map.c -o src/test_hash_map

all: 
	$(PROGS)	
test:
	mdpp test
clean: 
	rm -f *.o $(PROGS)
