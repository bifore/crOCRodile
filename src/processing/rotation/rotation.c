#include "rotation.h"
#include "histogram.h"
#include <math.h>

#define ANGLE 20

Image *historietta_de_la_rotacion(Image *old_image, double angle);

int max_array_pos(float *a, int nb)
{
    float max = 0;
    int pos = 0;
    for (int i = 0; i < nb; i++)
    {
        if (a[i] > max)
        {
            max = a[i];
            pos = i;
        }
    }
    return pos;
}

float get_variance(int *array)
{
    float variance = 0;
    float average = 0;
    int nb = array[0];
    for (int i = 1; i <= nb; i++)
    {
        average += array[i];
    }
    average /= nb;
    for (int i = 1; i <= nb; i++)
    {
        variance += pow((array[i] - average), 2);
    }
    variance /= nb;
    
    return variance;
}

int find_rotation_angle(Image *image)
{
    int histo[image->height + 1];
    float variance[ANGLE * 2 + 1];
    int i = 0;
    for (int angle = -ANGLE; angle <= ANGLE; angle++)
    {
        r_histogram(image, histo, angle);
        variance[i] = get_variance(histo);
        
        i++;
    }
    return -(-ANGLE + max_array_pos(variance, ANGLE * 2 + 1));
}

Image *img_autorotate(Image *img)
{
    double best_angle_degrees = find_rotation_angle(img);
    printf("Automatically suggested rotation angle : %lf", best_angle_degrees);
    return img_rotate(img, best_angle_degrees);
}

Image *img_rotate(Image *img, double degrees)
{
    if (degrees == 0.)
    {
        printf("Rotation not needed.\n");
        return img;
    }
    printf("Applying a rotation of %lf degrees on image %p", degrees, img);
    return historietta_de_la_rotacion(img, degrees);
}

Image *historietta_de_la_rotacion(Image *old_image, double angle_deg)
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
    new_img->width = wo;
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
    
    for (int y = 0; y < ho; y++)
    {
        for (int x = 0; x < wo; x++)
        {
            /* New coordinates */
            int xo = (int) (ocx + (cos_rad * (x - ncx) + sin_rad * (y - ncy)));
            int yo = (int) (ocy + (cos_rad * (y - ncy) - sin_rad * (x - ncx)));
            if (xo >= 0 && yo >= 0 && xo < old_image->width && yo < old_image->height)
            {
                set_pixel(new_img, x, y, get_pixel(old_image, xo, yo));
            }
        }
    }
    return new_img;
}

//region ROTATION FUNCTIONS GRAVEYARD
/*
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
    
    if (hypo == 0)
    {
        sin = 0;
    }
    
    printf("sin(α) = %.6lf\n", sin);
    printf("Needing rotation of : %.1lf degrees.\n", asin(sin) * 180 / PI);
    return asin(sin);
}

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


    for (int orig_y = 0; orig_y < dest_heigth; orig_y++)
    {
        float u = rowu;
        float v = rowv;

        bool *pDst = destination + (dest_delta * orig_y);

        for (int orig_x = 0; orig_x < dest_width; orig_x++)
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

    for (int orig_y = 0; orig_y < height; ++orig_y)
    {
        for (int orig_x = 0; orig_x < width; ++orig_x)
        {
            int u = (int) (cos(-angle_radians) * orig_x + sin(-angle_radians) * orig_y);
            int v = (int) (-sin(-angle_radians) * orig_x + cos(-angle_radians) * orig_y);
            //printf("%d,%d %d,%d\n", u, v, orig_x, orig_y);
            destination->raster[(v + height / 2) * width + (u + width / 2)] = origin->raster[orig_y * width + orig_x];
        }
    }
}
 */
//endregion
