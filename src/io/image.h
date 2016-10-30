#ifndef IO_IMAGE_H_
#define IO_IMAGE_H_

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdint.h>
#include "../util/image.h"

GdkPixbuf *img_load(const char *name);
void img_save(const guchar *data, const char *name, int w, int h);

Image * img_load_IMAGE(const char *name);
void img_save_IMAGE(const Image *img, const char *name, int w, int h);

#endif /* !IO_IMAGE_H_ */
