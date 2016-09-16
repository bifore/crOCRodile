#include "image.h"

Image * img_create(BITMAP * bmp)
{
    int pixel_nb = bmp->info->width * bmp->info->height;
    Image * img = (Image *) malloc(sizeof(Image));
    img->width = bmp->info->width;
    img->height = bmp->info->height;
    img->x_root = 0;
    img->y_root = 0;
    img->raster = (bool *) malloc(sizeof(bool) * pixel_nb);
    for(unsigned y = 0; y < bmp->info->height; ++y)
    {
        int oset = (bmp->info->height - 1 - y) * bmp->info->width;
        for(unsigned x = 0; x < bmp->info->width; ++x)
        {
            int value = bmp->raster[(oset + x) * 4 + 1];
            img->raster[(y * bmp->info->width) + x] = value < 128;
        }
    }
    return img;
}


void img_free(Image * img)
{
    free(img->raster);
    free(img);
}


Image * img_crop(Image * img, int x, int y, int width, int height)
{
    int pixel_nb = width * height;
    Image * result = (Image *) malloc(sizeof(Image));
    result->width = width;
    result->height = height;
    result->x_root = img->x_root + x;
    result->y_root = img->y_root + y;
    result->raster = (bool *) malloc(sizeof(bool) * pixel_nb);
    for(int yi = 0; yi < height; ++yi)
    {
        for(int xi = 0; xi < width; ++xi)
        {
            int opos = ((yi + y) * img->width) + xi + x;
            result->raster[yi * width + xi] = img->raster[opos];
        }
    }
    return result;
}


void img_crop_ip(Image * img, int x, int y, int width, int height)
{
    Image * result = img_crop(img, x, y, width, height);
    img->width = width;
    img->height = height;
    img->x_root = result->x_root;
    img->y_root = result->y_root;
    free(img->raster);
    img->raster = result->raster;
    free(result);
}


Image * img_crop_border(Image * img, bool ip)
{
    int y = 0, x = 0;
    bool is_empty = true;
    while(is_empty && y < img->height)
    {
        for(int x = 0; x < img->width; ++x)
        {
            if(img->raster[y * img->width + x])
            {
                is_empty = false;
                break;
            }
        }
        if(is_empty)
            ++y;
    }
    is_empty = true;
    while(is_empty && x < img->width)
    {
        for(int y = 0; y < img->height; ++y)
        {
            if(img->raster[y * img->width + x])
            {
                is_empty = false;
                break;
            }
        }
        if(is_empty)
            ++x;
    }
    int width = img->width, height = img->height;
    is_empty = true;
    while(is_empty && height >= 0)
    {
        for(int x = 0; x < img->width; ++x)
        {
            if(img->raster[(height - 1) * img->width + x])
            {
                is_empty = false;
                break;
            }
        }
        if(is_empty)
            --height;
    }
    is_empty = true;
    while(is_empty && width >= 0)
    {
        for(int y = 0; y < img->height; ++y)
        {
            if(img->raster[y * img->width + width - 1])
            {
                is_empty = false;
                break;
            }
        }
        if(is_empty)
            --width;
    }
    if(ip)
    {
        img_crop_ip(img, x, y, width - x, height - y);
        return NULL;
    }
    return img_crop(img, x, y, width - x, height - y);
}


void img_print(Image * img)
{
    printf("D => (%i x %i) ", img->width, img->height);
    printf("@ => (%i, %i)\n/", img->x_root, img->y_root);
    for(int x = 0; x < img->width; ++x)
        printf("-");
    printf("\\\n");
    for(int y = 0; y < img->height; ++y)
    {
        printf("|");
        for(int x = 0; x < img->width; ++x)
        {
            printf(img->raster[y * img->width + x] ? "#" : " ");
        }
        printf("|\n");
    }
    printf("\\");
    for(int x = 0; x < img->width; ++x)
        printf("-");
    printf("/\n");
}


Image * img_extract_character(Image * img)
{
    // search an entry point
    bool as_find = false;
    int xa, ya;
    for(int y = 0; y < img->height && !as_find; ++y)
    {
        for(int x = 0; x < img->width && !as_find; ++x)
        {
            if(img->raster[y * img->width + x])
            {
                as_find = true;
                ya = y;
                xa = x;
            }
        }
    }
    if(!as_find)
        return NULL;
    Vector * pixels = vec_create(10);
    _img_explore_character(img, pixels, xa, ya);
    Image * character = (Image *) malloc(sizeof(Image));
    character->width = img->width;
    character->height = img->height;
    character->x_root = img->x_root;
    character->y_root = img->y_root;
    character->raster = calloc(img->width * img->height, sizeof(bool));
    for(int i = 0; i < pixels->size; ++i)
        character->raster[vec_get_int(pixels, i)] = true;
    img_crop_border(character, true);
    vec_free(pixels, true);
    return character;
}

void _img_explore_character(Image * img, Vector * pixels, int x, int y)
{
    int p_index = y * img->width + x;
    int p_index_lr = (y + 1) * img->width + x + 1;
    vec_add_int(pixels, p_index);
    img->raster[p_index] = false;
    if(x > 0 && img->raster[p_index - 1])
        _img_explore_character(img, pixels, x - 1, y);
    if(y > 0 && img->raster[p_index - img->width])
        _img_explore_character(img, pixels, x, y - 1);
    if(x < img->width - 1 && img->raster[p_index + 1])
        _img_explore_character(img, pixels, x + 1, y);
    if(y < img->height - 1 && img->raster[p_index + img->width])
        _img_explore_character(img, pixels, x, y + 1);
    if(x > 0 && y > 0 && img->raster[p_index - img->width - 1])
        _img_explore_character(img, pixels, x - 1, y - 1);
    if(x < img->width - 1 && y > 0 && img->raster[p_index - img->width + 1])
        _img_explore_character(img, pixels, x + 1, y - 1);
    if(x < img->width - 1 && y < img->height - 1 && img->raster[p_index_lr])
        _img_explore_character(img, pixels, x + 1, y + 1);
    if(x > 0 && y < img->height - 1 && img->raster[p_index + img->width - 1])
        _img_explore_character(img, pixels, x - 1, y + 1);
}
