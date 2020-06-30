#include <string.h>
#include "string_utils.h"
#include "definitions.h"
#include "util.h"
#include <stdio.h>
#include <ctype.h>
#include "process_file.h"
#include "funcpointers.h"
/*
 * Turns the line into sz number of tokens, depending on the delimeters.
 */
char** tokenize(char* line, char* delimeters, size_t* sz)
{
   char** tokens;
   char* token;
   
   tokens = xcalloc(MAX_LINE_LENGTH / MAX_WORD_LENGTH, \
           sizeof(char*));
   *sz = 0;
   token = strtok(line, " ");
   do {
        tokens[*sz] = xcalloc(strlen(token) + 1, sizeof(char));
        strcpy(tokens[*sz], token);
        ++(*sz);
   } while((token = strtok(NULL, " ")) != NULL); 
   return tokens;
}
/**
 * Search the string str for the token and replaces it with the replacement.
 * Returns the a heap allocated string which has been modified. 
 */
char* search_and_replace(char* str, const char* token, char* replacement) 
{
    char* replaced;
    replaced = xcalloc(MAX_LINE_LENGTH, sizeof(char));
    char* f; 
    if ((f = strstr(str, token)) != NULL) {
        /* Copy the first part of the string */
        strncpy(replaced, str, f - str); 
        size_t head_size = f - str;
        /* Insert the replacement */
        strcpy(replaced + strlen(replaced), replacement);
        /* Insert the tail of the string */
        strcpy(replaced + strlen(replaced), str + head_size + strlen(token));
    }
    return replaced;

}

void expand_between(char* str, const size_t lower, const size_t upper, const hash_table* macros) 
{
    char* begin = str + lower + 1;
    size_t cnt, i;
    char** kys = (char**) keys(macros, &cnt);
    qsort(kys, cnt, sizeof(char*), string_cmp);
    macro_t* m;
    for (i = 0; i < cnt; ++i) {
        m = lookup(macros, kys[i]);
        if (contains(begin, kys[i])) {
            expand_macro(kys[i], begin, m, macros);
        }
    }
}
/*
 * Replaces all occurences of tokens in str, with their replacements.
 * cnt is the number of tokens to in **tokens.
 */
char* search_and_replace_all(char* str, char** tokens, char** replacements, size_t cnt) 
{
    char buffer[MAX_LINE_LENGTH];
    strcpy(buffer, str);
    size_t i;
    char* intermediate;
    for (i = 0; i < cnt; ++i){
        while(contains(buffer, tokens[i])) {
            intermediate = search_and_replace(buffer, tokens[i], replacements[i]);
            if (intermediate != NULL) {
                strcpy(buffer, intermediate);
                free(intermediate);
            }
        }
    }
    char* final = xcalloc(strlen(buffer) + 1, sizeof(char));
    strcpy(final, buffer);
    return final;
}

/*
 * Checks if a string, str, contains the token token.
 * RETURN:
 *  1 if str contains token.
 *  0 Otherwise.
 */
int contains(char* str, char* token) 
{
    if (token == NULL || str == NULL)
        return 0;
    return strstr(str, token) != NULL;
}

/*
 * Trims the str of all white space.
 */
void trim_whitespace(char* str)
{
   char* tokens[] = {" ", "\t"};
   char* replacements[] = {"", ""};
   char* trimmed = search_and_replace_all(str, tokens, replacements, 2);
   strcpy(str, trimmed);
   free(trimmed);
}

/*
 * Trims leading white space from a string.
 */
void trim_leading_whitespace(char* str)
{
    size_t i;
    size_t len;
    char* cpy = xcalloc(strlen(str) + 1, sizeof(char));
    char* orig_pos = cpy;
    strcpy(cpy, str);
    len = strlen(cpy);
    for (i = 0; i < len; ++i) {
        if (cpy[i] != ' ' && cpy[i] != '\t')
            break;
        else {
            cpy += 1;
            i--;
        }
    }
    strcpy(str, cpy);
    free(orig_pos);
}
