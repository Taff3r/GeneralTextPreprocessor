#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include "definitions.h"
#include "globals.h"
void uerror(const char* msg)
{
    fprintf(stderr, KRED);
    fprintf(stderr, msg);
    fprintf(stderr, "Exiting \n");
    fprintf(stderr, RST);
    exit(1);
}

void uerror_no_exit(const char* msg)
{

    fprintf(stderr, KYLW);
    fprintf(stderr, msg);
    fprintf(stderr, RST);
}

void formatted_uerror(const char* msg, const char* token)
{
    char formatted[MAX_LINE_LENGTH];
    char info[MAX_LINE_LENGTH];
    sprintf(info, "%s:%lld ", input_file_name, line_number);
    fprintf(stderr, KYLW UNDL);
    fprintf(stderr, info);
    fprintf(stderr, RST);
    sprintf(formatted, msg, token);
    uerror(formatted);
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
