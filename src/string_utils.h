#ifndef STRING_UTIL_H
#define STRING_UTIL_H
#include <stdlib.h>
char** tokenize(char*, char*, size_t*);
char* search_and_replace(char* str, char* token, char* replacement);
char* search_and_replace_all(char* str, char** tokens, char** replacements, size_t cnt);
int contains(char* str, char* token);
#endif
