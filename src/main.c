#include "io/image.h"

#include "defaults.h"


int main()
{
    GdkPixbuf *img = img_load("Lorem-Droid-Mono-left.bmp");
    Canny_filter *cf = canny(img);
    guchar *data = (guchar *) malloc(sizeof(guchar) * cf->w * cf->h * 3);
    int a = 0;
    for(int i = 0; i < cf->w * cf->h; ++i)
        for(int u = 0; u < 3; ++u)
            if(cf->data[i])
                data[++a] = 255;
            else
                data[++a] = 0;
    img_save(data, "normal_text_edges.png", cf->w, cf->h);
    free(data);

    Image * original = img_create(img);
    Image * image = img_crop_border(original, false);

    Vector * chars = vec_create(10);
    Image * character = img_extract_character(image);
    int v = 0;
    while(character != NULL)
    {
        if(character->width < 7 || character->height < 10)
            img_free(character);
        else
        {
            /*img_print(character);
            printf("%d", v++);
            char x;
            while(x = getchar())
            {   
                if(x == '\n')
                    break;
            }*/
            vec_add(chars, (void *) character);
        }
        character = img_extract_character(image);
    }
    printf("character number -> %i\n", chars->size);

    Vector *alpha = vec_create(26);
    for(int i = 0; i < 26; ++i)
    {
        char path[] = "res/X.char";
        path[4] = 'a' + i;
        vec_add(alpha, img_read_bin(path));
        //img_print(vec_get(alpha, alpha->size - 1));
    }

    for(int i = 0; i < chars->size; ++i)
    {
        Image *img = (Image *) vec_get(chars, i);
        Image *norm = img_normalize(img, 20);
        /*printf("%d\n", i);
        if(i == 12)
        {
            img_print(norm);
            img_save_bin(norm, "res/i.char");
        }
        if(i == 1)
        {
            img_print(norm);
            img_save_bin(norm, "res/l.char");
        }*/
        int min = 20 * 20 + 1;
        int min_pos = -1;
        for(int c = 0; c < 26; ++c)
        {
            Image *alph = (Image *) vec_get(alpha, c);
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
                min_pos = c;
            }
        }
        printf("%d\n", i);
        img_print(norm);
        char x;
        while(x = getchar())
            if(x == '\n')
                break;
        printf("\n=> %c\n\n", 'a' + min_pos);
        img_free(norm);
        img_free(img);
    }
    //Image *imgg = img_read_bin("res/a.char");
    //img_print(imgg);
    vec_free(chars, false);


    /*Image *ch = (Image *) vec_get(selected, 0);
    Network *net = net_create(400, 1, "realtest");
    net_addLayer(net, 4);
    net_randomizeLayer(net, 1);
    //for(int i = 0; i < 200; ++i)
    //    ((Layer *) vec_get(net->layers, 1))->w->mat[i] = 2;
    net_addLayer(net, 1);
    net_randomizeLayer(net, 2);
    //for(int i = 0; i < 200; ++i)
    //    ((Layer *) vec_get(net->layers, 2))->w->mat[i] = 2;
    float *input = (float *) malloc(400 * sizeof(float));
    for(int i = 0; i < 400; ++i)
    {
        if(ch->raster[i])
        {
            input[i] = .75;
            printf("0");
        }
        else
        {
            input[i] = .25;
            printf("1");
        }
        if(i % ch->width == ch->width - 1)
            printf("\n");
    }
    net_loadInput(net, input);
    for(int i = 0; i < 1000; ++i)
    {
        net_forward(net);
        printf("a => %f\n", ((Layer *) vec_get(net->layers, 2))->a->mat[0]);
        Matrix *y = mat_create(1, 1, NULL);
        y->mat[0] = 1.;
        Vector *delta = net_backward(net, y);
        Matrix *a = (Matrix *) vec_get(delta, 0);
        printf("\t\t\t\td => %f\n", a->mat[0]);
        for(int x = 0; x < a->height * a->width; ++x)
            ((Layer *) vec_get(net->layers, 2))->w->mat[x] -= a->mat[x] * 3.0;
        Matrix *b = (Matrix *) vec_get(delta, 1);
        //for(int x = 0; x < b->height * 4; ++x)
        //    ((Layer *) vec_get(net->layers, 1))->w->mat[x] -= b->mat[x] * 3.0;
    }

    ch = (Image *) vec_get(selected, 1);
    input = (float *) malloc(400 * sizeof(float));
    for(int i = 0; i < 400; ++i)
    {
        if(ch->raster[i])
        {
            input[i] = .75;
            printf("0");
        }
        else
        {
            input[i] = .25;
            printf("1");
        }
        if(i % ch->width == ch->width - 1)
            printf("\n");
    }
    net_loadInput(net, input);

    net_forward(net);
    printf("r => %f\n", ((Layer *) vec_get(net->layers, 2))->a->mat[0]);

    ch = (Image *) vec_get(selected, 2);
    input = (float *) malloc(400 * sizeof(float));
    for(int i = 0; i < 400; ++i)
    {
        if(ch->raster[i])
        {
            input[i] = .75;
            printf("0");
        }
        else
        {
            input[i] = .25;
            printf("1");
        }
        if(i % ch->width == ch->width - 1)
            printf("\n");
    }
    net_loadInput(net, input);

    net_forward(net);
    printf("r => %f\n", ((Layer *) vec_get(net->layers, 2))->a->mat[0]);

    img_free(original);
    img_free(image);

    g_object_unref(img);
    canny_free(cf);*/

    /*Network *net = net_create(1, 2, "test");
    net_addLayer(net, 3);
    net_addLayer(net, 1);
    float *input = (float *) malloc(sizeof(float) * 2);
    input[0] = .5;
    input[1] = .8;
    net_loadInput(net, input);
    ((Layer *) vec_get(net->layers, 1))->w->mat[0] = 2;
        printf("a => %f\n", ((Layer *) vec_get(net->layers, 2))->a->mat[0]);
    ((Layer *) vec_get(net->layers, 1))->w->mat[1] = 2;
    ((Layer *) vec_get(net->layers, 1))->w->mat[2] = 2;
    ((Layer *) vec_get(net->layers, 2))->w->mat[0] = 2;
    ((Layer *) vec_get(net->layers, 2))->w->mat[1] = 2;
    ((Layer *) vec_get(net->layers, 2))->w->mat[2] = 2;
    ((Layer *) vec_get(net->layers, 2))->w->mat[3] = 2;
    ((Layer *) vec_get(net->layers, 2))->w->mat[4] = 2;
    ((Layer *) vec_get(net->layers, 2))->w->mat[5] = 2;
    for(int i = 0; i < 100; ++i)
    {
        net_forward(net);
        printf("a => %f\n", ((Layer *) vec_get(net->layers, 2))->a->mat[0]);
        printf("a => %f\n", ((Layer *) vec_get(net->layers, 2))->a->mat[1]);

        Matrix *y = mat_create(1, 2, NULL);
        y->mat[0] = .22;
        y->mat[1] = .11;
        y->mat[1] = .11;
        y->mat[1] = .05;

        Vector *delta = net_backward(net, y);
        Matrix *a = (Matrix *) vec_get(delta, 0);
        Matrix *b = (Matrix *) vec_get(delta, 1);
        printf("\t\t\t\td => %f\n", a->mat[0]);
        ((Layer *) vec_get(net->layers, 2))->w->mat[0] -= a->mat[0] * 3.;
        ((Layer *) vec_get(net->layers, 2))->w->mat[1] -= a->mat[1] * 3.;
        ((Layer *) vec_get(net->layers, 2))->w->mat[2] -= a->mat[2] * 3.;
        ((Layer *) vec_get(net->layers, 1))->w->mat[0] -= b->mat[0] * 3.;
        ((Layer *) vec_get(net->layers, 1))->w->mat[1] -= b->mat[1] * 3.;
        ((Layer *) vec_get(net->layers, 1))->w->mat[2] -= b->mat[2] * 3.;
        ((Layer *) vec_get(net->layers, 1))->w->mat[3] -= b->mat[3] * 3.;
        ((Layer *) vec_get(net->layers, 1))->w->mat[4] -= b->mat[4] * 3.;
        ((Layer *) vec_get(net->layers, 1))->w->mat[5] -= b->mat[5] * 3.;
    }


    net_free(net);*/

    /*Network *net = net_create(2, 3, "test");
      net_addLayer(net, 3);
      net_randomizeLayer(net, 1);
      net_addLayer(net, 1);
      net_randomizeLayer(net, 2);

      float *input = (float *) malloc(2 * 3 * sizeof(float));
      input[0] = 3. / 15.;
      input[1] = 5. / 15.;
      input[2] = 5. / 15.;
      input[3] = 1. / 15.;
      input[4] = 10./ 15.;
      input[5] = 2. / 15.;
      net_loadInput(net, input);
    //Layer *fl = (Layer *) vec_get(net->layers, 0);
    //printf("%f - %f - %f\n", fl->a->mat[0], fl->a->mat[1], fl->a->mat[2]);

    Matrix *y = mat_create(1, 3, NULL);
    y->mat[0] = 75. / 100.;
    y->mat[1] = 82. / 100.;
    y->mat[2] = 93. / 100.;

    float scalar = 3.0;
    net_forward(net);
    Layer *ll = (Layer *) vec_get(net->layers, net->layers->size - 1);
    printf("%f - %f - %f\n", ll->a->mat[0], ll->a->mat[1], ll->a->mat[2]);

    for(int a = 0; a < 100; ++a)
    {
    //Vector *delta = net_backward(net, y);
    Vector *delta = net_numGrad(net, y);
    for(int i = 0; i < delta->size; ++i)
    {
    mat_scalar((Matrix *) vec_get(delta, i), scalar);
    //int li = net->layers->size - 1 - i;
    int li = i + 1;
    Layer *l = (Layer *) vec_get(net->layers, li);
    mat_substract_ip(l->w, (Matrix *) vec_get(delta, i));
    mat_free((Matrix *) vec_get(delta, i), true);
    }
    vec_free(delta, false);
    net_forward(net);
    printf("%f - %f - %f\n", ll->a->mat[0], ll->a->mat[1], ll->a->mat[2]);
    }

    mat_free(y, true);
    net_free(net);*/

}
