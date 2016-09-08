#ifndef UTILS_LK_LIST_H_
#define UTILS_LK_LIST_H_

#include <stdlib.h>

typedef struct _LkList
{
    void * value;
    struct _LkList * next;
    struct _LkList * previous;
    struct _LkList * head;
} LkList;

LkList * lkl_create();
void lkl_free(LkList * lkList);
LkList * lkl_add(LkList * lkList, void * value);
LkList * lkl_delete(LkList * lklList);
int lkl_count(LkList * lkList);

#endif /* !UTILS_LK_LIST_H_ */
