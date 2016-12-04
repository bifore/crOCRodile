//
// Created by tristan9 on 12/4/16.
//

#ifndef CROCRODILE_ROTATION_H
#define CROCRODILE_ROTATION_H
#define PI 3.14159265358979323846

#include <math.h>
#include "../util/image.h"

double img_get_orientation_factor_radians(Image *image);

Image *img_autorotate(Image *img);

Image *img_rotate(Image *img, double radians);

#endif //CROCRODILE_ROTATION_H
