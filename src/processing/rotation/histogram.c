#include <math.h>
#include <SDL_rect.h>
#include "../../util/image.h"
#include "histogram.h"
#include "../../util/maths.h"

void grayHistogram(Image *surface, int *array)
{
    char pixel;
    Uint8 gray;
    for(int y=0; y < surface->height; y++)
    {
        for(int x=0; x < surface->width; x++)
        {
            pixel = getPixel(surface, x, y);
            //SDL_GetRGB(pixel, surface->format, &gray, &gray, &gray);
            array[gray]++;
        }
    }
}
void vHistogram(Image *surface, int *array)
{
    Uint32 pixel;
    Uint8 gray;
    int sum;
    for(int y = 0; y < surface->height; y++)
    {
        sum = 0;
        for(int x = 0; x < surface->width; x++)
        {
            pixel = getPixel(surface, x, y);
            
            if(!gray)
                sum++;
        }
        array[y] = sum;
    }
}
void vHistogramBounds(Image *surface, int *array, SDL_Rect *bounds)
{
    Uint32 pixel;
    Uint8 gray;
    int sum;
    for(int y = bounds->y; y - bounds->y < bounds->h; y++)
    {
        sum = 0;
        for(int x = bounds->x; x - bounds->x < bounds->w; x++)
        {
            pixel = getPixel(surface, x, y);
            //SDL_GetRGB(pixel, surface->format, &gray, &gray, &gray);
            
            if(!gray)
                sum++;
        }
        array[y] = sum;
    }
}
void hHistogram(Image *surface, int *array)
{
    Uint32 pixel;
    Uint8 gray;
    int sum;
    for(int x = 0; x < surface->width; x++)
    {
        sum = 0;
        for(int y = 0; y < surface->height; y++)
        {
            pixel = getPixel(surface, x, y);
            //SDL_GetRGB(pixel, surface->format, &gray, &gray, &gray);
            
            if(!gray)
                sum++;
        }
        array[x] = sum;
    }
}
void hHistogramBounds(Image *surface, int *array, SDL_Rect *bounds)
{
    Uint32 pixel;
    Uint8 gray;
    int sum;
    for(int x = bounds->x; x - bounds->x < bounds->w; x++)
    {
        sum = 0;
        for(int y = bounds->y; y - bounds->y < bounds->h; y++)
        {
            pixel = getPixel(surface, x, y);
            //SDL_GetRGB(pixel, surface->format, &gray, &gray, &gray);
            
            if(!gray)
                sum++;
        }
        array[x] = sum;
    }
}
void rHistogram(Image *surface, int *array, int angle)
{
    char pixel;
    Uint8 gray;
    float tany = 0;
    int nx = 0;
    int ny = 0; // nouvelles valeurs pour x et y
    int sum;
    int processed = 0;
    for(int y=0; y < surface->height; y++)
    {
        sum = 0;
        nx = 0;
        ny = y;
        tany = tan(3.14159*angle/180);
        
        if(y + surface->width * tany < surface->height)
        {
            while(nx >= 0 && nx < surface->width && ny >= 0 && ny < surface->height)
            {
                nx = nx + 1;
                ny = y + nx * tany;
                if(nx >= 0 && nx < surface->width && ny >= 0 && ny < surface->height)
                {
                    pixel = getPixel(surface, nx, ny);
                    //SDL_GetRGB(pixel, surface->format, &gray, &gray, &gray);
                    if(!gray)
                        sum++;
                }
            }
            array[processed+1] = sum;
            processed++;
        }
    }
    array[0] = processed;
}
Matrix *hrlsa(Matrix *matrix)
{
    int nb;
    for(int y = 0; y < matrix->height; y++)
    {
        nb = 0;
        for(int x = 0; x < matrix->width; x++)
        {
            if(!matrix->mat[y*matrix->width+x])
            {
                if(nb < 45)
                {
                    for(int i = x - nb; i < x; i++)
                        matrix->mat[y*matrix->width+i] = 0;
                }
                nb = 0;
            }
            else
            {
                nb++;
            }
        }
    }
    return matrix;
}
Matrix *vrlsa(Matrix *matrix)
{
    int nb;
    for(int y = 0; y < matrix->height; y++)
    {
        nb = 0;
        for(int x = 0; x < matrix->width; x++)
        {
            if(!matrix->mat[y*matrix->width+x])
            {
                if(nb < 20)
                {
                    for(int i = y - nb; i < y; i++)
                        matrix->mat[i*matrix->width+x] = 0;
                }
                nb = 0;
            }
            else
            {
                nb++;
            }
        }
    }
    return matrix;
}
