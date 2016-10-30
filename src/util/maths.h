#ifndef UTILS_MATHS_H_
#define UTILS_MATHS_H_

#include <stdbool.h>

typedef struct
{
    int width;
    int height;
    float *mat;
} Matrix;

Matrix *mat_create(int width, int height, float *matrix);

void mat_free(Matrix *mat, bool content);

Matrix *mat_multiply(Matrix *a, Matrix *b);

Matrix *mat_transpose(Matrix *a);

Matrix *mat_substract(Matrix *a, Matrix *b);

void mat_substract_ip(Matrix *a, Matrix *b);

void mat_apply(float (*func)(float), Matrix *mat);

Matrix *mat_cpy(Matrix *a);

Matrix *ary_multiply(Matrix *a, Matrix *b);

void ary_multiply_ip(Matrix *a, Matrix *b);

float mth_sigmoid(float x);

float mth_sigmoid_prime(float x);

float mth_apply(float (*func)(float), float x);

#endif /* !UTILS_MATHS_H_ */
