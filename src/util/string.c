#include "string.h"

char *str_con(int nb, ...)
{
    va_list list;
    va_start(list, nb);
    int len = 1;
    char **strs = malloc(sizeof(char *) * nb);
    for (int i = 0; i < nb; ++i)
    {
        strs[i] = va_arg(list, char *);
        len += strlen(strs[i]);
    }
    char *dest = malloc((size_t) len);
    strcpy(dest, strs[0]);
    for (int i = 1; i < nb; ++i)
    {
        strcat(dest, strs[i]);
    }
    free(strs);
    va_end(list);
    return dest;
}
