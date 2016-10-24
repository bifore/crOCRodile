#ifndef IO_IMAGE_H_
#define IO_IMAGE_H_

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdint.h>

GdkPixbuf *img_load(const char *name);
void img_save(const guchar *data, const char *name, int w, int h);

#endif /* !IO_IMAGE_H_ */
