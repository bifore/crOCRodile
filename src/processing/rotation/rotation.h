//
// Created by tristan9 on 12/4/16.
//

#ifndef CROCRODILE_ROTATION_H
#define CROCRODILE_ROTATION_H
#define PI 3.14159265358979323846

#include <math.h>
#include "../../util/image.h"
#include "histogram.h"

typedef struct
{
    int x, y;
    int width, heigth;
} Rectangle;

int max_array_pos(float *a, int nb);

float get_variance(int *array);

int find_rotation_angle(Image *image);

int max_bounds(Rectangle *histo, int length);

Vector *detect_blocks(Image *surface);

Rectangle *get_lines(Image *surface, int *nb, Rectangle *bounds);

void reset_array(int *array, int length);

Rectangle *get_chars(Image *surface, Rectangle *lines, int length, int *nb);

void trim_chars(Image *surface, Rectangle *chars, int length);

double img_get_orientation_factor_radians(Image *image);

Image *img_autorotate(Image *img);

Image *img_rotate(Image *img, double radians);

#endif //CROCRODILE_ROTATION_H
