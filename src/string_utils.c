#include <string.h>
#include "string_utils.h"
#include "definitions.h"
#include "util.h"
#include <stdio.h>
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
char* search_and_replace(char* str, char* token, char* replacement) 
{
    char* replaced;
    replaced = xcalloc(strlen(str) + strlen(replacement) + 1, sizeof(char));
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
int contains(char* str, char* token) 
{
    return strstr(str, token) != NULL;
}
