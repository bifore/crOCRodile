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

int find_rotation_angle(Image *image);

Image *img_autorotate(Image *img);

Image *rotate_manual_image(Image *img, double degrees);

#endif //CROCRODILE_ROTATION_H
