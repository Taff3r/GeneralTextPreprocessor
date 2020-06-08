#include "util.h"
#include <string.h> 
#include <stdio.h> 
#include <stdlib.h> 
int main(int argc, char** argv)
{
    char* file_name;
    char* output_name;


    output_name = NULL;
    if (argc < 2)
        uerror("Missing file to process!\n");

    file_name = malloc(sizeof(char) * (strlen(argv[1]) + 1));
    strcpy(file_name, argv[1]);
    printf("FILENAME : %s\n", file_name);

    if (argc >= 3) {
        output_name = malloc(sizeof(char) * (strlen(argv[2]) + 1));
        strcpy(output_name, argv[2]);
        printf("OUTPUT NAME: %s\n", output_name);
    }
    goto CLEANUP;
    
CLEANUP:
    free(file_name);
    if (output_name != NULL)
        free(output_name);
}
