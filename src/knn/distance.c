#include "distance.h"

#include "math.h"

float euclidean(Image *a, Image *b)
{
    float dist = 0;
    for(int y = 0; y < a->height; ++y)
        for(int x = 0; x < a->width; ++x)
        {
            int i = y * a->width + x;
            float t = absf(a->raster[i] - b->raster[i]);
            dist += t * t;
        }
    return sqrtf(dist);
}
