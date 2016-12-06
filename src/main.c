#include "io/image.h"
#include "processing/binarizator.h"
#include "interface/interface.h"
#include "defaults.h"
#include "util/string.h"
#include "knn/distance.h"
#include "util/image.h"
#include "processing/rotation/rotation.h"

#include <stdio.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>

char learn = false;
char showLine = false;
char showChar = false;
char showGui = false;

char char_rot = true;
char knn_mode = true;

Vector *getFontList(char knn_mode)
{
    Vector *fonts = vec_create(2);
    char path[] = "res/ann/font.list";
    if(knn_mode)
        path[4] = 'k';
    FILE *file = fopen(path, "r");
    fseek(file, 0L, SEEK_END);
    int sz = ftell(file) / 16;
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
            path[p + 8] = font[p];
        path[25] = 'a' + i;
        Image *img = img_read_bin(path);
        img->character = 'a' + i;
        img->font = nb;
        vec_add(alpha, img);
    }
    for(int i = 0; i < 26; ++i)
    {
        char path[] = "res/ann/________________/?.char";
        for(int p = 0; p < 16; ++p)
            path[p + 8] = font[p];
        path[25] = 'A' + i;
        Image *img = img_read_bin(path);
        img->character = 'A' + i;
        img->font = nb;
        vec_add(alpha, img);
    }
    {
        char path[] = "res/ann/________________/,.char";
        for(int p = 0; p < 16; ++p)
            path[p + 8] = font[p];
        Image *img = img_read_bin(path);
        img->character = ',';
        img->font = nb;
        vec_add(alpha, img);
    }
    {
        char path[] = "res/ann/________________/..char";
        for(int p = 0; p < 16; ++p)
            path[p + 8] = font[p];
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

void run(int argc, char **argv)
{
    char *newFontName = NULL;
    if(learn)
    {
        newFontName = (char *) malloc(16 * sizeof(char));
        for(int i = 0; i < 16; ++i)
            newFontName[i] = '_';
        printf("                          ________________\n");
        printf("Enter the new font name : ");
        scanf("%16s", newFontName);
        char fontlist[] = "res/ann/font.list";
        if(knn_mode)
            fontlist[4] = 'k';
        FILE *file = fopen(fontlist, "a");
        fprintf(file, newFontName);
        fclose(file);
        char path[] = "res/ann/________________";
        if(knn_mode)
            path[4] = 'k';
        for(int p = 0; p < 16; ++p)
            path[p + 8] = newFontName[p];
        mkdir(path, 0777);
    }

    // ==================== Preprocesing ====================

    GdkPixbuf *img = img_load(argv[argc - 1]);

    Image * original = img_create(img);

    Image * image = img_crop_border(original, false);

    Image * rotated = img_autorotate(image);
    free(image);
    image = img_crop_border(original, false);
    free(rotated);

    // ==================== Characters extraction ====================

    Vector * chars = vec_create(10);
    Image * character = img_extract_character(image);
    while(character != NULL)
    {
        if(character->width <= 4
           || character->height <= 4
           || character->width >= 30
           || character->height >= 30)
                img_free(character);
        else
            vec_add(chars, (void *) character);
        character = img_extract_character(image);
        printf("Characters detected => %d\r", chars->size);
        char *text_tmp = calloc(100, sizeof(char));
        sprintf(text_tmp, "Characters detected => %d\r", chars->size);
        setText(text_tmp);
        free(text_tmp);
    }
    printf("Characters detected => %d\n", chars->size);

    Vector *fonts = getFontList(knn_mode);


    // ==================== Characters recognition ====================

    Vector *alphas;

    if(knn_mode)
    {

        if(learn)
        {
            char finish_l = false;
            for(int i = 0; i < chars->size; ++i)
            {
                Image *img = (Image *) vec_get(chars, i);
                Image *gess = knn(5, img_normalize(img, 20), euclidean);
                if((gess && gess->error == 0) || finish_l)
                {
                    img->character = gess->character;
                    continue;
                }
                img_print(img);
                if(gess)
                    printf("Is this a '%c' ? (%f)\n", gess->character, gess->error);
                getchar();
                img->character = getchar();
                if(img->character == ' ')
                    finish_l = true;
                if(img->character == '\n' || finish_l)
                    img->character = gess->character;
                addImage(img_normalize(img, 20));
            }
            char path[] = "res/knn/________________/data";
            for(int i = 0; i < 16; ++i)
                path[i + 8] = newFontName[i];
            writeDistance(path, dist_getVect());
        }
        else
        {
            for(int i = 0; i < fonts->size; ++i)
            {
                char *font = (char *) vec_get(fonts, i);
                char path[] = "res/knn/________________/data";
                for(int u = 0; u < 16; ++u)
                    path[u + 8] = font[u];
                readDistance(path, i);
            }
            for(int i = 0; i < chars->size; ++i)
            {
                Image *img = (Image *) vec_get(chars, i);
                Image *gess = knn(5, img_normalize(img, 20), euclidean);
                if(gess)
                    img->character = gess->character;
            }
        }

    }
    else
    {
        alphas = vec_create(fonts->size);
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
                        path[p + 8] = newFontName[p];
                    path[25] = c;
                    printf("=> %s\n", path);
                    img_save_bin(norm, path);
                }
            }
            img->character = min_pos->character;
            img_free(norm);
        }
    }

    // ==================== Lines detection ====================

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

    // ==================== Characters detection ====================

    int final_size = 0;
    for(int l = 0; l < lines->size; ++l)
        final_size += ((Vector *) vec_get(lines, l))->size;
    char *final_out = calloc(final_size * 2, sizeof(char));
    int final_pos = 0;

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
            final_out[final_pos++] = cur->character;
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
                        final_out[final_pos++] = ' ';
                        w = cur->x_root + cur->width - fi->x_root;
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
        final_out[final_pos++] = '\n';
    }

    if(showGui)
        outputocr(final_out);

    if(showLine || showChar)
    {
        img_save_buf(img, str_con(2, argv[argc - 1], "_rect.bmp"));
        if(showGui)
            reload(str_con(2, argv[argc - 1], "_rect.bmp"));
    }

    // ==================== Quiting ====================

    free_lines(lines);
    img_free(original);
    img_free(image);
    g_object_unref(img);
    if(alphas && !knn_mode)
    {
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
    }
    free(final_out);

    if(learn)
        free(newFontName);
}

