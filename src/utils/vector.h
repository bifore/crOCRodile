#ifndef UTILS_VECTOR_H_
#define UTILS_VECTOR_H_

#define VECTOR_GROWTH_RATE 2

#include <stdlib.h>
#include <stdbool.h>

typedef struct
{
    int size;
    int capacity;
    int cellSize;
    void * * data;
} Vector;

Vector * vec_create(int capacity);
void vec_free(Vector * vec, bool free_data);
Vector * vec_delete(Vector * vec, int index);

Vector * vec_add(Vector * vec, void * value);
Vector * vec_set(Vector * vec, int index, void * value);
void * vec_get(Vector * vec, int index);

Vector * vec_add_int(Vector * vec, int value);
int vec_get_int(Vector * vec, int index);


#endif /* !UTILS_VECTOR_H_ */
