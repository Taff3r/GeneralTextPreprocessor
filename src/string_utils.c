#include <string.h>
#include "string_utils.h"
#include "definitions.h"
#include "util.h"
#include <stdio.h>
char** tokenize(char* line, size_t* sz)
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
char* search_and_replace(char* str, char* token, char* replacement) 
{
    printf("str = %s, token = %s, replacement = %s\n", str, token, replacement);
    char* replaced;
    replaced = xcalloc(strlen(str) + strlen(replacement) + 1, sizeof(char));
    char* f; 
    if ((f = strstr(str, token)) != 0) {
        /* Copy the first part of the string */
        strncpy(replaced, str, f - str); 
        size_t head_size = f - str;
        /* Insert the replacement */
        strncpy(replaced + strlen(replaced), replacement, strlen(replacement));
        /* Insert the tail of the string */
        strcpy(replaced + strlen(replaced), str + head_size + strlen(token));
    }
    return replaced;

}
