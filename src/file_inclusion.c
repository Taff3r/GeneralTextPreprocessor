#include "file_inclusion.h"
#include <stdio.h>
#include "util.h"
#include "definitions.h"
#include "process_file.h"
void include_file(const char* path, hash_table* macros)
{
    printf("Trying to open %s\n", path);
    FILE* inc;
    inc = fopen(path, "r");
    char line[MAX_LINE_LENGTH];
    if (!inc) {
        char formatted[MAX_LINE_LENGTH];
        sprintf(formatted, "Cannot open path: %s\n", path);
        uerror_no_exit(formatted);
        goto CLOSE;
    }

    /* Read file line by line */
    while(1) {
        /* Assume no line longer than MAX_LINE_LENGTH chars */
        char* r = fgets(line,MAX_LINE_LENGTH - 1, inc);
        if (r == NULL)
            break;
        /* normal word write to output */
        if (line[0] != RESERVED_MACRO_CHAR)
            continue;
        add_macro(line, macros);
    }



CLOSE:
    if (inc != NULL)
        fclose(inc);
}
