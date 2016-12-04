#include "io/image.h"
#include "processing/binarizator.h"
#include "interface/interface.h"
#include "defaults.h"
#include "util/string.h"
#include "util/image.h"
#include "processing/rotation/rotation.h"

#include <stdio.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>

Vector *getFontList()
{
    Vector *fonts = vec_create(2);
    FILE *file = fopen("res/ann/font.list", "r");
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
        char path[] = "res/ann/________________/?.char";
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
        char path[] = "res/ann/________________/?.char";
        for(int p = 0; p < 16; ++p)
            path[p + 4] = font[p];
        path[21] = 'A' + i;
        Image *img = img_read_bin(path);
        img->character = 'A' + i;
        img->font = nb;
        vec_add(alpha, img);
    }
    {
        char path[] = "res/ann/________________/,.char";
        for(int p = 0; p < 16; ++p)
            path[p + 4] = font[p];
        Image *img = img_read_bin(path);
        img->character = ',';
        img->font = nb;
        vec_add(alpha, img);
    }
    {
        char path[] = "res/ann/________________/..char";
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

/*
int main(int argc, char **argv)
{
    char learn = false;
    char showLine = false;
    char showChar = false;
    char showGui = false;
    if(argc <= 1)
    {
        printf("Try 'croORCodile --help' for more information.\n");
        return -1;
    }
    for(int i = 1; i < argc; ++i)
    {
        if(!strcmp(argv[i], "--learn") || !strcmp(argv[i], "-l"))
           learn = true;
        if(!strcmp(argv[i], "--show-gui") || !strcmp(argv[i], "-G"))
           showGui = true;
        if(!strcmp(argv[i], "--show-line") || !strcmp(argv[i], "-L"))
           showLine = true;
        if(!strcmp(argv[i], "--show-char") || !strcmp(argv[i], "-C"))
           showChar = true;
        if(!strcmp(argv[i], "--help") || !strcmp(argv[i], "-h"))
        {
            printf("\n"
                   "Usage:\n"
                   "    croORCodile [options] file\n"
                   "\n"
                   "General Options:\n"
                   "    -h, --help         "
                   "Show this help.\n"
                   "    -l, --learn        "
                   "Enter interactive learning mode.\n"
                   "    -L, --show-line    "
                   "Display lines in the output image.\n"
                   "    -C, --show-char    "
                   "Show the graphical user interface.\n"
                   "    -G, --show-gui     "
                   "Display characters in the output image.\n");
            return 0;
        }
    }

    if(showGui)
        gui_start();
    
    char *newFontName = NULL;
    if(learn)
    {
        newFontName = (char *) malloc(16 * sizeof(char));
        for(int i = 0; i < 16; ++i)
            newFontName[i] = '_';
        printf("                          ________________\n");
        printf("Enter the new font name : ");
        scanf("%16s", newFontName);
        char path[] = "res/ann/________________";
        for(int p = 0; p < 16; ++p)
            path[p + 4] = newFontName[p];
        mkdir(path, 0777);
    }

    GdkPixbuf *img = img_load(argv[argc - 1]);

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
        printf("Characters detected => %d\r", chars->size);
    }
    printf("Characters detected => %d\n", chars->size);

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
                char path[] = "res/ann/________________/?.char";
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
    float meanHspaceDiv = 0.;
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
                    meanHspace += cur->x_root - pre->x_root + pre->width;
                    ++meanHspaceDiv;
                }
            }
            vec_add(lines, line);
        }
    }
    meanHspace /= meanHspaceDiv;
    vec_free(chars, false);

    for(int l = 0; l < lines->size; ++l)
    {
        Vector *line = (Vector *) vec_get(lines, l);
        Image *fi = (Image *) vec_get(line, 0);
        Image *li = (Image *) vec_get(line, line->size - 1);
        int ymin = fi->y_root;
        int yminW = fi->y_root;
        int hmaxW = -1;
        for(int c = 0; c < line->size; ++c)
        {
            Image *cur = (Image *) vec_get(line, c);
            if(fi == NULL)
                fi = cur;
            if(cur->y_root < ymin)
                ymin = cur->y_root;
            if(cur->y_root < yminW)
                yminW = cur->y_root;
            if(cur->y_root - yminW + cur->height > hmaxW)
                hmaxW = cur->y_root - yminW + cur->height;
            printf("%c", cur->character);
            int w = cur->width;
            if(showChar)
                img_drawRect(img, cur->x_root, cur->y_root, w, cur->height,
                         255, 0, 0);
            if(c + 1 < line->size)
            {
                Image *nxt = (Image *) vec_get(line, c + 1);
                if(nxt->x_root - cur->x_root + cur->width > meanHspace * 1.1)
                    if(nxt->character != '.' && nxt->character != ',')
                    {
                        printf(" ");
                        w = cur->x_root + cur->width - fi->x_root;
                        //img_drawRect(img, fi->x_root - 1, yminW - 1, w + 2,
                        //             hmaxW + 2, 0, 0, 255);
                        yminW = 999999;
                        hmaxW = -1;
                        fi = NULL;
                    }
            }
        }
        if(showLine)
        {
            fi = (Image *) vec_get(line, 0);
            int hmax = -1;
            for(int c = 0; c < line->size; ++c)
            {
                Image *cur = (Image *) vec_get(line, c);
                if(cur->y_root - ymin + cur->height > hmax)
                    hmax = cur->y_root - ymin + cur->height;
            }
            int w = li->x_root + li->width - fi->x_root;
            img_drawRect(img, fi->x_root, ymin, w, hmax, 0, 255, 0);
        }
        printf("\n");
    }

    if(showLine || showChar)
        img_save_buf(img, str_con(2, argv[argc - 1], "_rect.bmp"));

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
}
*/

int main(int argc, char**argv)
{
    Image *img = img_create(img_load(TEST_SET_IMAGE_ROTATION));
    Image *cropped = img_crop_border(img, false);
    Image *rotated = rotate_auto_image(cropped);
    img_save(rotated, "IMG-ROT-ROTATED.bmp");
}