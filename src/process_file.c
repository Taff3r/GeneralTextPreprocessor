#include "process_file.h"
#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include <string.h>
#include "hashmap.h"
#include "definitions.h"
#include "string_utils.h"
#include "file_inclusion.h"
size_t hash(const void* ptr)
{
    size_t hash = 5381;
    int c;
    const char* str = (char*) ptr;
    while ((c = *str++) != '\0')
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

int cmp(const void* a, const void* b)
{
    const char* m_a = (const char*) a;
    const char* m_b = (const char*) b;
    return strcmp(m_a, m_b) == 0;
}

void delete_val(void* macro)
{
    macro_t* m = macro;
    free(m->expansion);
    if(m->argv != NULL) {
        for (size_t i = 0; i < m->argc; ++i)
            free(m->argv[i]); 
        free(m->argv);
    }
}

void delete_key(void* str)
{
    return;
}
/*
 * Read all macro defintions and put them in the map.
 */
void process(FILE* input, FILE* output)
{

    hash_table* map = new_hash_table(cmp, hash, delete_key, delete_val);
    char line[MAX_LINE_LENGTH];
    while(1) {
        /* Assume no line longer than MAX_LINE_LENGTH chars */
        char* r = fgets(line,MAX_LINE_LENGTH - 1, input);
        if (r == NULL)
            break;
        else if (strcmp(line, "\n") == 0) /* Empty line */
            fputs("\n", output);
        /* normal word write to output */
        else if (line[0] != RESERVED_MACRO_CHAR)
            write_line(map, line, output);
        else /* Add since it is a macro line. */
            add_macro(line, map);

    }
    delete_hash_table(map);
} 

void write_line(hash_table* macros, char* line, FILE* output)
{
    /* Check if line contains any of the macros in the table,
     * if so expand them if possible 
     */
    macro_t* m;
    size_t i;
    size_t sz; 

    char** macro_keys = (char**) keys(macros, &sz);
    for (i = 0; i < sz; ++i) {
        while (contains(line, (char*) macro_keys[i])) {
            m = lookup(macros, macro_keys[i]);
            if (m != NULL) {
                expand_macro(macro_keys[i], line, m, output);
            }
        } 
    }
    free(macro_keys);
    fputs(line, output);
}

void expand_macro(char* key, char* line, const macro_t* macro, FILE* output)
{
    switch(macro->type) {
        case DEF:
            {
                char* tmp = search_and_replace(line, key, macro->expansion);
                strcpy(line, tmp);
                free(tmp);
            }
            break;
        case FUN:
            {
             char* expanded = expand_function(key, line, macro);
             strcpy(line, expanded);  
             free(expanded);
            }
            break;
        default:
            break;
    }
}
/*
 * TODO: Add table here to allow recursion.
 * TODO: Make sure that a key is not used as an argument without it being
 *       called properly.
 */
char* expand_function(const char* key, char* line, const macro_t* macro) 
{
    char* line_args[macro->argc];
    /* Find instance of key in line */
    char* key_il;
    char line_cpy[MAX_LINE_LENGTH]; /* TODO correct size */
    strcpy(line_cpy, line);
    key_il = strstr(line_cpy, key);
    /* Find the pos of parentheses*/
    size_t pos_of_parentheses;
    /* Read the args inside the parentheses */
    pos_of_parentheses = key_il - line_cpy + strlen(key);
    size_t i = 0;
    char* line_p = line_cpy + pos_of_parentheses + 1;
    line_args[i++] = strtok(line_p, ",)");
    for (; i < macro->argc; ++i) {
        line_p += strlen(line_args[i - 1]) + 1;
        line_args[i] = strtok(line_p, ",)");
    }

    /* Replaced the arguments provided with to the expansion */
    char* expansion = search_and_replace_all(macro->expansion, macro->argv, \
            line_args, macro->argc);
    /* TODO CORRECT SIZE */
    char* concat;
    concat = xcalloc(strlen(key) + macro->argc * MAX_WORD_LENGTH + 1, sizeof(char));
    char key_cpy[strlen(key) + 1];
    strcpy(key_cpy, key);
    strcat(concat, key_cpy);
    strcat(concat, "(");
    for (i = 0; i < macro->argc; ++i) {
        strcat(concat, line_args[i]);
        if( i + 1 != macro->argc)
            strcat(concat, ",");
    }
    strcat(concat, ")");

    /* Replace the key and the arg list in the string with the expansion */
    char* final = search_and_replace(line, concat, expansion);
    free(concat);
    free(expansion);
    return final;
}

/*
 * TODO: Include table here for recursion.
 */
void format_expansion(char* line_args[], const macro_t* macro, FILE* output) {
    /* Read each token in expansion and replace with corresponding line_arg */
    char* to_put;
    to_put = search_and_replace_all(macro->expansion, macro->argv, line_args, macro->argc);
    fputs(to_put, output);
    free(to_put);
}

void add_macro(char* line, hash_table* t)
{
    char* macro_type;
    char* key;
    macro_t* m;

    m = xmalloc(sizeof(macro_t));
    key = xcalloc(MAX_WORD_LENGTH + 1, sizeof(char));
    macro_type = strtok(line, " "); 
    /* TODO Add possibility of having any white space as seperation to expan */
    if (strcmp(macro_type, MACRO_DEF) == 0) {
        /* Add as simple replacement macro */
        strcpy(key, strtok(NULL, " "));
        if (lookup(t, key))
            formatted_uerror("Multiple definitions of key: %s\n", key);

        char* expansion = strtok(NULL, NEWLINE_CHAR); /* A lot of memory */
        m->expansion = xcalloc(strlen(expansion) + 1, sizeof(char));
        strcpy(m->expansion, expansion);
        init_def_macro(m);
        insert(t, key, m);
    } else if (strcmp(macro_type, FUNC_DEF) == 0) {
        /* Add as function macro */
        strcpy(key, strtok(NULL, "("));
        char* arg_list = strtok(NULL, ")");
        trim_whitespace(arg_list);
        char* expansion = strtok(NULL, NEWLINE_CHAR);
        expansion += 1; /* TODO SPAGHETT */
        init_fun_macro(m, arg_list, expansion);
        insert(t, key, m);
    } else if (strcmp(macro_type, INC_DEF) == 0) {
        char* path;
        path = xcalloc(MAX_LINE_LENGTH, sizeof(char));

        strcpy(path, strtok(NULL, NEWLINE_CHAR));
        include_file(path, t);
        free(path);
        /* Turns out these aren't needed here. TODO refactor */
        free(m);
        free(key);
    }
}

void init_fun_macro(macro_t* m, char* arg_list, char* expansion)
{
    char key_cpy[strlen(arg_list) + 1];
    size_t argc = 0;
    char** argv_tmp = xcalloc(MAX_ARGC, sizeof(char*));
    m->type = FUN;
    strcpy(key_cpy, arg_list);
    m->expansion = xcalloc(strlen(expansion) + 1, sizeof(char));
    strcpy(m->expansion, expansion);
    size_t i = 0;
    size_t len = strlen(key_cpy);
    /* find first occurence of RESERVED_MACRO_CHAR */;
    /* TODO: Error handling, missing arg list, or parentheses */
    while (i < len){
        /* Find position of first RESERVED_MACRO_CHAR */
        while(key_cpy[i] != RESERVED_MACRO_CHAR)
            ++i;
        /* Find position of next ',' or ')' */
        size_t n = i;
        while (key_cpy[n] != ',' &&  key_cpy[n] != '\0')
            n++;
        size_t k;
        argv_tmp[argc] = xcalloc(n - i + 1, sizeof(char));
        for (k = 0; k < n - i; ++k)
            argv_tmp[argc][k] = key_cpy[i + k];
        argc++;
        i += n - i + 1; 
    }
    m->argc = argc;
    m->argv = xcalloc(argc, sizeof(char*));
    for (i = 0; i < argc; ++i){
        m->argv[i] = xcalloc(strlen(argv_tmp[i]) + 1, sizeof(char));
        strcpy(m->argv[i], argv_tmp[i]);
    }
    for (i = 0; i < MAX_ARGC; ++i)
        free(argv_tmp[i]);
    free(argv_tmp);
}
void init_def_macro(macro_t* m)
{
    m->argv = NULL;
    m->type = DEF;
    m->argc = 0;
}

