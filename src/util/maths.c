#include "maths.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

Matrix *mat_create(int width, int height, float *matrix)
{
    Matrix *mat = (Matrix *) malloc(sizeof(Matrix));
    mat->width = width;
    mat->height = height;
    if (matrix == NULL)
        mat->mat = (float *) calloc(width * height, sizeof(float));
    else
        mat->mat = matrix;
    return mat;
}

void mat_free(Matrix *mat, bool content)
{
    if (content)
        free(mat->mat);
    free(mat);
}

void mat_print(Matrix *mat)
{
    printf("/");
    for(int i = 0 ; i < mat->width; ++i)
        printf("-");
    printf("\\\n");
    for(int y = 0; y < mat->height; ++y)
    {
        printf("|");
        for(int x = 0; x < mat->width; ++x)
            printf("%f", mat->mat[y * mat->width + x]);
        printf("|\n");
    }
    printf("\\");
    for(int i = 0 ; i < mat->width; ++i)
        printf("-");
    printf("/\n");
}

Matrix * mat_multiply(Matrix * a, Matrix * b)
{
    Matrix *r = mat_create(b->width, a->height, NULL);
    for (int y = 0; y < r->height; ++y)
    {
        for (int x = 0; x < r->width; ++x)
        {
            for (int d = 0; d < a->width; ++d)
            {
                float v = a->mat[y * a->width + d] * b->mat[d * b->width + x];
                r->mat[y * r->width + x] += v;
            }
        }
    }
    return r;
}


Matrix *ary_multiply(Matrix *a, Matrix *b)
{
    Matrix *r = mat_create(a->width, a->height, NULL);
    for (int y = 0; y < a->height; ++y)
    {
        float l = b->mat[y];
        for (int x = 0; x < a->width; ++x)
            r->mat[y * a->width + x] = a->mat[y * a->width + x] * l;
    }
    return r;
}

void ary_multiply_ip(Matrix *a, Matrix *b)
{
    for (int y = 0; y < a->height; ++y)
    {
        float l = b->mat[y];
        for (int x = 0; x < a->width; ++x)
            a->mat[y * a->width + x] *= l;
    }
}

Matrix *mat_substract(Matrix *a, Matrix *b)
{
    Matrix *r = mat_create(a->width, a->height, NULL);
    for (int i = 0; i < a->width * a->height; ++i)
        r->mat[i] = a->mat[i] - b->mat[i];
    return r;
}

void mat_substract_ip(Matrix *a, Matrix *b)
{
    for (int i = 0; i < a->width * a->height; ++i)
        a->mat[i] -= b->mat[i];
}

Matrix *mat_transpose(Matrix *a)
{
    Matrix *r = mat_create(a->height, a->width, NULL);
    for (int y = 0; y < r->height; ++y)
        for (int x = 0; x < r->width; ++x)
            r->mat[y * r->width + x] = a->mat[x * a->width + y];
    return r;
}

Matrix *mat_cpy(Matrix *a)
{
    Matrix *r = mat_create(a->width, a->height, NULL);
    memcpy(r->mat, a->mat, a->width * a->height * sizeof(float));
    return r;
}

void mat_apply(float (*func)(float), Matrix *mat)
{
    for (int i = 0; i < mat->width * mat->height; ++i)
    {
        mat->mat[i] = (*func)(mat->mat[i]);
    }
}

float mth_sigmoid(float x)
{
    return 1 / (1 + expf(-x));
}

float mth_sigmoid_prime(float x)
{
    return expf(x) / ((1 + expf(x)) * (1 + expf(x)));
}

float mth_apply(float (*func)(float), float x)
{
    return (*func)(x);
}

void mat_scalar(Matrix *a,  float s)
{
    for(int i = 0; i < a->height * a->width; ++i)
        a->mat[i] *= s;
}
