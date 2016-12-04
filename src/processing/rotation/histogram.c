#include "rotation.h"
#include <math.h>

void histogram_r(Image *surface, int *array, int angle)
{
    char pixel;
    float tany = 0;
    int nx = 0;
    int ny = 0; // nouvelles valeurs pour orig_x et orig_y
    int sum;
    int processed = 0;
    for (int y = 0; y < surface->height; y++)
    {
        sum = 0;
        nx = 0;
        ny = y;
        tany = (float) tan(PI * angle / 180);
        
        if (y + surface->width * tany < surface->height)
        {
            while (nx >= 0 && nx < surface->width && ny >= 0 && ny < surface->height)
            {
                nx = nx + 1;
                ny = (int) (y + nx * tany);
                if (nx >= 0 && nx < surface->width && ny >= 0 && ny < surface->height)
                {
                    pixel = img_get_pixel(surface, nx, ny);
                    if (!pixel)
                        sum++;
                }
            }
            array[processed + 1] = sum;
            processed++;
        }
    }
    array[0] = processed;
}

