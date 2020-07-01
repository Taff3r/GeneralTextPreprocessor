
# Intro
How are you?
l L l
https://instagram.com/simontenggren one  two  three  four
Are you having a simontenggren day https://instagram.com/Simon22 years old. one  two  three  four

#include "util.h"
#include "process_file.h"
#include <string.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <malloc.h>
#include <unistd.h>
#include "globals.h"

char current_path[MAX_WORD_LENGTH << 3];
char current_file[MAX_WORD_LENGTH];
int main(int argc, char** argv)
{
    char* file_name        = NULL;
    char* output_file_name = NULL;
    FILE* input            = NULL;
    FILE* output           = NULL;
    if (argc > 1) {
        file_name = malloc(sizeof(char) * (strlen(argv[1]) + 1));
        strcpy(file_name, argv[1]);
        strcpy(current_file, file_name);
        input = fopen(file_name, "r");
        getcwd(current_path, (MAX_WORD_LENGTH << 3) - 1);
        if (!input) {
            uerror_no_exit("No such file!\n");
            goto CLEANUP;
        }
    } else {
        input = stdin;
        strcpy(current_file, "stdin");
        
    }
      
    if (argc > 2) {
        output_file_name = malloc(sizeof(char) * (strlen(argv[2]) + 1));
        strcpy(output_file_name, argv[2]);
        output = fopen(output_file_name, "w");
    } else 
        output = stdout; 
        
    process(input, output);
    
CLEANUP:
    if (file_name != NULL)
        free(file_name);
    if (output_file_name != NULL)
        free(output_file_name);
    fclose(input);
    fclose(output);
}



## Another header
cool cool cool
FORSEN
cool
some other text
hello there
more text...
Another: hello there, Simon
fucking fuckfuck
LAMDA
CAPL
Hello there my name is Simon and I am 22 years old.

#### fucking fuckfuck Simon
Some more text
text
txt
ttt
t BIGEPS
asdfasf asdfasdfasd
asdfasdfasdfasdfasdfasdf
test
test
test
test
test
test
SOME
