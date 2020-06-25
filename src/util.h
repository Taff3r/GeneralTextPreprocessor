#ifndef UTIL_H
#define UTIL_H
#include <stdlib.h>
#include "definitions.h"
void uerror(const char* msg);
void formatted_uerror(const char* msg, const char* token);
void uerror_no_exit(const char* msg);
void* xcalloc(size_t cnt, size_t size);
void* xmalloc(size_t size);
void extract_path_and_file(char*, char*, const char*);
#endif
