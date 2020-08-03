CC = gcc
CFLAGS = -g -static -O3 -Wall -Wextra -pedantic-errors -std=c99
DEBUGFLAGS = -fprofile-arcs -ftest-coverage

PROGS = gtpp

gtpp: src/process_file.o src/main.o src/hash_map.o src/util.o  src/string_utils.o src/file_inclusion.o src/funcpointers.o
	$(CC) src/hash_map.c src/util.c src/process_file.c src/string_utils.c src/file_inclusion.c src/funcpointers.c src/main.c -o gtpp
clean: 
	rm -rf src/*.o $(PROGS) vgcore*
install:
	mv gtpp /usr/local/bin/
std:
	mkdir ~/.gtpp/
	mkdir ~/.gtpp/std/
	cp macros/* ~/.gtpp/std/
