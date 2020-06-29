#ifndef FUNC_P_H
#define FUNC_P_H
#include <stdlib.h>
int string_cmp(const void* a, const void* b);
void delete_key(void* str);
void delete_val(void* macro);
int cmp(const void* a, const void* b);
size_t hash(const void* ptr);
#endif
