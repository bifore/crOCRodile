#include "image.h"

GdkPixbuf *img_load(const char *name)
{
    GError *e = NULL;
    GdkPixbuf *b = gdk_pixbuf_new_from_file(name, &e);
    if (e != NULL)
    {
        fprintf(stderr, "Unable to read the image: %s\n", e->message);
        g_error_free(e);
        g_object_unref(b);
        return NULL;
    }
    return b;
}

void img_drawRect(GdkPixbuf *img, int x, int y, int w, int h,
                  char r, char g, char b)
{
    int n = gdk_pixbuf_get_has_alpha(img) ? 4 : 3;
    int row_size = gdk_pixbuf_get_rowstride(img);
    guchar *ori = gdk_pixbuf_get_pixels(img);
    for(int i = x; i <= x + w; ++i)
    {
        guchar *p1 = ori + y * row_size + i * n;
        p1[0] = r;
        p1[1] = g;
        p1[2] = b;
        guchar *p2 = ori + (y + h) * row_size + i * n;
        p2[0] = r;
        p2[1] = g;
        p2[2] = b;
    }
    for(int i = y; i <= y + h; ++i)
    {
        guchar *p1 = ori + i * row_size + x * n;
        p1[0] = 255;
        p1[1] = 0;
        p1[2] = 0;
        guchar *p2 = ori + i * row_size + (x + w) * n;
        p2[0] = 255;
        p2[1] = 0;
        p2[2] = 0;
    }
}

void img_save_buf(GdkPixbuf *img, const char *name)
{
    GError *e = NULL;
    gdk_pixbuf_save(img, name, "bmp", &e, NULL);
    if (e != NULL)
    {
        fprintf(stderr, "Unable to write the image: %s\n", e->message);
        g_error_free(e);
        return;
    }
}

void img_save_ary(const guchar *d, const char *name, int w, int h)
{
    GdkColorspace c = GDK_COLORSPACE_RGB;
    GdkPixbuf *img = gdk_pixbuf_new_from_data(d, c, 0, 8, w, h, w * 3, NULL, NULL);
    GError *e = NULL;
    gdk_pixbuf_save(img, name, "bmp", &e, NULL);
    if (e != NULL)
    {
        fprintf(stderr, "Unable to write the image: %s\n", e->message);
        g_error_free(e);
        g_object_unref(img);
        return;
    }
    g_object_unref(img);
}

void img_save(const Image *img, const char *name, int w, int h)
{
    guchar *data = (guchar *) malloc(sizeof(guchar) * img->width * img->height * 3);
    int a = 0;
    for (int i = 0; i < img->width * img->height; ++i)
        for (int u = 0; u < 3; ++u)
            data[++a] = (guchar) (img->raster[i] ? 255 : 0);
    img_save_ary(data, name, w, h);
    free(data);
}

void img_save_bin(Image *img, const char *name)
{
    FILE *file = fopen(name, "w");
    fwrite(img->raster, sizeof(char), 20 * 20 * sizeof(char), file);
    fclose(file);
}

Image *img_read_bin(const char *name)
{
    Image *img = (Image *) malloc(sizeof(Image));
    img->x_root = -1;
    img->y_root = -1;
    img->height = 20;
    img->width = 20;
    img->raster = (char *) malloc(20 * 20 * sizeof(char));
    FILE *file = fopen(name, "r");
    fread(img->raster, sizeof(char), 20 * 20 * sizeof(char), file);
    fclose(file);
    return img;
}

