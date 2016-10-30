#include <err.h>
#include "image.h"
#include "../processing/canny.h"
#include "../defaults.h"
#include "../util/image.h"

GdkPixbuf *img_load(const char *name)
{
    GError *e = NULL;
    GdkPixbuf *b = gdk_pixbuf_new_from_file(name, &e);
    if(e != NULL)
    {
        fprintf(stderr, "Unable to read the image: %s\n", e->message);
        g_error_free(e);
        g_object_unref(b);
        return NULL;
    }
    return b;
}

void img_save(const guchar *d, const char *name, int w, int h)
{
    GdkColorspace c = GDK_COLORSPACE_RGB;
    GdkPixbuf *img = gdk_pixbuf_new_from_data(d, c, 0, 8, w, h, w * 3, NULL, NULL);
    GError *e = NULL;
    gdk_pixbuf_save(img, name, "bmp", &e, NULL);
    if(e != NULL)
    {
        fprintf(stderr, "Unable to write the image: %s\n", e->message);
        g_error_free(e);
        g_object_unref(img);
        return;
    }
    g_object_unref(img);
}

Image * img_load_IMAGE(const char *name) {
    errx(-1, "Broken for now");
    //return img_create(img_load(name));
}

void img_save_IMAGE(const Image *img, const char *name, int w, int h) {
    guchar *data = (guchar *) malloc(sizeof(guchar) * img->width * img->height * 3);
    int a = 0;
    for(int i = 0; i < img->width * img->height; ++i)
        for(int u = 0; u < 3; ++u)
            data[++a] = (guchar) (img->raster[i] ? 255 : 0);
    img_save(data, name, w, h);
}

