#include "rotation.h"
#include "histogram.h"
#include <math.h>

#define ANGLE 20

//region PRIVATE METHODS

Image *make_me_sway(Image *old_image, double angle);

//endregion

//region NON-WORKING

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
        histogram_r(image, histo, angle);
        variance[i] = get_variance(histo);
        
        i++;
    }
    return -(-ANGLE + max_array_pos(variance, ANGLE * 2 + 1));
}

//endregion

//region WRAPPERS

Image *rotate_auto_image(Image *img)
{
    double best_angle_degrees = find_rotation_angle(img);
    printf("Automatically suggested rotation angle : %lf\n", best_angle_degrees);
    return rotate_manual_image(img, best_angle_degrees);
}

Image *rotate_manual_image(Image *img, double degrees)
{
    if (degrees == 0.)
    {
        printf("Rotation not needed.\n");
        return img;
    }
    printf("Applying a rotation of %lf degrees on image %p", degrees, img);
    return make_me_sway(img, degrees);
}

//endregion

//region IMPLEMENTATION

Image *make_me_sway(Image *old_image, double angle_deg)
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
                img_set_pixel(new_img, x, y, img_get_pixel(old_image, xo, yo));
            }
        }
    }
    return new_img;
}

double find_rotation_angle_2(Image *image)
{
    
}

//endregion
