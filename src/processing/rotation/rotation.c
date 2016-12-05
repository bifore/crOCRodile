#include "rotation.h"
#include "histogram.h"
#include <math.h>

#define THRESHOLD_VARIANCE 0.10f

//region PRIVATE METHODS

Image *make_me_sway(Image *old_image, double angle);

void print_variance(float *variance, int height);

int nb_lines_under_threshold(float *variance, int height);

float *variance(Image *image);

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

int find_rotation_angle_2(Image *image)
{
    float *variance_val = variance(image);
    print_variance(variance_val, image->height);
    int lines = nb_lines_under_threshold(variance_val, image->height);
    printf("Number of lines under THRESHOLD VARIANCE (%.2f) : %d out of %d\n",
           THRESHOLD_VARIANCE,
           lines,
           image->height
    );
    double ratio = (double) lines / (double) image->height;
    
    printf("Angular ratio is : %lf\n", ratio);
    
    double angle = (90 * (ratio / 0.5)) / 4.8;
    
    printf("\tso the suggested rotation angle is : %lf\n", angle);
    
    return (int) angle;
}

int nb_lines_under_threshold(float *variance, int height)
{
    int nblines = 0;
    for (int i = 0; i < height && variance[i] < THRESHOLD_VARIANCE; ++i)
    {
        nblines ++;
    }
    return nblines;
};

void print_variance(float *variance, int height)
{
    printf("Variance-per-3 pixels : { %f", variance[0]);
    for (int i = 1; i < height; ++i)
    {
        printf(", %f", variance[i]);
    }
    printf("}\n");
}

float *variance(Image *image)
{
    float *loads = malloc(image->height * sizeof(float));
    
    for (int y = 0; y < image->height; ++y)
    {
        float load_line = 0;
        for (int x = 0; x < image->width; ++x)
        {
            load_line += image->raster[y * image->width + x] ? 1 : 0;
        }
        loads[y] = load_line / image->width;
    }
    
    return loads;
}



//endregion
