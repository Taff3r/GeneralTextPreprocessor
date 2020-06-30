#ifndef STRING_UTIL_H
#define STRING_UTIL_H
#include <stdlib.h>
#include "process_file.h"
char** tokenize(char*, char*, size_t*);
char* search_and_replace(char* str, const char* token, char* replacement);
char* search_and_replace_all(char* str, char** tokens, char** replacements, size_t cnt);
int contains(char* str, char* token);
void trim_whitespace(char*);
void trim_leading_whitespace(char*);
void expand_between(char* str, const size_t lower, const size_t upper, const hash_table* macros);
#endif
