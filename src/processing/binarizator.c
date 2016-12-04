/*
** Created by tristan9 on 10/30/16.
*/

#include <limits.h>
#include "binarizator.h"
#include "../util/image.h"

int average_color(guchar *p);

guchar *get_point_delta(guchar *orig, int dx, int dy, int row_s, int col_s);

void binarize_around(guchar *original_raster,
                     char *bin_raster,
                     int y,
                     int row_s,
                     int x,
                     int col_s);

Image *binarize(GdkPixbuf *file)
{
    printf("Starting binarization...\n");
    
    Image *img;
    
    //region GET_ITERATION_DATA
    guchar *origin = gdk_pixbuf_get_pixels(file);
    int pixel_size = gdk_pixbuf_get_has_alpha(file) ? 4 : 3;
    int line_length = gdk_pixbuf_get_rowstride(file);
    //endregion
    
    //region INIT_NEW_IMAGE
    img = (Image *) malloc(sizeof(Image));
    img->width = gdk_pixbuf_get_width(file);
    img->height = gdk_pixbuf_get_height(file);
    img->trueHeight = img->height;
    img->trueWidth = img->trueWidth;
    img->x_root = 0;
    img->y_root = 0;
    img->raster = (char *) malloc(img->width * img->height * sizeof(char));
    //endregion
    
    printf("Image has a pixel size and total rows length of (%d, %d)", pixel_size, line_length);
    
    //region ITARATION
    for (int y = 0; y < img->height; ++y) {
        for (int x = 0; x < img->width; ++x) {
            guchar *p = origin + y * line_length + x * pixel_size;
            binarize_around(
                    p,
                    img->raster,
                    y,
                    img->height,
                    x,
                    img->width
            );
        }
    }
    //endregion
    
    return img;
}



void binarize_around(
        guchar *original_raster,
        char *bin_raster,
        int y,
        int row_s,
        int x,
        int col_s)
{
    guchar *p_ul = get_point_delta(original_raster, -1, -1, row_s, col_s);
    guchar *p_uc = get_point_delta(original_raster, 0, -1, row_s, col_s);
    guchar *p_ur = get_point_delta(original_raster, +1, -1, row_s, col_s);
    guchar *p_ml = get_point_delta(original_raster, -1, 0, row_s, col_s);
    guchar *p_mr = get_point_delta(original_raster, +1, 0, row_s, col_s);
    guchar *p_dl = get_point_delta(original_raster, -1, +1, row_s, col_s);
    guchar *p_dc = get_point_delta(original_raster, 0, +1, row_s, col_s);
    guchar *p_dr = get_point_delta(original_raster, +1, +1, row_s, col_s);

    bool ul_exists = x - 1 >= 0 && y - 1 >= 0;
    bool uc_exists = y - 1 >= 0;
    bool ur_exists = x + 1 < col_s && y - 1 > 0;
    bool ml_exists = x - 1 >= 0;
    bool mr_exists = x + 1 <= col_s;
    bool dl_exists = x - 1 >= 0 && y + 1 < row_s;
    bool dc_exists = y + 1 < row_s;
    bool dr_exists = x + 1 < col_s && y + 1 < row_s;

    int ul_color = ul_exists ? average_color(p_ul) : INT_MIN;
    int uc_color = uc_exists ? average_color(p_uc) : INT_MIN;
    int ur_color = ur_exists ? average_color(p_ur) : INT_MIN;
    int ml_color = ml_exists ? average_color(p_ml) : INT_MIN;
    int mr_color = mr_exists ? average_color(p_mr) : INT_MIN;
    int dl_color = dl_exists ? average_color(p_dl) : INT_MIN;
    int dc_color = dc_exists ? average_color(p_dc) : INT_MIN;
    int dr_color = dc_exists ? average_color(p_dr) : INT_MIN;

    int valid = 0;
    int sum = 0;

    int around[8] = {
            ul_color,       // 0 = upleft
            uc_color,       // 1 = upcenter
            ur_color,       // 2 = upright
            ml_color,       // 3 = middleleft
            mr_color,       // 4 = middleright
            dl_color,       // 5 = downleft
            dc_color,       // 6 = downcenter
            dr_color        // 7 = downright
    };

    for (int i = 0; i < 8; ++i)
    {
        valid += around[i] != * (int *) NULL;
        sum += around[i] != INT_MIN ? around[i] : 0;
    }

    float mean = ((float) sum / (float) valid);

    if (ul_exists)
        bin_raster[(y - 1) * row_s + (x - 1)] = ul_color > mean;
    if (uc_exists)
        bin_raster[(y - 1) * row_s + x] = uc_color > mean;
    if (ur_exists)
        bin_raster[(y - 1) * row_s + (x + 1)] = ur_color > mean;
    if (ml_exists)
        bin_raster[y * row_s + (x - 1)] = ml_color > mean;
    if (mr_exists)
        bin_raster[y * row_s + (x + 1)] = mr_color > mean;
    if (dl_exists)
        bin_raster[(y + 1) * row_s + (x - 1)] = dl_color > mean;
    if (dc_exists)
        bin_raster[(y + 1) * row_s + x] = dc_color > mean;
    if (dr_exists)
        bin_raster[(y + 1) * row_s + (x + 1)] = dr_color > mean;
}

int average_color(guchar *p)
{
    return (p[0] + p[1] + p[2]) / 3;
}

guchar *get_point_delta(guchar *orig, int dx, int dy, int row_s, int col_s)
{
    return orig + dx * col_s + dy * row_s;
}
