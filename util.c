#include "util.h"
#include <stdio.h>
#include <stdlib.h>
void uerror(const char* msg)
{
    fprintf(stderr, msg);
    fprintf(stderr, "Exiting \n");
    exit(1);
}
