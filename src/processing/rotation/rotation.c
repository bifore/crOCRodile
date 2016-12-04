#include "rotation.h"
#include "histogram.h"
#include <math.h>

#define ANGLE 20

Image *historietta_de_la_rotacion(Image *old_image, double angle);

Vector *characters;

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

int max_bounds(Rectangle *histo, int length)
{
    int pos = 0;
    for (int i = 0; i < length; i++)
    {
        if (histo[i].width > histo[pos].width)
            pos = i;
    }
    return pos;
}

Vector *detect_blocks(Image *surface)
{
    characters = vec_create(8);
    int w = surface->width;
    int h = surface->height;
    
    //int **matrix = mat_from_img(surface);
    Matrix *matrix = mat_from_img(surface);
    matrix = and_matrix(hrlsa(matrix), vrlsa(matrix));
    Image *img = img_from_matrix(matrix);
    
    int length = 0;
    Rectangle bounds = {
            .orig_x = 0,
            .orig_y = 0,
            .width = surface->width,
            .heigth = surface->height - 1
    };
    Rectangle *blocks = get_chars(img, &bounds, 1, &length);
    for (int i = 0; i < length; i++)
    {
        // pour le show
        //add_vector(characters, &blocks[i]);
        int length2 = 0;
        Rectangle *lines = get_lines(surface, &length2, &blocks[i]);
        
        // pour le show
        //for(int j = 0; j < length2; j++)
        //    add_vector(characters, &lines[j]);
        int length3 = 0;
        Rectangle *chars = get_chars(surface, lines, length2, &length3);
        trim_chars(surface, chars, length3);
        if (length3 == 0)
        {
            for (int j = 0; j < length2; j++)
                vec_add(characters, &lines[j]);
        }
        for (int k = 0; k < length3; k++)
            vec_add(characters, &chars[k]);
    }
    
    return characters;
}

Rectangle *get_lines(Image *surface, int *nb, Rectangle *bounds)
{
    int histo[surface->height];
    v_histogram_bounds(surface, histo, bounds);
    Rectangle *lines = malloc(sizeof(Rectangle));
    
    int y = bounds->orig_y;
    int seuil = 0;
    int done = 1;
    int started = 0;
    for (int line = bounds->orig_y + 1; line - bounds->orig_y < bounds->heigth - 1; line++)
    {
        if (histo[line] == 0)
            seuil++;
        if (histo[line] > 0 && histo[line - 1] == 0 && !started)
        {
            if (seuil < 5 && !done)
            {
                *nb -= 1;
                done = 1;
                y = line - seuil;
            } else
            {
                lines = realloc(lines, sizeof(Rectangle) * (*nb + 1));
                lines[*nb].heigth = 0;
                lines[*nb].orig_x = bounds->orig_x;
                lines[*nb].orig_y = line - seuil;
                lines[*nb].width = bounds->width;
                
                y = line - seuil;
                started = 1;
            }
            seuil = 0;
        }
        if ((histo[line] > 0 && histo[line + 1] == 0) && started)
        {
            lines[*nb].heigth = bounds->orig_y + line - y + 1;
            y = 0;
            *nb += 1;
            done = 0;
            started = 0;
        }
    }
    return lines;
}

void reset_array(int *array, int length)
{
    for (int i = 0; i < length; i++)
    {
        array[i] = 0;
    }
}

Rectangle *get_chars(Image *surface, Rectangle *lines, int length, int *nb)
{
    int *histo = calloc((size_t) surface->width, sizeof(int));
    Rectangle *chars = malloc(sizeof(Rectangle));
    char pixel;
    
    for (int i = 0; i < length; i++)
    {
        // histogramme
        for (int x = lines[i].orig_x; x - lines[i].orig_x < lines[i].width; x++)
        {
            for (int y = lines[i].orig_y; y - lines[i].orig_y < lines[i].heigth; y++)
            {
                pixel = get_pixel(surface, x, y);
                
                if (pixel == 0)
                    histo[x] += 1;
            }
        }
        
        for (int column = lines[i].orig_x; column - lines[i].orig_x < lines[i].width;
             column++)
        {
            if (histo[column] > 0 && histo[column - 1] == 0)
            {
                chars[*nb].orig_x = column;
                chars[*nb].orig_y = lines[i].orig_y;
                chars[*nb].heigth = lines[i].heigth;
            }
            if (histo[column] > 0 && histo[column + 1] == 0)
            {
                chars[*nb].width = column - chars[*nb].orig_x;
                *nb += 1;
                chars = realloc(chars, sizeof(Rectangle) * (*nb + 1));
            }
        }
        reset_array(histo, surface->width);
    }
    
    return chars;
}

void trim_chars(Image *surface, Rectangle *chars, int length)
{
    int *histo = calloc((size_t) surface->width, sizeof(int));
    char pixel;
    int up;
    for (int i = 0; i < length; i++)
    {
        for (int y = chars[i].orig_y; y - chars[i].orig_y < chars[i].heigth; y++)
        {
            for (int x = chars[i].orig_x; x - chars[i].orig_x < chars[i].width; x++)
            {
                pixel = get_pixel(surface, x, y);
                if (!pixel)
                    histo[y - chars[i].orig_y] += 1;
            }
        }
        
        int ori_y = chars[i].orig_y;
        int ori_h = chars[i].heigth;
        up = 1;
        
        for (int line = ori_y + 1; line - ori_y < ori_h - 1; line++)
        {
            if ((histo[line - ori_y] && !histo[line - ori_y - 1] && up) || (histo[0] && up))
            {
                chars[i].heigth = chars[i].heigth - (line - chars[i].orig_y);
                chars[i].orig_y = line;
                up = 0;
            }
            
            if (histo[line - ori_y] && !histo[line - ori_y + 1] && !histo[ori_h - 1])
            {
                chars[i].heigth = line - chars[i].orig_y;
            }
        }
        reset_array(histo, surface->width);
    }
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
    char pixel;
    
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
