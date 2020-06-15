#include "util.h"
#include "process_file.h"
#include <string.h> 
#include <stdio.h> 
#include <stdlib.h> 
int main(int argc, char** argv)
{
    char* file_name;
    FILE* input;
    FILE* output = NULL;

    /* TODO add option to read from stdin */
    if (argc < 2)
        uerror("Missing file to process!\n");

    file_name = malloc(sizeof(char) * (strlen(argv[1]) + 1));
    strcpy(file_name, argv[1]);
    input = fopen(file_name, "r");
    if (!input) {
        uerror_no_exit("No such file!\n");
        goto CLEANUP;
    }

    output = fopen("a.pp", "w");
    process(input, output);
    
CLEANUP:
    free(file_name);
    fclose(input);
    fclose(output);
}
