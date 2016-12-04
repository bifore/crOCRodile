#include "maths.h"

#include <string.h>
#include <math.h>

Matrix *mat_create(int width, int height, float *matrix)
{
    Matrix *mat = (Matrix *) malloc(sizeof(Matrix));
    mat->width = width;
    mat->height = height;
    if (matrix == NULL)
        mat->mat = (float *) calloc((size_t) (width * height), sizeof(float));
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

Matrix *mat_from_img(Image *image)
{
    float *raster_float = malloc(image->width * image->height * sizeof(float_t));
    
    for (int y = 0; y < image->height; ++y)
        for (int x = 0; x < image->width; ++x)
            raster_float[y*image->width + x] = get_pixel(image, x, y) ? 1f : 0f;
    
    return mat_create(image->width, image->height, raster_float);
}

Image *img_from_matrix(Matrix *matrix) {
    Image *img = malloc(sizeof(Image));
    img->width = matrix->width;
    img->height = matrix->height;
    img->trueWidth = img->width;
    img->trueHeight = img->trueHeight;
    img->x_root = -1;
    img->y_root = -1;
    
    char * new_raster = malloc(matrix->width * matrix->height * sizeof(char));
    for (int y = 0; y < matrix->height; ++y)
    {
        for (int x = 0; x < matrix->width; ++x)
        {
            new_raster[y*matrix->width + x] = (char) matrix->mat[y * matrix->width + x];
        }
    }
    
    img->raster = new_raster;
    return img;
}

Matrix *and_matrix(Matrix *mat1, Matrix *mat2)
{
    int width = mat1->width >= mat2->width ? mat1->width : mat2->width;
    int height = mat1->height >= mat2->height ? mat1->height : mat2->height;
    
    Matrix *and_mat = mat_create(width, height, NULL);
    
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            float mat1p = NULL;
            if (mat1->width >= x+1 && mat1->height >= y+1) {
                mat1p = mat1->mat[y*width+x];
            }
            float mat2p = NULL;
            if (mat2->width >= x+1 && mat2->height >= y+1) {
                mat2p = mat2->mat[y*width+x];
            }
            and_mat->mat[y*width+x] = (int) mat1p & (int) mat2p;
        }
    }
    
    return and_mat;
}
