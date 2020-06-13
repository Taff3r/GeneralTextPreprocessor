#include "util.h"
#include <stdio.h>
#include <stdlib.h>
void uerror(const char* msg)
{
    fprintf(stderr, msg);
    fprintf(stderr, "Exiting \n");
    exit(1);
}

void uerror_no_exit(const char* msg)
{
    fprintf(stderr, msg);
}

void* xmalloc(size_t size)
{
    void* ptr = malloc(size);
    if(ptr == NULL)
        exit(1);
    return ptr;
}
void* xcalloc(size_t cnt, size_t size)
{
    void* ptr = calloc(cnt, size);
    if(ptr == NULL)
        exit(1);
    return ptr;
}
