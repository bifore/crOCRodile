#include <math.h>
#include <SDL_rect.h>
#include "../../util/image.h"
#include "histogram.h"

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
int **hrlsa(int **matrix, int width, int height)
{
    int nb;
    for(int y = 0; y < height; y++)
    {
        nb = 0;
        for(int x = 0; x < width; x++)
        {
            if(!matrix[x][y])
            {
                if(nb < 45)
                {
                    for(int i = x - nb; i < x; i++)
                        matrix[i][y] = 0;
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
int **vrlsa(int **matrix, int width, int height)
{
    int nb;
    for(int y = 0; y < height; y++)
    {
        nb = 0;
        for(int x = 0; x < width; x++)
        {
            if(!matrix[x][y])
            {
                if(nb < 20)
                {
                    for(int i = y - nb; i < y; i++)
                        matrix[x][i] = 0;
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
