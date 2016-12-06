#include "distance.h"

#include "math.h"

Vector *image_set;

void addImage(Image *img)
{
    vec_add(image_set, img);
}

float euclidean(Image *a, Image *b)
{
    float dist = 0;
    for(int y = 0; y < a->height; ++y)
        for(int x = 0; x < a->width; ++x)
        {
            int i = y * a->width + x;
            float t = abs(a->raster[i] - b->raster[i]);
            dist += t * t;
        }
    return sqrtf(dist);
}

Vector *dist_getVect()
{
    return image_set;
}

Image *knn(int k, Image *img, float (*dist) (Image *, Image *))
{
    if(!image_set)
        image_set = vec_create(8);
    Image **ns = calloc(k, sizeof(Image *));
    float *f = malloc(k * sizeof(float));
    for(int i = 0; i < k; ++i)
        f[i] = -1;
    for(int i = 0; i < image_set->size; ++i)
    {
        Image *cur = vec_get(image_set, i);
        float d = (*dist) (img, cur);
        for(int u = 0; u < k; ++u)
            if(f[u] == -1 || f[u] > d)
            {
                f[u] = d;
                ns[u] = cur;
            }
    }
    /*Image *best = NULL;
    float max = -1;
    float minD = 255 * 20 * 20;
    for(int i = 0; i < k; ++i)
    {
        if(!ns[i])
            continue;
        char c = ns[i]->character;
        int s = 0;
        for(int u = 0; u < k; ++u)
            if(ns[u]->character == c)
                ++s;
        if(s > max || (s == max && minD > f[i]))
        {
            best = ns[i];
            max = s;
            minD = f[i];
        }
    }*/
    Image *best = NULL;
    float minD = 255 * 20 * 20;
    for(int i = 0; i < k; ++i)
    {
        if(!ns[i])
            continue;
        float d = 0;
        for(int u = 0; u < k; ++u)
            if(ns[u] && ns[u]->character == ns[i]->character)
                d += f[u];
        if(d < minD)
        {
            best = ns[i];
            minD = d;
        }
    }
    free(ns);
    free(f);
    if(best)
        best->error = minD;
    return best;
}

void writeDistance(char *path, Vector *img_vec)
{
    FILE *file = fopen(path, "w");
    for(int i = 0; i < img_vec->size; ++i)
    {
        Image *img = (Image *) vec_get(img_vec, i);
        char *raster_c = (char *) calloc(50, sizeof(char));
        for(int u = 0; u < 50; ++u)
            for(int k = 0; k < 8; ++k)
            {
                raster_c[u] <<= 1;
                raster_c[u] += img->raster[u * 8 + k];
            }
        fwrite(&img->character, sizeof(char), 1 *  sizeof(char), file);
        //fwrite(raster_c, sizeof(char), 50, file);
        fwrite(img->raster, sizeof(char), 20 * 20 * sizeof(char), file);
        free(raster_c);
    }
    fclose(file);
}

void readDistance(char *path, int font)
{
    if(!image_set)
        image_set = vec_create(8);
    FILE *file = fopen(path, "r");
    if(!file)
        return;
    fseek(file, 0L, SEEK_END);
    int sz = (ftell(file) - 1) / 51;
    rewind(file);
    for(int i = 0; i < sz; ++i)
    {
        char *raster_c = (char *) malloc(50 * sizeof(char));
        Image *img = malloc(sizeof(Image));
        fread(&img->character, sizeof(char), 1 * sizeof(char), file);
        //fread(raster_c, sizeof(char), 50, file);
        img->width = 20;
        img->height = 20;
        img->x_root = -1;
        img->y_root = -1;
        img->trueWidth = 20;
        img->trueHeight = 20;
        img->font = font;
        img->raster = malloc(400 * sizeof(char));
        fread(img->raster, sizeof(char), 20 * 20 * sizeof(char), file);
        /*for(int u = 0; u < 50; ++u)
            for(int k = 0; k < 8; ++k)
                img->raster[u * 8 + k] = (raster_c[u] >> k) & 0x1;*/
        vec_add(image_set, img);
        free(raster_c);
    }
    fclose(file);
}
