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
