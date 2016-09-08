#include "safe.h"
#include "error.h"
#include "lkList.h"

void initSafe()
{
    lkList * ptrs = lk_create();
}

void * smalloc(int size)
{
    void * ptr = malloc(size);
    lk_add(ptrs, ptr);
    return ptr;
}

void sfree(void * ptr)
{
    ptrs = ptrs->head;
    bool find = false;
    while(!find)
    {
        if(ptrs == NULL)
        {
            fail("Safe: unable to find the pointer");
        }
        if(ptrs->value == ptr)
        {
            find = true;
        }
        else
        {
            ptrs = ptrs->next;
        }
    }
    ptrs = lk_delete(ptrs);
    free(ptr);
}
