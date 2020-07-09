#include "process_file.h"
#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include <string.h>
#include "hashmap.h"
#include "definitions.h"
#include "string_utils.h"
#include "file_inclusion.h"
#include "globals.h"
#include "funcpointers.h"

/*
 * Read all macro defintions and put them in the map.
 */
long long line_number = 0;
void process(FILE* input, FILE* output)
{
    hash_table* map = new_hash_table(cmp, hash, delete_key, delete_val);
    char line[MAX_LINE_LENGTH];
    while(1) {
        /* Assume no line longer than MAX_LINE_LENGTH chars */
        char* r = fgets(line,MAX_LINE_LENGTH - 1, input);
        if (r == NULL)
            break;
        else if (comment_cnt == 0 && strcmp(line, NEWLINE_CHAR) == 0) /* Empty line */
                fputs("\n", output);
            /* normal word write to output */
        else if (comment_cnt == 0 && line[0] != RESERVED_MACRO_CHAR )
                write_line(map, line, output);
        else /* Add since it is a macro line. */
                add_macro(line, map);

        ++line_number;
    }
    free(macro_keys);
    delete_hash_table(map);
} 

size_t cnt = 0;
char** macro_keys = NULL;
void m_keys(const hash_table* macros) {
    if (has_new_keys) {
        free(macro_keys);
        macro_keys = (char**) keys(macros, &cnt);
        qsort(macro_keys, cnt, sizeof(char*), string_cmp);
        has_new_keys = 0;
    } 
}
void write_line(hash_table* macros, char* line, FILE* output)
{
    /* Check if line contains any of the macros in the table,
     * if so expand them if possible 
     */
    macro_t* m;
    size_t i;

    /* Check if new keys are available and sorts them if so */
    m_keys(macros);

    for (i = 0; i < cnt; ++i) {
        while (contains(line, macro_keys[i])) {
            m = lookup(macros, macro_keys[i]);
            if (m != NULL) {
                expand_macro(macro_keys[i], line, m, macros);
            }
        } 
    }
    fputs(line, output);
}

void recursive_check_line(const hash_table* macros, char* line)
{
    /* Check if line contains any of the macros in the table,
     * if so expand them if possible 
     */
    macro_t* m;
    size_t i;

    /* Check if new keys are available and sorts them if so */
    m_keys(macros);

    for (i = 0; i < cnt; ++i) {
        while (contains(line, (char*) macro_keys[i])) {
            m = lookup(macros, macro_keys[i]);
            if (m != NULL) {
                expand_macro(macro_keys[i], line, m, macros);
            }
        } 
    }
}

void expand_macro(char* key, char* line, const macro_t* macro, const hash_table* macros)
{
    switch(macro->type) {
        case DEF:
            {
                def_m* m = macro->macro;
                char* tmp = search_and_replace(line, key, m->expansion);
                recursive_check_line(macros, tmp);
                strcpy(line, tmp);
                free(tmp);
            }
            break;
        case FUN:
            {
                char* expanded = expand_function(key, line, macro);
                recursive_check_line(macros, expanded);
                strcpy(line, expanded);  
                free(expanded);
            }
            break;
        case FLE:
            {
                /* Replace the key in the line */
                file_m* m = macro->macro;
                char* expanded_file = expand_file(m->file);
                char* replaced = search_and_replace(line, key, expanded_file);
                strcpy(line, replaced);
                free(expanded_file);
                free(replaced);
            }
        default:
            break;
    }
}

