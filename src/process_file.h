#ifndef PROCESS_FILE_H
#define PROCESS_FILE_H
#include <stdio.h>
#include "hashmap.h"
enum e_macro {
    DEF,
    FUN,
    INC,
    FLE
};

typedef struct macro_t macro_t;
struct macro_t {
    void* macro;
    enum e_macro type;
};

typedef struct func_m func_m;
struct func_m {
    char** argv;
    char* expansion;
    size_t argc;
};

typedef struct def_m def_m;
struct def_m {
    char* expansion;
};

typedef struct file_m file_m;
struct file_m {
    FILE* file;
};

size_t hash(const void* ptr);
int cmp(const void* a, const void* b);
void delete_key(void*);
void delete_val(void*);
void process(FILE*, FILE*);

void expand_macro(char*, char*, const macro_t*, const hash_table*);
void find_and_replace_keys(char*, const hash_table*);
void write_line(hash_table*, char*, FILE*);
char* expand_function(const char* key, char* line, const macro_t* macro);
void add_macro(char*, hash_table*);

void init_fun_macro(macro_t*, char*, char*);
void init_def_macro(macro_t*);
void init_file_macro(macro_t*, char*);
void m_keys(const hash_table*);
#endif
