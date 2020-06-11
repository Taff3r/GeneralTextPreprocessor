#ifndef PROCESS_FILE_H
#define PROCESS_FILE_H
#include <stdio.h>
#include "hashmap.h"
typedef struct macro_t macro_t;
struct macro_t {
    char* expansion;
};
size_t hash(const void* ptr);
int cmp(const void* a, const void* b);
void delete_key(void*);
void delete_val(void*);
void process(FILE*, FILE*);
void write_line(hash_table*, char*, FILE*);
void add_macro(char*, hash_table*);

#endif