char* expand_function(const char* key, char* line, const macro_t* macro) 
{
    func_m* m = macro->macro;
    char** line_args = xcalloc(m->argc, sizeof(char*));
    char* key_il;
    char line_cpy[MAX_LINE_LENGTH]; /* TODO correct size */
    size_t pos_of_parentheses;

    /* Find instance of key in line */
    strcpy(line_cpy, line);
    key_il = strstr(line_cpy, key);

    /* Find the pos of parentheses*/
    pos_of_parentheses = key_il - line_cpy + strlen(key);
    char* line_p = line_cpy + pos_of_parentheses;

    size_t l_par, r_par, last_delim, i, k;
    l_par = r_par = last_delim = i = k = 0;

    if (line_p[i] != '(')
        formatted_uerror("Missing '(' in function call.\n", NULL);

    last_delim = i++; 
    l_par++;
    /* Read the args inside the parentheses */
    /* TODO ADD ERROR HANDLING */
    for (; k < m->argc && line_p[i] != '\n' && l_par > r_par; ++i) {
        if (line_p[i] == '(')
            ++l_par;
        else if (line_p[i] == ')') {
            ++r_par;
            if (l_par == r_par)
                goto COPY;
        }
        else if (line_p[i] == ',')
            if (l_par == r_par + 1) {
COPY:           
                line_args[k] = xcalloc(i - last_delim, sizeof(char)); 
                strncpy(line_args[k++], line_p + last_delim  + 1, i - last_delim - 1);
                last_delim = i;
            }
    }
    /* Replaced the arguments provided with to the expansion */
    char* expansion = search_and_replace_all(m->expansion, m->argv, \
            line_args, m->argc);

    char* concat;
    concat = xcalloc(strlen(key) + m->argc * MAX_WORD_LENGTH + 1, sizeof(char));
    char key_cpy[strlen(key) + 1];
    strcpy(key_cpy, key);
    strcat(concat, key_cpy);
    strcat(concat, "(");
    for (i = 0; i < m->argc; ++i) {
        strcat(concat, line_args[i]);
        if( i + 1 != m->argc)
            strcat(concat, ",");
    }
    strcat(concat, ")");
    /* Replace the key and the arg list in the string with the expansion */
    char* final = search_and_replace(line, concat, expansion);
    for(i = 0; i < m->argc; ++i)
        free(line_args[i]);
    free(line_args);
    free(concat);
    free(expansion);
    return final;
}

unsigned has_new_keys = 0;
size_t comment_cnt    = 0;
/*
 * Adds macros to the hash table
 */
