#include "image.h"

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

