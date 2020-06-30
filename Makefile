CC = gcc
CFLAGS = -g -static -O3 -Wall -pedantic-errors
DEBUGFLAGS = -fprofile-arcs -ftest-coverage

PROGS = gtpp

gtpp: src/main.o src/hash_map.o src/util.o src/process_file.o src/string_utils.o src/file_inclusion.o src/funcpointers.o
	$(CC) src/main.c src/hash_map.c src/util.c src/process_file.c src/string_utils.c src/file_inclusion.c src/funcpointers.c -o gtpp

test: test/test.o hash_map.o
	$(CC) $(CFLAGS) test/test.c src/hash_map.c -o src/test_hash_map

debug: src/main.o src/hash_map.o src/util.o src/process_file.o src/string_utils.o src/file_inclusion.o src/funcpointers.h
	$(CC) $(CFLAGS) $(DEBUGFLAGS) src/main.c src/hash_map.c src/util.c src/process_file.c src/string_utils.c src/file_inclusion.c src/funcpointers.c -o gtpp
	
all: 
	$(PROGS)	
test:
	gtpp test
clean: 
	rm -rf src/*.o $(PROGS) vgcore*
