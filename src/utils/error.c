#include "error.h"

void fail(char * msg)
{
    fprintf(stderr, msg);
    fprintf(stderr, "\n");
    exit(EXIT_FAILURE);
}
