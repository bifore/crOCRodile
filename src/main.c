#include "io/image.h"
#include "processing/canny.h"

#include "ann/network.h"

int main()
{
    /*GdkPixbuf *img = img_load("normal_text.bmp");
    Canny_filter *cf = canny(img);
    guchar *data = (guchar *) malloc(sizeof(guchar) * cf->w * cf->h * 3);
    int a = 0;
    for(int i = 0; i < cf->w * cf->h; ++i)
        for(int u = 0; u < 3; ++u)
            if(cf->data[i])
                data[++a] = 255;
            else
                data[++a] = 0;
    img_save(data, "normal_text_edges.bmp", cf->w, cf->h);
    free(data);
    g_object_unref(img);
    canny_free(cf);*/

    Network *net = net_create(4, 1, "test_1");
    net_addLayer(net, 10);
    net_randomizeLayer(net, 1);
    net_addLayer(net, 1);
    net_randomizeLayer(net, 2);

    float *input = (float *) malloc(4 * 1 * sizeof(float));
    input[0] = .1;
    input[1] = .2;
    input[2] = .4;
    input[3] = .6;
    net_loadInput(net, input);

    net_forward(net);

    Matrix *y = mat_create(1, 1, NULL);
    y->mat[0] = 1.0;
    Vector *delta = net_backward(net, y);

    mat_free(y, true);
    for(int i = 0; i < delta->size; ++i)
        mat_free((Matrix *) vec_get(delta, i), true);
    vec_free(delta, false);

    net_free(net);
}
