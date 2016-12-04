#include <SDL_rect.h>
#include "rotation.h"

#include "../../util/maths.h"
#include "histogram.h"
#include "../../util/image.h"

#define ANGLE 20

Image *historietta_de_la_rotacion(Image *old_image, double angle);

Vector *characters;
int iter = 0;

int maxArrayPos(float *a, int nb)
{
    float max = 0;
    int pos = 0;
    for(int i = 0; i < nb; i++)
    {
        if (a[i] > max)
        {
            max = a[i];
            pos = i;
        }
    }
    return pos;
}
float getVariance(int *array)
{
    float variance = 0;
    float average = 0;
    int nb = array[0];
    for(int i=1; i <= nb; i++)
    {
        average += array[i];
    }
    average /= nb;
    for(int i=1; i <= nb; i++)
    {
        variance += pow((array[i] - average), 2);
    }
    variance /= nb;
    
    return variance;
}
int findAngle(Image *image)
{
    int histo[image->height + 1];
    float variance[ANGLE*2 + 1];
    int i = 0;
    for(int angle=-ANGLE; angle <= ANGLE; angle++)
    {
        rHistogram(image, histo, angle);
        variance[i] = getVariance(histo);
        
        i++;
    }
    return -(-ANGLE + maxArrayPos(variance, ANGLE*2 + 1));
}
int maxSDL_Rect(SDL_Rect *histo, int length)
{
    int pos = 0;
    for(int i = 0; i < length; i++)
    {
        if(histo[i].w > histo[pos].w)
            pos = i;
    }
    return pos;
}
Vector *detectBlocks(Image *surface)
{
    characters = vec_create(8);
    int w = surface->width;
    int h = surface->height;
    
    //int **matrix = mat_from_img(surface);
    Matrix *matrix = mat_from_img(surface);
    matrix = andMatrix(hrlsa(matrix), vrlsa(matrix), w, h);
    Image *img = img_from_matrix(matrix);
    
    int length = 0;
    SDL_Rect bounds = {.x = 0, .y = 0, .w = surface->width, .h = surface->height - 1};
    SDL_Rect *blocks = getChars(img, &bounds, 1, &length);
    for(int i = 0; i < length; i++)
    {
        // pour le show
        //add_vector(characters, &blocks[i]);
        int length2 = 0;
        SDL_Rect *lines = getLines(surface, &length2, &blocks[i]);
        
        // pour le show
        //for(int j = 0; j < length2; j++)
        //    add_vector(characters, &lines[j]);
        int length3 = 0;
        SDL_Rect *chars = getChars(surface, lines, length2, &length3);
        trimChars(surface, chars, length3);
        if(length3 == 0)
        {
            for(int j = 0; j < length2; j++)
                vec_add(characters, &lines[j]);
        }
        for(int k = 0; k < length3; k++)
            vec_add(characters, &chars[k]);
    }
    
    return characters;
}
SDL_Rect *getLines(Image *surface, int *nb, SDL_Rect *bounds)
{
    int histo[surface->height];
    vHistogramBounds(surface, histo, bounds);
    SDL_Rect *lines = malloc(sizeof(SDL_Rect));
    
    int y = bounds->y;
    int seuil = 0;
    int done = 1;
    int started = 0;
    for(int line = bounds->y + 1; line - bounds->y < bounds->h - 1; line++)
    {
        if (histo[line] == 0)
            seuil++;
        if(histo[line] > 0 && histo[line-1] == 0 && !started)
        {
            if(seuil < 5 && !done)
            {
                *nb -= 1;
                done = 1;
                y = line - seuil;
            }
            else
            {
                lines = realloc(lines, sizeof(SDL_Rect) * (*nb+1));
                lines[*nb].h = 0;
                lines[*nb].x = bounds->x;
                lines[*nb].y = line - seuil;
                lines[*nb].w = bounds->w;
                
                y = line - seuil;
                started = 1;
            }
            seuil = 0;
        }
        if((histo[line] > 0 && histo[line+1] == 0) && started)
        {
            lines[*nb].h = bounds->y + line - y + 1;
            y = 0;
            *nb += 1;
            done = 0;
            started = 0;
        }
    }
    return lines;
}
void drawBlocks(Image *surface, Vector *vect)
{
    srand((unsigned int) time(NULL));
    Uint8 c[3][3] = {{255, 0, 0}, {0, 255, 0}, {0, 0, 255}};
    Uint32 color;
    int p;
    
    int x, y, x1, y1;
    for(int i = 0; i < vect->used; i++)
    {
        p = rand() % 3;
        p = 1;
        color = SDL_MapRGB(surface->format, c[p][0], c[p][1], c[p][2]);
        x = vect->data[i].x;
        y = vect->data[i].y;
        x1 = x + vect->data[i].w;
        y1 = y + vect->data[i].h;
        for(int j = x; j < x1; j++)
        {
            setPixel(surface, j, y, color);
            setPixel(surface, j, y1, color);
        }
        for(int j = y; j < y1; j++)
        {
            setPixel(surface, x, j, color);
            setPixel(surface, x1, j, color);
        }
    }
}
void resetArray(int *array, int length)
{
    for(int i=0; i < length; i++)
    {
        array[i] = 0;
    }
}
SDL_Rect *getChars(Image *surface, SDL_Rect *lines, int length, int *nb)
{
    int *histo = calloc(surface->w, sizeof(int));
    SDL_Rect *chars = malloc(sizeof(SDL_Rect));
    Uint32 pixel;
    Uint8 g;
    
    for(int i = 0; i < length; i++)
    {
        // histogramme
        for(int x = lines[i].x; x - lines[i].x < lines[i].w; x++)
        {
            for(int y=lines[i].y; y - lines[i].y < lines[i].h; y++)
            {
                pixel = getPixel(surface, x, y);
                SDL_GetRGB(pixel, surface->format, &g, &g, &g);
                
                if(g == 0)
                    histo[x] += 1;
            }
        }
        
        // d�tection des caract�res
        // d�passement de bornes ?
        for(int column = lines[i].x; column - lines[i].x < lines[i].w;
            column++)
        {
            if(histo[column] > 0 && histo[column-1] == 0)
            {
                chars[*nb].x = column;
                chars[*nb].y = lines[i].y;
                chars[*nb].h = lines[i].h;
            }
            if(histo[column] > 0 && histo[column+1] == 0)
            {
                chars[*nb].w = column - chars[*nb].x;
                *nb += 1;
                chars = realloc(chars, sizeof(SDL_Rect)*(*nb+1));
            }
        }
        resetArray(histo, surface->w);
    }
    
    return chars;
}
void trimChars(Image *surface, SDL_Rect *chars, int length)
{
    int *histo = calloc(surface->w, sizeof(int));
    Uint32 pixel;
    Uint8 g;
    int up;
    for(int i=0; i < length; i++)
    {
        for(int y = chars[i].y; y - chars[i].y < chars[i].h; y++)
        {
            for(int x = chars[i].x; x - chars[i].x < chars[i].w; x++)
            {
                pixel = getPixel(surface, x, y);
                SDL_GetRGB(pixel, surface->format, &g, &g, &g);
                
                if(g == 0)
                    histo[y-chars[i].y] += 1;
            }
        }
        
        int ori_y = chars[i].y;
        int ori_h = chars[i].h;
        up = 1;
        
        for(int line = ori_y + 1; line - ori_y < ori_h - 1; line++)
        {
            if((histo[line-ori_y] && !histo[line-ori_y-1] && up)
               || (histo[0] && up))
            {
                chars[i].h = chars[i].h - (line - chars[i].y);
                chars[i].y = line;
                up = 0;
            }
            
            if(histo[line-ori_y] && !histo[line-ori_y+1] && !histo[ori_h-1])
            {
                chars[i].h = line - chars[i].y;
            }
        }
        resetArray(histo, surface->w);
    }
}

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
