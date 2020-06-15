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
        if (tokens[i][strlen(tokens[i]) - 1] == '\n'){
            put_nl = 1;
            strtok(tokens[i], "\n");
        }
        /* This only changes things when the token might be a function call
         * I think 
         */
        tokens[i] = strtok(tokens[i], "(");

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

/*
 * TODO: Add table here to allow recursion.
 *
 */
void expand_macro(char* token, char* line, const macro_t* macro, FILE* output)
{
    switch (macro->type)  {
        case DEF:
            {
                fputs(macro->expansion, output);
            }
            break;
        case FUN:
            {
                /* TODO Missing ')' error */
                /* Read all args */
                size_t i = 0;
                
                char* line_args[macro->argc];
                /* Find first left parentheses */
                while(line[i] != '(')
                    i++;
                line = line + i + 1; /* Line is stack allocated so its ok */
                i = 0;
                line_args[i] = strtok(line, ",)");
                for (i = 1; i < macro->argc; ++i)
                    line_args[i] = strtok(NULL, ",)");
                format_expansion(line_args, macro, output);
            }
            break;
        case INC:
            break;
        default:
            break;
    }
}
/*
 * TODO: Include table here for recursion.
 */
void format_expansion(char* line_args[], const macro_t* macro, FILE* output) {
    size_t t_cnt;
    size_t i;
    size_t k;
    unsigned found; 
    unsigned put_nl = 0;
    /* Read each token in expansion and replace with corresponding line_arg */
    char** expansion_tokens = tokenize(macro->expansion, &t_cnt);
    for (i = 0; i < t_cnt; ++i) {
        if (expansion_tokens[i][strlen(expansion_tokens[i]) - 1] == '\n') {
           put_nl = 1; 
           expansion_tokens[i] = strtok(expansion_tokens[i], "\n");
        }
        found = 0;
        for (k = 0; k < macro->argc; ++k){
            /* The arg can be inside a token aswell
             * But try this first for now TODO
             * Its better to do a search and replace on the line.
             * */
            if (strcmp(macro->argv[k], expansion_tokens[i]) == 0) {
               found = 1; 
               break;
            }
            /* Try to find the arg inside the token */
        }
        if (found)
            fputs(line_args[k], output);
        else 
            fputs(expansion_tokens[i], output);
        if (put_nl)
            fputs("\n", output);
        else
            fputs(" ", output);
        put_nl = 0;
        found = 0;
    }
    fputs(NEWLINE_CHAR, output);
    for (i = 0; i < t_cnt; ++i)
        free(expansion_tokens[i]);
    free(expansion_tokens);
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
    size_t i = 0;
    //size_t len = strlen(arg_list);
    size_t len = strlen(arg_list);
    /* find first occurence of RESERVED_MACRO_CHAR */;
    /* TODO: Error handling, missing arg list, or parentheses */
    while (i < len){
        /* Find position of first RESERVED_MACRO_CHAR */
        while(key_cpy[i] != RESERVED_MACRO_CHAR)
            ++i;
        /* Find position of next ',' or ')' */
        size_t n = i;
        while (key_cpy[n] != ',' &&  key_cpy[n] != ')')
            n++;
        size_t k;
        argv_tmp[argc] = xcalloc(n - i + 1, sizeof(char));
        for (k = 0; k < n - i; ++k)
            argv_tmp[argc][k] = key_cpy[i + k];
        argc++;
        i += n + 1; 
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

