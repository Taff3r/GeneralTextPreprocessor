#include "process_file.h"
#include <stdlib.h>
#include "util.h"
#include <string.h>
#include "hashmap.h"
#include "definitions.h"
#include "string_utils.h"
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
    char** tokens = tokenize(line, &sz);
    size_t put_nl = 0;
    for (i = 0; i < sz; ++i) {
        if (tokens[i][strlen(tokens[i]) - 1]== '\n'){
            put_nl = 1;
            strtok(tokens[i], "\n");
        }
        m = lookup(macros, tokens[i]);
        if (m == NULL)
            fputs(tokens[i], output);
        else 
            expand_macro(tokens[i], line, m, output);

        /* This feels hacky */
        if (put_nl) {
            fputs("\n", output);
            put_nl = 0;
        } else
            fputs(" ", output); 
        free(tokens[i]);
    }
    free(tokens);
}

void expand_macro(char* token, char* line, const macro_t* macro, FILE* output)
{
    switch (macro->type)  {
        case DEF:
            fputs(macro->expansion, output);
            break;
        case FUN:
            {
                /* TODO Missing ')' error */
                /* Read all args */
                size_t i = 1;
                char* line_args[macro->argc];
                line_args[0] = strtok(line, ",)"); /* Atleast one argument? */
                while(i < macro->argc){
                    line_args[i++] = strtok(NULL, ",)");
                }
                format_expansion(line_args, macro, output);
            }
            break;
        case INC:
            break;
        default:
            break;
    }
}

void format_expansion(char* line_args[], const macro_t* macro, FILE* output) {
    /* Read each token in expansion and replace with corresponding line_arg */
}

void add_macro(char* line, hash_table* t)
{
    char* macro_type;
    char* key;
    macro_t* m;

    m = xmalloc(sizeof(macro_t));
    key = xcalloc(MAX_WORD_LENGTH + 1, sizeof(char));
    macro_type = strtok(line, " "); 
    if (strcmp(macro_type, MACRO_DEF) == 0) {
        /* Add as simple replacement macro */
        strcpy(key, strtok(NULL, " "));
        char* expansion = strtok(NULL, NEWLINE_CHAR);
        m->expansion = xcalloc(strlen(expansion) + 1, sizeof(char));
        strcpy(m->expansion, expansion);
        init_def_macro(m);
        insert(t, key, m);
    } else if (strcmp(macro_type, FUNC_DEF) == 0) {
        /* Add as function macro */
        /* Key is the entire function including the args */
        strcpy(key, strtok(NULL, "("));
        char* arg_list = strtok(NULL, " ");
        char* expansion = strtok(NULL, NEWLINE_CHAR);
        init_fun_macro(m, arg_list, expansion);
        insert(t, key, m);

    }
    /* TODO: Add file inclusion macro */
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
    printf("%s\n", key_cpy);
    size_t i = 0;

    /* find first occurence of RESERVED_MACRO_CHAR */
    /* TODO: Error handling, missing arg list, or parentheses */
    while (i < strlen(key_cpy) - 1 && key_cpy[i] != ')') {
        while(key_cpy[i] != RESERVED_MACRO_CHAR)
            ++i;
        char* arg = strtok(key_cpy + i, ",)");
        argv_tmp[argc] = xcalloc(strlen(arg) + 1, sizeof(char));
        strcpy(argv_tmp[argc], arg);
        argc++;
        ++i;
    }
    m->argc = argc - 1;
    m->argv = xcalloc(m->argc, sizeof(char*));
    for (i = 0; i < m->argc; ++i){
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

