#include "file_inclusion.h"
#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "definitions.h"
#include "process_file.h"
#include "globals.h"
#include "string.h"
#include <unistd.h> /* TODO: Not portable? Fuck Windows?*/
/*
 *
 * Opens the file pointed to path_to_file. 
 * NOTE Must be complete path, and NOT relative.
 * E.g. /home/simont/macros/m.gtpp
 */
void include_file(const char* path_to_file, hash_table* macros)
{
    /* Old state */
    long long old_ln;
    char old_file_name[MAX_WORD_LENGTH - 1];
    char old_path[MAX_LINE_LENGTH - 1];

    /* Temp path */
    char new_file_name[MAX_WORD_LENGTH];
    char new_path[MAX_LINE_LENGTH];
    /* Reading vars */
    FILE* inc;
    char line[MAX_LINE_LENGTH - 1];
    
    /* Save old state */
    old_ln = line_number;
    strcpy(old_file_name, current_file);
    strcpy(old_path, current_path);
    
    /* Extract file and path to open */
    extract_path_and_file(new_path, new_file_name, path_to_file);

    /* Change path and globals */
    chdir(new_path);
    strcpy(current_path, new_path);
    char tmp[MAX_LINE_LENGTH - 1];
    getcwd(tmp, MAX_LINE_LENGTH - 1);

    /* Try to open file and read it */
    inc = fopen(new_file_name, "r");
    if (!inc)
        formatted_uerror("Cannot open path to file %s\n", new_file_name);

    strcpy(current_file, new_file_name);
    strcpy(current_file, path_to_file);
    line_number = 0;
    /* Read file line by line */
    while(1) {
        /* Assume no line longer than MAX_LINE_LENGTH chars */
        char* r = fgets(line,MAX_LINE_LENGTH - 1, inc);
        if (r == NULL)
            break;
        ++line_number;
        /* normal word write to output */
        if (line[0] != RESERVED_MACRO_CHAR)
            continue;
        add_macro(line, macros);
    }
    chdir(old_path);
    strcpy(current_path, old_path);
    strcpy(current_file, old_file_name);
    line_number = old_ln;
    fclose(inc);
}
