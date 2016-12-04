//
// Created by tristan9 on 12/4/16.
//

#include "rotation.h"

Image *img_c_sucks_balls_rotate(Image *old_image, double angle);

char getPixel(Image *image, int xo, int yo);

void setPixel(Image *img, int x, int y, char pixel);

double img_get_orientation_factor_radians(Image *image)
{
    int topx = 0;
    int lefty = 0;
    
    for (int i = 0; i < image->width; ++i)
    {
        if (image->raster[i])
        {
            topx = i;
            break;
        }
    }
    for (int i = 0; i < image->height; ++i)
    {
        if (image->raster[i * image->width])
        {
            lefty = i;
            break;
        }
    }
    
    printf("Found left intersection : %d\n", lefty);
    printf("Found top intersection : %d\n", topx);
    
    int hypo = (int) sqrt((pow(lefty, 2) + pow(topx, 2)));
    double sin = (double) topx / (double) hypo;
    
    if (hypo == 0) {
        sin = 0;
    }
    
    printf("sin(α) = %.6lf\n", sin);
    printf("Needing rotation of : %.1lf degrees.\n", asin(sin) * 180 / PI);
    return asin(sin);
}

Image *img_autorotate(Image *img)
{
    double best_angle_degrees = img_get_orientation_factor_radians(img);
    return img_rotate(img, best_angle_degrees);
}

Image *img_rotate(Image *img, double degrees)
{
    if (degrees == 0.) {
        printf("Rotation not needed.\n");
        return img;
    }
    return img_c_sucks_balls_rotate(img, degrees);
}

Image *img_c_sucks_balls_rotate(Image *old_image, double angle_deg)
{
    /* Trigo */
    double cos_rad = cos(PI * angle_deg / 180);
    double sin_rad = sin(PI * angle_deg / 180);
    /* Original Dimensions */
    int width = old_image->width;
    int height = old_image->height;
    
    /* New dimensions */
    int wo = abs((int) (width * cos_rad)) + abs((int) (height * sin_rad));
    int ho = abs((int) (height * cos_rad)) + abs((int) (width * sin_rad));
    
    Image *new_img = malloc(sizeof(Image));
    new_img->width= wo;
    new_img->height = ho;
    new_img->trueHeight = new_img->height;
    new_img->trueWidth = new_img->width;
    new_img->x_root = 0;
    new_img->y_root = 0;
    new_img->raster = malloc(new_img->width * new_img->height * sizeof(char));
    
    /* Middle coordinates */
    int ocx = width / 2;
    int ocy = height / 2;
    /* New middle coordinates */
    int ncx = wo / 2;
    int ncy = ho / 2;
    char pixel;
    
    for (int y = 0; y < ho; y++)
    {
        for(int x = 0; x < wo; x++)
        {
            /* New coordinates */
            int xo = (int) (ocx + (cos_rad * (x - ncx) + sin_rad * (y - ncy)));
            int yo = (int) (ocy + (cos_rad * (y - ncy) - sin_rad * (x - ncx)));
            if (xo >= 0 && yo >= 0 && xo < old_image->width && yo < old_image->height)
            {
                setPixel(new_img, x, y, getPixel(old_image, xo, yo));
            }
        }
    }
    return new_img;
}

void setPixel(Image *img, int x, int y, char pixel)
{
    img->raster[img->width * y + x] = pixel;
}

char getPixel(Image *image, int xo, int yo)
{
    return image->raster[yo * image->width + xo];
}

//region ROTATION FUNCTIONS GRAVEYARD
/*
void img_superfast_rotate(
        bool *destination, int dest_width, int dest_heigth, int dest_delta,
        bool *source, int src_width, int src_heigth, int src_delta,
        float dst_center_x, float dst_center_y,
        float src_center_x, float serc_center_y,
        float angle_rad, float scaling)
{
    src_delta /= sizeof(bool);
    dest_delta /= sizeof(bool);

    float duCol = (float) sin(-angle_rad) * (1.0f / scaling);
    float dvCol = (float) cos(-angle_rad) * (1.0f / scaling);
    float duRow = dvCol;
    float dvRow = -duCol;

    float startingu = src_center_x - (dst_center_x * dvCol + dst_center_y * duCol);
    float startingv = serc_center_y - (dst_center_x * dvRow + dst_center_y * duRow);

    float rowu = startingu;
    float rowv = startingv;


    for (int y = 0; y < dest_heigth; y++)
    {
        float u = rowu;
        float v = rowv;

        bool *pDst = destination + (dest_delta * y);

        for (int x = 0; x < dest_width; x++)
        {
            if (u > 0 && v > 0 && u < src_width && v < src_heigth)
            {
                bool *pSrc = source + (int) u + ((int) v * src_delta);
                *pDst++ = *pSrc++;
            } else
            {
                *pDst++ = 0;
            }

            u += duRow;
            v += dvRow;
        }

        rowu += duCol;
        rowv += dvCol;
    }
}

void img_dumb_rotate(Image *origin, Image *destination, double angle_radians)
{
    int height = origin->height;
    int width = origin->width;

    printf("Rotating using angle : %lf radians\n", angle_radians);
    printf("                    -> %lf degrees\n", angle_radians * 180 / PI);

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            int u = (int) (cos(-angle_radians) * x + sin(-angle_radians) * y);
            int v = (int) (-sin(-angle_radians) * x + cos(-angle_radians) * y);
            //printf("%d,%d %d,%d\n", u, v, x, y);
            destination->raster[(v + height / 2) * width + (u + width / 2)] = origin->raster[y * width + x];
        }
    }
}
 */
//endregion

