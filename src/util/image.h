#ifndef UTIL_IMAGE_H_
#define UTIL_IMAGE_H_

#include <gtk/gtk.h>

#include "../util/vector.h"

typedef struct
{
    char character;
    int width;
    int height;
    char *raster;
    int x_root;
    int y_root;
    int trueWidth;
    int trueHeight;
    int font;
} Image;

Image *img_create(GdkPixbuf *bmp);
Image *img_create_noBin(GdkPixbuf *bmp);

Image *img_cpy(Image *img);

void img_free(Image *img);

Image *img_crop(Image *img, int x, int y, int width, int height);

void img_crop_ip(Image *img, int x, int y, int width, int height);

Image *img_crop_border(Image *img, int ip);

void img_print(const Image *img);

Image *img_extract_character(Image *img);

void _img_explore_character(Image *img, Vector *pixels, int x, int y);

Image *img_normalize(Image *img, int size);

#endif /* UTIL_IMAGE_H_ */
