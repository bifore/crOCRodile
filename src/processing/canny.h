#ifndef PROCESSING_CANNY_H_
#define PROCESSING_CANNY_H_

#include "../io/image.h"
#include <gtk/gtk.h>
#include <math.h>
#include <stdlib.h>

typedef struct
{
    int *data;
    int *magnitude;
    float *dir;
    float *x_conv;
    float *y_conv;
    float *x_gradient;
    float *y_gradient;
    GdkPixbuf *image;
    int w;
    int h;
} Canny_filter;

Canny_filter *canny(GdkPixbuf *image);
void canny_free(Canny_filter *cf);

#endif /* !PROCESSING_CANNY_H_ */
