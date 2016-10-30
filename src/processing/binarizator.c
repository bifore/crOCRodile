/*
** Created by tristan9 on 10/30/16.
*/

#include <limits.h>
#include <glib/gtypes.h>
#include "binarizator.h"

int average_color(guchar *p);

guchar *get_point_delta(guchar *orig, int dx, int dy, int row_s, int col_s);

void binarize_around(Image *image,
                     guchar *ori,
                     bool *raster,
                     int len,
                     int y,
                     int row_s,
                     int x,
                     int col_s,
                     float percent_tolerance);

Image *binarize(GdkPixbuf *file, float percent_tolerance)
{
    printf("Starting binarization");
    Image *img = (Image *) malloc(sizeof(Image));

    guchar *origin = gdk_pixbuf_get_pixels(file);
    int col_s = gdk_pixbuf_get_has_alpha(file) ? 4 : 3;
    int row_s = gdk_pixbuf_get_rowstride(file);
    img->width = gdk_pixbuf_get_width(file);
    img->height = gdk_pixbuf_get_height(file);

    img->x_root = 0;
    img->y_root = 0;

    img->raster = (bool *) malloc(img->width * img->height * sizeof(bool));
    for (int y = 0; y < img->height; ++y) {
        for (int x = 0; x < img->width; ++x) {
            guchar *p = origin + y * row_s + x * col_s;
            binarize_around(
                    img,
                    p,
                    img->raster,
                    img->width * img->height,
                    y,
                    img->height,
                    x,
                    img->width,
                    percent_tolerance
            );
        }
    }
    return img;
}

void binarize_around(
        Image *final,
        guchar *ori,
        bool *raster,
        int len,
        int y,
        int row_s,
        int x,
        int col_s,
        float percent_tolerance)
{
    guchar *p_ul = get_point_delta(ori, -1, -1, row_s, col_s);
    guchar *p_uc = get_point_delta(ori, 0, -1, row_s, col_s);
    guchar *p_ur = get_point_delta(ori, +1, -1, row_s, col_s);
    guchar *p_ml = get_point_delta(ori, -1, 0, row_s, col_s);
    guchar *p_mr = get_point_delta(ori, +1, 0, row_s, col_s);
    guchar *p_dl = get_point_delta(ori, -1, +1, row_s, col_s);
    guchar *p_dc = get_point_delta(ori, 0, +1, row_s, col_s);
    guchar *p_dr = get_point_delta(ori, +1, +1, row_s, col_s);

    int ul_bounds = x - 1 >= 0 && y - 1 >= 0;
    int uc_bounds = y - 1 >= 0;
    int ur_bounds = x + 1 < col_s && y - 1 > 0;
    int ml_bounds = x - 1 >= 0;
    int mr_bounds = x + 1 <= col_s;
    int dl_bounds = x - 1 >= 0 && y + 1 < row_s;
    int dc_bounds = y + 1 < row_s;
    int dr_bounds = x + 1 < col_s && y + 1 < row_s;

    int ul = ul_bounds ? average_color(p_ul) : INT_MIN;
    int uc = uc_bounds ? average_color(p_uc) : INT_MIN;
    int ur = ur_bounds ? average_color(p_ur) : INT_MIN;
    int ml = ml_bounds ? average_color(p_ml) : INT_MIN;
    int mr = mr_bounds ? average_color(p_mr) : INT_MIN;
    int dl = dl_bounds ? average_color(p_dl) : INT_MIN;
    int dc = dc_bounds ? average_color(p_dc) : INT_MIN;
    int dr = dc_bounds ? average_color(p_dr) : INT_MIN;

    int valid = 0;
    int sum = 0;

    /*
     * 0 = upright
     * 1 = upcenter
     * 2 = upright
     * 3 = middleleft
     * 4 = middleright
     * 5 = downleft
     * 6 = downcenter
     * 7 = downright
     */
    int around[8] = {ul, uc, ur, ml, mr, dl, dc, dr};

    for (int i = 0; i < 8; ++i)
    {
        valid += around[i] != INT_MIN;
        sum += around[i] != INT_MIN ? around[i] : 0;
    }

    float mean = ((float) sum / (float) valid) - ((float) 255. * percent_tolerance);

    if (ul_bounds)
        raster[(y - 1) * row_s + (x - 1)] = ul > mean;
    if (uc_bounds)
        raster[(y - 1) * row_s + x] = uc > mean;
    if (ur_bounds)
        raster[(y - 1) * row_s + (x + 1)] = ur > mean;
    if (ml_bounds)
        raster[y * row_s + (x - 1)] = ml > mean;
    if (mr_bounds)
        raster[y * row_s + (x + 1)] = mr > mean;
    if (dl_bounds)
        raster[(y + 1) * row_s + (x - 1)] = dl > mean;
    if (dc_bounds)
        raster[(y + 1) * row_s + x] = dc > mean;
    if (dr_bounds)
        raster[(y + 1) * row_s + (x + 1)] = dr > mean;
}


int average_color(guchar *p)
{
    return (p[0] + p[1] + p[2]) / 3;
}

guchar *get_point_delta(guchar *orig, int dx, int dy, int row_s, int col_s)
{
    return orig + dx * col_s + dy * row_s;
}
