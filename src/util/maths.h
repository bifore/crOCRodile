#ifndef UTILS_MATHS_H_
#define UTILS_MATHS_H_

#include <stdbool.h>

typedef struct {
    int width;
    int height;
    float * mat;
} Matrix;

Matrix * mat_create(int width, int height, float * matrix);
void mat_free(Matrix * mat, bool content);

Matrix * mat_multiply(Matrix * a, Matrix * b);
Matrix * mat_transpose(Matrix * a);
void mat_apply(float (* func) (float), Matrix * mat);

float mth_sigmoid(float x);
float mth_apply(float (* func) (float), float x);

#endif /* !UTILS_MATHS_H_ */