void add_macro(char* line, hash_table* t)
{
    char* macro_type;
    char* key;
    macro_t* m;
    char line_cpy[strlen(line) + 1]; /* Needed for error handling */

    m = xmalloc(sizeof(macro_t));
    key = xcalloc(MAX_WORD_LENGTH + 1, sizeof(char));
    strcpy(line_cpy, line);
    macro_type = strtok(line, " \t\n"); 
    if (macro_type[0] != RESERVED_MACRO_CHAR){ /* TODO This is a stupid fix */
        free(m);
        free(key);
        return;
    } 
    if (comment_cnt == 0 && strcmp(macro_type, MACRO_DEF) == 0) {
        /* Add as simple replacement macro */
        def_m* d_m = xmalloc(sizeof(def_m));
        strcpy(key, strtok(NULL, " \t"));
        if (lookup(t, key))
            formatted_uerror("Multiple definitions of key: %s\n", key);

        char* expansion = strtok(NULL, NEWLINE_CHAR); /* A lot of memory */
        trim_leading_whitespace(expansion);
        d_m->expansion = xcalloc(strlen(expansion) + 1, sizeof(char));
        strcpy(d_m->expansion, expansion);
        /* TODO: Changed here removed init_def*/
        m->macro = d_m;
        m->type = DEF;
        insert(t, key, m);
    } else if (comment_cnt == 0 && strcmp(macro_type, FUNC_DEF) == 0) {
        strcpy(key, strtok(NULL, "("));
        /* Error check */
        if (*(line_cpy + strlen(macro_type) + strlen(key) + 1) != '(')
            formatted_uerror("Missing \"(\" in argument function definition!\n", NULL);

        /* Add as function macro */
        char* arg_list = strtok(NULL, ")");
        trim_whitespace(arg_list);
        char* expansion = strtok(NULL, NEWLINE_CHAR);
        trim_leading_whitespace(expansion);
        init_fun_macro(m, arg_list, expansion);
        insert(t, key, m);
    } else if (comment_cnt == 0 && strcmp(macro_type, INC_DEF) == 0) {
        char* path;
        path = xcalloc(MAX_LINE_LENGTH - 1, sizeof(char));

        strcpy(path, strtok(NULL, NEWLINE_CHAR));
        include_file(path, t);
        free(path);
        /* Turns out these aren't needed here. TODO refactor */
        free(m);
        free(key);

    } else if (comment_cnt == 0 && strcmp(macro_type, FILE_DEF) == 0) {
        char* path;
        path = xcalloc(MAX_LINE_LENGTH - 1, sizeof(char));
        strcpy(key, strtok(NULL, " \t"));
        strcpy(path, strtok(NULL, NEWLINE_CHAR));
        trim_leading_whitespace(path);
        init_file_macro(m, path);
        insert(t, key, m);
        free(path);
    } else if (comment_cnt == 0 && strcmp(macro_type, UNDEF_DEF) == 0) {
        strcpy(key, strtok(strtok(NULL, " \t"), NEWLINE_CHAR));
        trim_leading_whitespace(key);
        if (lookup(t, key)) {
            remove_from_table(t, key);
            free(key);
            free(m);
        } else 
            formatted_uerror("Cannot remove key: %s! It is not defined\n", key);
    } else if (comment_cnt == 0 && strcmp(macro_type, IF_DEF) == 0) {
        strcpy(key, strtok(strtok(NULL, " \t"), NEWLINE_CHAR));
        trim_leading_whitespace(key);
        if (strcmp(key, BOOL_FALSE) == 0) { /* $IF FALSE */
            ++comment_cnt; 
        } else if (strcmp(key, BOOL_TRUE) == 0) { /* $IF TRUE */
            ; /* DO NOTHING */
        } else {
            if (!lookup(t, key)) { /* $IF SOMEUNDEFINEDMACRO */ 
                ++comment_cnt;
            } 
        }
        free(key);
        free(m);
    } else if (comment_cnt == 0 && strcmp(macro_type, IFN_DEF) == 0) {
        strcpy(key, strtok(strtok(NULL, " \t"), NEWLINE_CHAR));
        trim_leading_whitespace(key);
        if (strcmp(key, BOOL_FALSE) == 0) { /* $IFN FALSE */
            ; /* DO NOTHING */
        } else if (strcmp(key, BOOL_TRUE) == 0) { /* $IFN TRUE */
            comment_cnt++;
        } else {
            if (lookup(t, key)) { /* $IFN SOMEDEFINEDMACRO */ 
                ++comment_cnt;
            }
        }
        free(key);
        free(m);
    } else if (strcmp(macro_type, ENDIF_DEF) == 0) {
        if (comment_cnt > 0)
            --comment_cnt;
        free(key);
        free(m);
    } else
        formatted_uerror("Unrecoginized token: %s\n", macro_type);
    has_new_keys = 1;
}

void init_fun_macro(macro_t* m, char* arg_list, char* expansion)
{
    func_m* f_m = xmalloc(sizeof(func_m));
    char key_cpy[strlen(arg_list) + 1];
    size_t argc = 0;
    char** argv_tmp = xcalloc(MAX_ARGC, sizeof(char*));

    m->type = FUN;
    strcpy(key_cpy, arg_list);
    f_m->expansion = xcalloc(strlen(expansion) + 1, sizeof(char));
    strcpy(f_m->expansion, expansion);
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
    f_m->argc = argc;
    f_m->argv = xcalloc(argc, sizeof(char*));
    for (i = 0; i < argc; ++i){
        f_m->argv[i] = xcalloc(strlen(argv_tmp[i]) + 1, sizeof(char));
        strcpy(f_m->argv[i], argv_tmp[i]);
    }
    for (i = 0; i < MAX_ARGC; ++i)
        free(argv_tmp[i]);
    free(argv_tmp);
    m->macro = f_m;
}


void init_file_macro(macro_t* m, char* path_to_file)
{
    file_m* f_m = xmalloc(sizeof(file_m));
    m->type = FLE;
    f_m->file = fopen(path_to_file, "r");
    if (!f_m->file) 
        formatted_uerror("Cannot open file %s\n", path_to_file);
    m->macro = f_m;

}