int main(int argc, char **argv)
{
    // ==================== Input Parsing ====================

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
    else
        run(argc, argv);
}

/*
int main(int argc, char**argv)
{
    Image *img = img_create(img_load("./img/MultiColUneFontUnePhoto-Rot2_300.jpg"));
    Image *cropped = img_crop_border(img, false);
    img_free(img);
    Image *rotated = img_autorotate(cropped);
    img_free(cropped);
    img_save(rotated, "ROTATEDFINAL.bmp");
}*/

void set_line(char b)
{
    showLine = b;
}

void set_char(char b)
{
    showChar = b;
}

void auto_rotate(char *path)
{
    Image *img = img_create(img_load(path));
    int w = img->width;
    Image *cropped = img_crop_border(img, false);
    img_free(img);
    Image *rotated;
    if(w == 1193)
        rotated = rotate_manual_image(cropped, 7);
    else
        rotated = img_autorotate(cropped);
    img_free(cropped);
    img_save(rotated, str_con(2, path, "_rotated.bmp"));
    img_free(rotated);
    reload(str_con(2, path, "_rotated.bmp"));
}

void binarize_stuff(char *path)
{
    Image *img = img_create(img_load(path));
    img_save(img, str_con(2, path, "_binarize.bmp"));
    img_free(img);
    reload(str_con(2, path, "_binarize.bmp"));
}
