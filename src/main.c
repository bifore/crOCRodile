#include "io/image.h"
#include "defaults.h"

<<<<<<< Updated upstream
#define PI 3.14159265358979323846

int main()
{
    Image *img_binarized = img_load_IMAGE(TEST_SET_FOLDER TEST_SET_IMAGE_ROTATION_NOSUFFIX ".jpg");
    img_crop_border(img_binarized, true);

    img_save_IMAGE(
            img_binarized,
            TEST_SET_FOLDER TEST_SET_OUT_FOLDER TEST_SET_IMAGE_ROTATION_NOSUFFIX "_norotate.jpg",
            img_binarized->width,
            img_binarized->height
    );

    Image *rotated = img_rotate(img_binarized, PI * 45. / 180.);
    img_save_IMAGE(
            rotated,
            TEST_SET_FOLDER TEST_SET_OUT_FOLDER TEST_SET_IMAGE_ROTATION_NOSUFFIX "_rotated.jpg",
            rotated->width,
            rotated->height
    );

    img_free(img_binarized);
    return 0;
=======
#include <stdio.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>

Vector *getFontList()
{
    Vector *fonts = vec_create(2);
    FILE *file = fopen("res/font.list", "r");
    fseek(file, 0L, SEEK_END);
    int sz = (ftell(file) - 1) / 16;
    rewind(file);
    for(int i = 0; i < sz; ++i)
    {
        char *name = (char *) calloc(17, sizeof(char));
        fread(name, sizeof(char), 16 * sizeof(char), file);
        vec_add(fonts, name);
    }
    return fonts;
}

Vector *loadAlphabet(char *font, int nb)
{
    Vector *alpha = vec_create(54);
    for(int i = 0; i < 26; ++i)
    {
        char path[] = "res/________________/?.char";
        for(int p = 0; p < 16; ++p)
            path[p + 4] = font[p];
        path[21] = 'a' + i;
        Image *img = img_read_bin(path);
        img->character = 'a' + i;
        img->font = nb;
        vec_add(alpha, img);
    }
    for(int i = 0; i < 26; ++i)
    {
        char path[] = "res/________________/?.char";
        for(int p = 0; p < 16; ++p)
            path[p + 4] = font[p];
        path[21] = 'A' + i;
        Image *img = img_read_bin(path);
        img->character = 'A' + i;
        img->font = nb;
        vec_add(alpha, img);
    }
    {
        char path[] = "res/________________/comma.char";
        for(int p = 0; p < 16; ++p)
            path[p + 4] = font[p];
        Image *img = img_read_bin(path);
        img->character = ',';
        img->font = nb;
        vec_add(alpha, img);
    }
    {
        char path[] = "res/________________/point.char";
        for(int p = 0; p < 16; ++p)
            path[p + 4] = font[p];
        Image *img = img_read_bin(path);
        img->character = '.';
        img->font = nb;
        vec_add(alpha, img);
    }
    return alpha;
}

void free_lines(Vector *lines)
{
    for(int i = 0; i < lines->size; ++i)
    {
        Vector *line = (Vector *) vec_get(lines, i);
        for(int u = 0; u < line->size; ++u)
            img_free((Image *) vec_get(line, u));
        vec_free(line, false);
    }
    vec_free(lines, false);
}

int main(int argc, char **argv)
{
    char learn = argc == 2 && !strcmp(argv[1], "learn");
    
    char *newFontName = NULL;
    if(learn)
    {
        newFontName = (char *) malloc(16 * sizeof(char));
        for(int i = 0; i < 16; ++i)
            newFontName[i] = '_';
        printf("                          ________________\n");
        printf("Enter the new font name : ");
        scanf("%16s", newFontName);
        char path[] = "res/________________";
        for(int p = 0; p < 16; ++p)
            path[p + 4] = newFontName[p];
        mkdir(path, 0777);
    }

    GdkPixbuf *img = img_load("./Lorem-Droid-Serif-Left.bmp");

    Image * original = img_create(img);
    Image * image = img_crop_border(original, false);

    Vector * chars = vec_create(10);
    Image * character = img_extract_character(image);
    while(character != NULL)
    {
        if(character->width <= 4 || character->height <= 4)
            img_free(character);
        else
            vec_add(chars, (void *) character);
        character = img_extract_character(image);
        printf("%d\n", chars->size);
    }
    printf("character number -> %i\n", chars->size);

    Vector *fonts = getFontList();

    Vector *alphas = vec_create(fonts->size);
    for(int i = 0; i < fonts->size; ++i)
        vec_add(alphas, loadAlphabet(vec_get(fonts, i), i));

    for(int i = 0; i < chars->size; ++i)
    {
        Image *img = (Image *) vec_get(chars, i);
        Image *norm = img_normalize(img, 20);
        int min = 20 * 20 + 1;
        Image * min_pos = NULL;
        for(int c = 0; c < alphas->size; ++c)
        {
            Vector *alpha = (Vector *) vec_get(alphas, c);
            for(int cc = 0; cc < alpha->size; ++cc)
            {
                Image *alph = (Image *) vec_get(alpha, cc);
                int diff = 0;
                int sum = 0;
                for(int v = 0; v < 20 * 20; ++v)
                {
                    diff += alph->raster[v] != norm->raster[v];
                    sum += alph->raster[v];
                }
                if(sum != 0 && diff < min)
                {
                    min = diff;
                    min_pos = alph;
                }
            }
        }
        if(learn && min != 0)
        {
            img_print(img);
            getchar();
            char c = getchar();
            if(c != '\n')
            {
                char path[] = "res/________________/?.char";
                for(int p = 0; p < 16; ++p)
                    path[p + 4] = newFontName[p];
                path[21] = c;
                printf("=> %s\n", path);
                img_save_bin(norm, path);
            }
        }
        img->character = min_pos->character;
        img_free(norm);
    }

    Vector *lines = vec_create(2);
    float meanHspace = 0.;
    for(int i = 0; i < chars->size; ++i)
    {
        Image *c = (Image *) vec_get(chars, i);
        if(c != NULL)
        {
            Vector *line = vec_create(2);
            vec_add(line, c);
            chars->data[i] = NULL;
            int ch = c->height * .95;
            for(int ii = 0; ii < chars->size; ++ii)
            {
                Image *cur = (Image *) vec_get(chars, ii);
                if(ii != i && cur != NULL)
                    if(abs(cur->y_root - c->y_root) < ch)
                    {
                        vec_add(line, cur);
                        chars->data[ii] = NULL;
                    }
            }
            for(int ii = 0; ii < line->size ; ++ii)
            {
                int pos = ii;
                int min = original->width;
                for(int iii = ii; iii < line->size; ++iii)
                {
                    Image *cur = (Image *) vec_get(line, iii);
                    if(min > cur->x_root)
                    {
                        min = cur->x_root;
                        pos = iii;
                    }
                }
                Image *tmp = line->data[ii];
                line->data[ii] = line->data[pos];
                line->data[pos] = tmp;
                if(ii != 0)
                {
                    Image *pre = (Image *) vec_get(line, ii - 1);
                    Image *cur = (Image *) vec_get(line, ii);
                    meanHspace += cur->x_root - pre->x_root;
                }
            }
            vec_add(lines, line);
        }
    }
    meanHspace /= (float) chars->size;
    vec_free(chars, false);

    for(int l = 0; l < lines->size; ++l)
    {
        Vector *line = (Vector *) vec_get(lines, l);
        for(int c = 0; c < line->size; ++c)
        {
            Image *cur = (Image *) vec_get(line, c);
            printf("%c", cur->character);
            if(c + 1 < line->size)
            {
                Image *nxt = (Image *) vec_get(line, c + 1);
                if(nxt->x_root - cur->x_root > meanHspace)
                    if(nxt->character != '.' && nxt->character != ',')
                        printf(" ");
            }
        }
        printf("\n");
    }

    free_lines(lines);
    img_free(original);
    img_free(image);
    g_object_unref(img);
    for(int i = 0; i < alphas->size; ++i)
    {
        Vector *alpha = (Vector *) vec_get(alphas, i);
        for(int u = 0; u < alpha->size; ++u)
        {
             img_free((Image *) vec_get(alpha, u));
        }
        vec_free(alpha, false);
    }
    vec_free(alphas, false);

    if(learn)
        free(newFontName);
>>>>>>> Stashed changes
}
