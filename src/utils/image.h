#ifndef UTILS_IMAGE_H_
#define UTILS_IMAGE_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "../io/bmpReader.h"
#include "vector.h"

typedef struct
{
    int width;
    int height;
    bool * raster;
    int x_root, y_root;
} Image;

Image * img_create(BITMAP * bmp);
void img_free(Image * img);

Image * img_crop(Image * img, int x, int y, int width, int height);
void img_crop_ip(Image * img, int x, int y, int width, int height);
Image * img_crop_border(Image * img, bool ip);

void img_print(Image * img);

Image * img_extract_charater(Image * img);
void _img_explore_character(Image * img, Vector * pixels, int x, int y);

#endif /* !UTILS_IMAGE_H_ */
