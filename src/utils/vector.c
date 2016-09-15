#include "vector.h"
#include "error.h"

Vector * vec_create(int capacity)
{
    Vector * vec = (Vector *) malloc(sizeof(Vector));
    vec->size = 0;
    vec->capacity = capacity;
    vec->data = malloc(sizeof(void *) * capacity);
    return vec;
}

void vec_free(Vector * vec)
{
   for(int i = 0; i < vec->size; ++i)
       free(vec->data[i]);
   free(vec->data);
   free(vec);
}

Vector * vec_add(Vector * vec, void * value)
{
    if(vec->size == vec->capacity)
    {
        if(vec->capacity < 10)
            vec->capacity = 10;
        vec->capacity *= VECTOR_GROWTH_RATE;
        vec->data = realloc(vec->data, sizeof(void *) * vec->capacity);
    }
    vec->data[vec->size] = value;
    ++vec->size;
    return vec;
}

Vector * vec_set(Vector * vec, int index, void * value)
{
    if(index >= vec->size)
    {
        fail("Vector: index out of range");
    }
    vec->data[index] = value;
    return vec;
}

void * vec_get(Vector * vec, int index)
{
    if(index >= vec->size)
    {
        fail("Vector: index out of range");
    }
    return vec->data[index];
}

Vector * vec_delete(Vector * vec, int index)
{
    if(index >= vec->size)
    {
        fail("Vector: index out of range");
    }
    for(int i = index; i < vec->size - 1; ++i)
    {
        vec->data[index] = vec->data[index + 1];
    }
    --vec->size;
    return vec;
}

Vector * vec_add_int(Vector * vec, int value)
{
    int * ptr = malloc(sizeof(int));
    *ptr = value;
    return vec_add(vec, (void *) ptr);
}


int vec_get_int(Vector * vec, int index)
{
    return *(int *) vec_get(vec, index);
}
