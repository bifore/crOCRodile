#include "lkList.h"

LkList * lkl_create()
{
    LkList * lkList = (LkList *) malloc(sizeof(LkList));
    lkList->head = lkList;
    return lkList;
}

void lkl_free(LkList * lkList)
{
    if(lkList->head != NULL)
    {
        lkList->head->head = NULL;
        lkl_free(lkList->head);
        return;
    }
    if(lkList->next != NULL)
    {
        lkList->next->head = NULL;
        lkl_free(lkList->next);
    }
    free(lkList->value);
    free(lkList);
}

LkList * lkl_add(LkList * lkList, void * value)
{
    if(lkList->value == NULL)
    {
        lkList->value = value;
        return lkList;
    }
    LkList * next = (LkList *) malloc(sizeof(LkList));
    next->value = value;
    next->next = lkList->next;
    next->previous = lkList;
    next->head = lkList->head;
    lkList->next = next;
    if(next->next != NULL)
    {
        next->next->previous = next;
    }
    return next;
}

LkList * lkl_delete(LkList * lkList)
{
    free(lkList->value);
    if(lkList->previous != NULL)
    {
        lkList->previous->next = lkList->next;
    }
    if(lkList->next != NULL)
    {
        lkList->next->previous = lkList->previous;
    }
    if(lkList->previous == NULL && lkList->next == NULL)
    {
        return lkList;
    }
    if(lkList->previous == NULL)
    {
        return lkList->next;
    }
    return lkList->previous;
}

int lkl_count(LkList * lkList)
{
    int count = 0;
    lkList = lkList->head;
    while(lkList != NULL)
    {
        ++count;
        lkList = lkList->next;
    }
    return count;
}
