#include "process_file.h"
#include <stdlib.h>
#include "util.h"
#include <string.h>
#include "hashmap.h"
#include "definitions.h"
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
            free(m->argv[i]); /* TODO: Add remove subpointers to args */
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
        /* Assume no word longer than 65536 chars */
        char* r = fgets(line,MAX_LINE_LENGTH - 1, input);
        if (r == NULL)
            break;
        // normal word write to output
        if (line[0] != RESERVED_MACRO_CHAR)
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
    char* token = strtok(line, " ");
    int add_nl = 0;
    do {
        if (token[strlen(token) - 1] == '\n') {
            token = strtok(token, "\n");
            add_nl = 1;
        }
        m = lookup(macros, token);
        if (m == NULL) 
            fputs(token, output);
        else 
            fputs(m->expansion, output);

        if (add_nl){
            fputs("\n", output);
            add_nl = 0;
        } else 
            fputs(" ", output);

    } while((token = strtok(NULL, " ")) != NULL);
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
        char* expansion = strtok(NULL, "\n");
        m->expansion = xcalloc(strlen(expansion) + 1, sizeof(char));
        strcpy(m->expansion, expansion);
        init_def_macro(m);
        insert(t, key, m);
    } else if (strcmp(macro_type, FUNC_DEF) == 0) {
        /* Add as function macro */
        strcpy(key, strtok(NULL, "("));
        
        char* arg_t = strtok(NULL, ")");
        char* args = xcalloc(strlen(arg_t) + 1, sizeof(char)); // TODO: OPT: Can avoid alloc?
        strcpy(args, arg_t);
        char* expansion = strtok(NULL, "\n");
        m->expansion = xcalloc(strlen(expansion) + 1, sizeof(char));
        strcpy(m->expansion, expansion);
        init_fun_macro(m, args);
        insert(t, key, m);
        free(args);
    }
    /* TODO: Add file inclusion macro */
}

void init_fun_macro(macro_t* m, char* args)
{
    m->type = FUN;
    size_t argc = 0;
    char* argv_tmp[MAX_ARGC];
    char* arg_t;

    arg_t = strtok(args, ", \n");
    if (arg_t == NULL)
        uerror_no_exit("Missing argument from function defintion\n"); /* TODO: Fix clean exit when missing args in function def */

    do {
       argv_tmp[argc++] = arg_t;
    } while((arg_t = strtok(NULL, ", \n")) != NULL);
    
    m->argv = xmalloc(argc * sizeof(char*));
    m->argc = argc;
    size_t i;
    for (i = 0; i < argc; ++i){
        m->argv[i] = xmalloc((strlen(argv_tmp[i]) + 1) * sizeof(char));
        strcpy(m->argv[i], argv_tmp[i]);
    }
}
void init_def_macro(macro_t* m)
{
    m->argv = NULL;
    m->type = DEF;
    m->argc = 0;
}

