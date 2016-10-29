#include "maths.h"

#include <stdlib.h>
#include <math.h>

Matrix * mat_create(int width, int height, float * matrix)
{
    Matrix * mat = (Matrix *) malloc(sizeof(Matrix));
    mat->width = width;
    mat->height = height;
    if(matrix == NULL)
        mat->mat = (float *) calloc((size_t) (width * height), sizeof(float));
    else
        mat->mat = matrix;
    return mat;
}

void mat_free(Matrix * mat, bool content)
{
    if(content)
        free(mat->mat);
    free(mat);
}

Matrix * mat_multiply(Matrix * a, Matrix * b)
{
    Matrix * r = mat_create(b->width, a->height, NULL);
    for(int y = 0; y < r->height; ++y)
    {
        for(int x = 0; x < r->width; ++x)
        {
            for(int d = 0; d < a->width; ++d)
            {
                int v = (int) (a->mat[y * a->width + d] * b->mat[d * b->width + x]);
                r->mat[y * r->width + x] += v;
            }
        }
    }
    return r;
}

Matrix * mat_transpose(Matrix * a)
{
    Matrix * r = mat_create(a->height, a->width, NULL);
    for(int y = 0; y < r->height; ++y)
    {
        for(int x = 0; x < r->width; ++x)
        {
            r->mat[y * r->width + x] = a->mat[x * a->width + y];
        }
    }
    return r;
}

void mat_apply(float (* func) (float), Matrix * mat)
{
    for(int i = 0; i < mat->width * mat->height; ++i)
    {
        mat->mat[i] = (*func) (mat->mat[i]);
    }
}

float mth_sigmoid(float x)
{
    return 1 / (1 + expf(-x));
}

float mth_apply(float (* func) (float), float x)
{
    return (* func) (x);
}
