#include <string.h>
#include "string_utils.h"
#include "definitions.h"
#include "util.h"

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
