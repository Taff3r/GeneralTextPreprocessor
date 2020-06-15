#ifndef STRING_UTIL_H
#define STRING_UTIL_H
#include <stdlib.h>
char** tokenize(char*, size_t*);
char* search_and_replace(char* str, char* token, char* replacement);
#endif
