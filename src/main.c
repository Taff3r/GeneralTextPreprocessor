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

    return 0;
}
