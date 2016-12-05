#ifndef CROCRODILE_ROTATION_H
#define CROCRODILE_ROTATION_H

#include "../../util/image.h"

#define PI 3.14159265358979323846

typedef struct
{
    int orig_x;
    int orig_y;
    int width;
    int heigth;
} Rectangle;

int max_array_pos(float *a, int nb);

float get_variance(int *array);

int find_rotation_angle(Image *image);

int find_rotation_angle_2(Image *image);

Image *rotate_auto_image(Image *img);

Image *rotate_manual_image(Image *img, double degrees);

#endif //CROCRODILE_ROTATION_H
