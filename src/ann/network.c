#include "network.h"

#include "../util/string.h"

#include <stdio.h>
#include <time.h>

Layer *lyr_create(int inputSize, int nbExample, int prevInput, int depth)
{
    Layer *lyr = (Layer *) malloc(sizeof(Layer));
    lyr->depth = depth;
    lyr->a = mat_create(inputSize, nbExample, NULL);
    lyr->z = mat_create(inputSize, nbExample, NULL);
    lyr->w = mat_create(inputSize, prevInput, NULL);
    return lyr;
}

void lyr_free(Layer *lyr)
{
    mat_free(lyr->a, true);
    mat_free(lyr->z, true);
    mat_free(lyr->w, true);
    free(lyr);
}

Network *net_create(int inputSize, int nbExample, char *name)
{
    Network *net = (Network *) malloc(sizeof(Network));
    net->name = name;
    net->layers = vec_create(2);
    vec_add(net->layers, lyr_create(inputSize, nbExample, 0, 0));
    return net;
}

void net_free(Network *net)
{
    for (int i = 0; i < net->layers->size; ++i)
        lyr_free((Layer *) vec_get(net->layers, i));
    vec_free(net->layers, false);
    free(net);
}

void net_randomizeLayer(Network *net, int depth)
{
    Layer *lyr = (Layer *) vec_get(net->layers, depth);
    srand(time(NULL));
    for (int i = 0; i < lyr->w->width * lyr->w->height; ++i)
    {
        lyr->w->mat[i] = (float) rand() / (float) (RAND_MAX);
    }
}

void net_loadLayerWeights(Network *net, int depth)
{
    Layer *lyr = (Layer *) vec_get(net->layers, depth);
    char *path = str_con(5, "res/network/", net->name, "_", depth, ".layer");
    FILE *file = fopen(path, "w");
    for (int i = 0; i < lyr->w->height * lyr->w->width; ++i)
    {
        fread(lyr->w->mat, sizeof(float), 1, file);
    }
    fclose(file);
}

void net_addLayer(Network *net, int nbUnit)
{
    Layer *fl = (Layer *) vec_get(net->layers, 0);
    int nbExample = fl->a->height;
    Layer *ll = (Layer *) vec_get(net->layers, net->layers->size - 1);
    int last = ll->w->width;
    Layer *lyr = lyr_create(nbUnit, nbExample, last, net->layers->size);
    vec_add(net->layers, lyr);
}

void net_loadInput(Network *net, float *input)
{
    Layer *fl = (Layer *) vec_get(net->layers, 0);
    free(fl->a->mat);
    fl->a->mat = input;
}

// ========== Forward ==========

void lyr_forward(Layer *previous, Layer *current)
{
    mat_free(current->a, true);
    mat_free(current->z, true);
    current->z = mat_multiply(previous->a, current->w);
    current->a = mat_cpy(current->z);
    mat_apply(mth_sigmoid_prime, current->a);
}

void net_forward(Network *net)
{
    for (int i = 1; i < net->layers->size; ++i)
        lyr_forward(vec_get(net->layers, i - 1), vec_get(net->layers, i));
}

// ========== Backward ==========

Vector *net_backward(Network *net, Matrix *y)
{
    Vector *delta = vec_create(net->layers->size - 1);
    Layer *ll = (Layer *) vec_get(net->layers, net->layers->size - 1);
    mat_substract_ip(y, ll->a);
    for (int i = 0; i < y->height * y->width; ++i)
        y->mat[i] *= -1;
    Matrix *z = mat_cpy(ll->z);
    mat_apply(mth_sigmoid_prime, z);
    Matrix *dlt = ary_multiply(y, z);
    mat_free(z, true);
    Layer *prev = (Layer *) vec_get(net->layers, net->layers->size - 2);
    Matrix *aT = mat_transpose(prev->a);
    vec_add(delta, mat_multiply(aT, dlt));
    mat_free(aT, true);
    for (int i = net->layers->size - 2; i > 0; --i)
    {
        Layer *cur = (Layer *) vec_get(net->layers, i);
        Matrix *wT = mat_transpose(cur->w);
        Matrix *yi = mat_multiply(dlt, wT);
        mat_free(wT, true);
        mat_free(dlt, true);
        z = mat_cpy(cur->z);
        mat_apply(mth_sigmoid_prime, z);
        dlt = ary_multiply(yi, z);
        mat_free(z, true);
        mat_free(yi, true);
        Layer *prev = (Layer *) vec_get(net->layers, i - 1);
        aT = mat_transpose(prev->a);
        vec_add(delta, mat_multiply(aT, dlt));
        mat_free(aT, true);
    }
    mat_free(dlt, true);
    return delta;
}

float net_cost(Network *net, Matrix *y)
{
    Layer *ll = (Layer *) vec_get(net->layers, net->layers->size - 1);
    float cost = 0;
    for (int i = 0; i < y->width * y->height; ++i)
    {
        float delta = y->mat[i] - ll->a->mat[i];
        cost += delta * delta;
    }
    return .5 * cost;
}


Vector *net_numGrad(Network *net, Matrix *y)
{
    float e = .001;
    Vector *grads = vec_create(net->layers->size - 1);
    for (int i = 1; i < net->layers->size; ++i)
    {
        Layer *lyr = (Layer *) vec_get(net->layers, i);
        int h = lyr->w->height;
        int w = lyr->w->width;
        Matrix *grad = mat_create(w, h, NULL);
        for (int iw = 0; iw < h * w; ++iw)
        {
            lyr->w->mat[iw] += e;
            net_forward(net);
            float lossB = net_cost(net, y);
            lyr->w->mat[iw] -= 2 * e;
            net_forward(net);
            float lossA = net_cost(net, y);
            lyr->w->mat[iw] += e;
            grad->mat[iw] = (lossB - lossA) / (2 * e);
        }
        vec_add(grads, grad);
    }
    net_forward(net);
    return grads;
}
