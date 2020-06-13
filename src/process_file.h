#ifndef PROCESS_FILE_H
#define PROCESS_FILE_H
#include <stdio.h>
#include "hashmap.h"
enum e_macro {
    DEF,
    FUN,
    INC
};

typedef struct macro_t macro_t;
struct macro_t {
    char** argv;
    char* expansion;
    enum e_macro type;
    size_t argc;
};
size_t hash(const void* ptr);
int cmp(const void* a, const void* b);
void delete_key(void*);
void delete_val(void*);
void process(FILE*, FILE*);

void expand_macro(char* token, char* line, const macro_t* macro, FILE* output);
void write_line(hash_table*, char*, FILE*);
void add_macro(char*, hash_table*);
void init_fun_macro(macro_t*, char*);
void init_def_macro(macro_t*);

void format_expansion(char* line_args[], const macro_t*, FILE* output);
#endif
