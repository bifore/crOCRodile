#include "../../src/utils/vector.h"
#include "../../src/utils/error.h"

int main()
{
    Vector * vec = vec_create(2);
    if(vec == NULL)
    {
        fail("Fail to create vector");
    }
    vec_add(vec, 1);
    vec_add(vec, 2);
    vec_add(vec, 3);
    vec_add(vec, 4);
    if(*(int *) vec_get(vec, 1) != 2 || *(int *) vec_get(vec, 3) != 4)
    {
        fail("Wrong append of the vector");
    }
    vec_set(vec, 2, 42);
    if(*(int *) vec_get(vec, 2) != 42)
    {
        fail("Wrong vector set");
    }
    vec_delete(vec, 0);
    if(*(int *) vec_get(vec, 0) != 2)
    {
        fail("Wrong vector delete");
    }
    vec_free(vec);
    return 0;
}
