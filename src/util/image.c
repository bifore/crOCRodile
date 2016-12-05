#define PI 3.14159265358979323846

#include <string.h>
#include <math.h>
#include "image.h"

Image *img_create(GdkPixbuf *file)
{
    Image *img = (Image *) malloc(sizeof(Image));
    int n = gdk_pixbuf_get_has_alpha(file) ? 4 : 3;
    int row_size = gdk_pixbuf_get_rowstride(file);
    guchar *ori = gdk_pixbuf_get_pixels(file);
    img->x_root = 0;
    img->y_root = 0;
    img->width = gdk_pixbuf_get_width(file);
    img->height = gdk_pixbuf_get_height(file);
    img->trueWidth = img->width;
    img->trueHeight = img->height;
    img->character = '\0';
    img->font = -1;
    img->raster = (char *) malloc(sizeof(char) * img->width * img->height);
    for(int y = 0; y < img->height; ++y)
        for(int x = 0; x < img->width; ++x)
        {
            guchar *p = ori + y * row_size + x * n;
            int i = y * img->width + x;
            int v = (p[0] + p[1] + p[2]) / 3;
            img->raster[i] = v < 128;
        }
    return img;
}

Image *img_create_noBin(GdkPixbuf *file)
{
    Image *img = (Image *) malloc(sizeof(Image));
    int n = gdk_pixbuf_get_has_alpha(file) ? 4 : 3;
    int row_size = gdk_pixbuf_get_rowstride(file);
    guchar *ori = gdk_pixbuf_get_pixels(file);
    img->x_root = 0;
    img->y_root = 0;
    img->width = gdk_pixbuf_get_width(file);
    img->height = gdk_pixbuf_get_height(file);
    img->trueWidth = img->width;
    img->trueHeight = img->height;
    img->character = '\0';
    img->font = -1;
    img->raster = (char *) malloc(sizeof(char) * img->width * img->height);
    for(int y = 0; y < img->height; ++y)
        for(int x = 0; x < img->width; ++x)
        {
            guchar *p = ori + y * row_size + x * n;
            int i = y * img->width + x;
            char v = (char) ((p[0] + p[1] + p[2]) / 3);
            img->raster[i] = v;
        }
    return img;
}

void img_free(Image *img)
{
    free(img->raster);
    free(img);
}

Image *img_crop(Image *img, int x, int y, int width, int height)
{
    int pixel_nb = width * height;
    Image *result = (Image *) malloc(sizeof(Image));
    result->width = width;
    result->height = height;
    result->trueWidth = width;
    result->trueHeight = height;
    result->character = img->character;
    result->x_root = img->x_root + x;
    result->y_root = img->y_root + y;
    result->font = img->font;
    result->raster = (char *) malloc(sizeof(char) * pixel_nb);
    for (int yi = 0; yi < height; ++yi)
    {
        for (int xi = 0; xi < width; ++xi)
        {
            int opos = ((yi + y) * img->width) + xi + x;
            result->raster[yi * width + xi] = img->raster[opos];
        }
    }
    return result;
}

void img_crop_ip(Image *img, int x, int y, int width, int height)
{
    Image *result = img_crop(img, x, y, width, height);
    img->width = width;
    img->height = height;
    img->trueWidth = width;
    img->trueHeight = height;
    img->x_root = result->x_root;
    img->y_root = result->y_root;
    img->font = result->font;
    free(img->raster);
    img->raster = result->raster;
    free(result);
}

Image *img_crop_border(Image *img, int ip)
{
    int y = 0, x = 0;
    int is_empty = true;
    while (is_empty && y < img->height)
    {
        for (int x = 0; x < img->width; ++x)
        {
            if (img->raster[y * img->width + x])
            {
                is_empty = false;
                break;
            }
        }
        if (is_empty)
            ++y;
    }
    is_empty = true;
    while (is_empty && x < img->width)
    {
        for (int y = 0; y < img->height; ++y)
        {
            if (img->raster[y * img->width + x])
            {
                is_empty = false;
                break;
            }
        }
        if (is_empty)
            ++x;
    }
    int width = img->width, height = img->height;
    is_empty = true;
    while (is_empty && height >= 0)
    {
        for (int x = 0; x < img->width; ++x)
        {
            if (img->raster[(height - 1) * img->width + x])
            {
                is_empty = false;
                break;
            }
        }
        if (is_empty)
            --height;
    }
    is_empty = true;
    while (is_empty && width >= 0)
    {
        for (int y = 0; y < img->height; ++y)
        {
            if (img->raster[y * img->width + width - 1])
            {
                is_empty = false;
                break;
            }
        }
        if (is_empty)
            --width;
    }
    if (ip)
    {
        img_crop_ip(img, x, y, width - x, height - y);
        return NULL;
    }
    return img_crop(img, x, y, width - x, height - y);
}

