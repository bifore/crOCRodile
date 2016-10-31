#ifndef IO_IMAGE_H_
#define IO_IMAGE_H_

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdint.h>
#include "../util/image.h"

GdkPixbuf *img_load(const char *name);
void img_save_ary(const guchar *data, const char *name, int w, int h);

void img_drawRect(GdkPixbuf *img, int x1, int y1, int x2, int y2,
                  char r, char g, char b);
void img_save_buf(GdkPixbuf *img, const char *name);

void img_save(const Image *img, const char *name, int w, int h);

void img_save_bin(Image *img, const char *name);
Image *img_read_bin(const char *name);

#endif /* !IO_IMAGE_H_ */
