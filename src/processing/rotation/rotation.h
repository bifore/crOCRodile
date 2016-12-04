//
// Created by tristan9 on 12/4/16.
//

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

Image *img_autorotate(Image *img);

Image *img_rotate(Image *img, double radians);

#endif //CROCRODILE_ROTATION_H