void img_print(const Image *img)
{
    printf("D => (%i x %i) ", img->width, img->height);
    printf("@ => (%i, %i)\n/", img->x_root, img->y_root);
    for (int x = 0; x < img->width; ++x)
        printf("-");
    printf("\\\n");
    for (int y = 0; y < img->height; ++y)
    {
        printf("|");
        for (int x = 0; x < img->width; ++x)
        {
            printf(img->raster[y * img->width + x] ? "#" : " ");
        }
        printf("|\n");
    }
    printf("\\");
    for (int x = 0; x < img->width; ++x)
        printf("-");
    printf("/\n");
}

Image *img_extract_character(Image *img)
{
    // search an entry point
    int as_find = false;
    int xa = 0, ya = 0;
    for (int y = 0; y < img->height && !as_find; ++y)
    {
        for (int x = 0; x < img->width && !as_find; ++x)
        {
            if (img->raster[y * img->width + x])
            {
                as_find = true;
                ya = y;
                xa = x;
            }
        }
    }

    if (!as_find)
        return NULL;
    Vector *pixels = vec_create(10);
    _img_explore_character(img, pixels, xa, ya);
    Image *character = (Image *) malloc(sizeof(Image));
    character->width = img->width;
    character->height = img->height;
    character->trueWidth = img->width;
    character->trueHeight = img->height;
    character->character = img->character;
    character->x_root = img->x_root;
    character->y_root = img->y_root;
    character->font = img->font;
    int size = img->width * img->height;
    character->raster = calloc((size_t) (size), sizeof(char));
    for (int i = 0; i < pixels->size; ++i)
        character->raster[vec_get_int(pixels, i)] = true;
    img_crop_border(character, true);
    vec_free(pixels, true);
    return character;
}

void _img_explore_character(Image *img, Vector *pixels, int x, int y)
{
    int p_index = y * img->width + x;
    int p_index_lr = (y + 1) * img->width + x + 1;
    vec_add_int(pixels, p_index);
    if(pixels->size > 80000)
        return;
    img->raster[p_index] = false;
    if (x > 0 && img->raster[p_index - 1])
        _img_explore_character(img, pixels, x - 1, y);
    if (y > 0 && img->raster[p_index - img->width])
        _img_explore_character(img, pixels, x, y - 1);
    if (x < img->width - 1 && img->raster[p_index + 1])
        _img_explore_character(img, pixels, x + 1, y);
    if (y < img->height - 1 && img->raster[p_index + img->width])
        _img_explore_character(img, pixels, x, y + 1);
    if (x > 0 && y > 0 && img->raster[p_index - img->width - 1])
        _img_explore_character(img, pixels, x - 1, y - 1);
    if (x < img->width - 1 && y > 0 && img->raster[p_index - img->width + 1])
        _img_explore_character(img, pixels, x + 1, y - 1);
    if (x < img->width - 1 && y < img->height - 1 && img->raster[p_index_lr])
        _img_explore_character(img, pixels, x + 1, y + 1);
    if (x > 0 && y < img->height - 1 && img->raster[p_index + img->width - 1])
        _img_explore_character(img, pixels, x - 1, y + 1);
}

Image *img_normalize(Image *img, int size)
{
    Image *result = (Image *) malloc(sizeof(Image));
    result->width = size;
    result->height = size;
    result->trueWidth = img->width;
    result->trueHeight = img->height;
    result->x_root = img->x_root;
    result->y_root = img->y_root;
    result->character = img->character;
    result->font = img->font;
    result->raster = calloc((size_t) (size * size), sizeof(char));
    float x_factor = (float) img->width / size;
    float y_factor = (float) img->height / size;
    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            int xi = (int) ((float) x * x_factor);
            int yi = (int) ((float) y * y_factor);
            if (img->raster[yi * img->width + xi])
                result->raster[y * size + x] = true;
        }
    }
    return result;
}

void set_pixel(Image *img, int x, int y, char pixel)
{
    img->raster[img->width * y + x] = pixel;
}

char get_pixel(Image *image, int xo, int yo)
{
    return image->raster[yo * image->width + xo];
}