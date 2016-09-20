#include "network.h"

Network * net_create(int inputSize)
{
    Network * net = (Network *) malloc(sizeof(Network));
    net->inputSize = inputSize;
    net->input = (float *) malloc(sizeof(float) * inputSize);
    net->layers = vec_create(2);
    return net;
}

void net_free(Network * net)
{
    for(int i = 0; i < net->layers->size; ++i)
    {
        lyr_free((Layer *) vec_get(net->layers, i));
    }
    vec_free(net->layers, false);
    free(net->input);
    free(net);
}

Layer * lyr_create(int inputSize, int unit_nb)
{
    Layer * lyr = (Layer *) malloc(sizeof(Layer));
    lyr->inputSize = inputSize;
    lyr->unit_nb = unit_nb;
    lyr->weights = (float *) malloc(sizeof(float) * inputSize * unit_nb);
    return lyr;
}

void lyr_free(Layer * lyr)
{
    free(lyr->weights);
    free(lyr);
}

Network * net_addLayer(Network * net, int unit_nb)
{
    int last = net->inputSize;
    if(net->layers->size != 0)
    {
        Layer * ll = (Layer *) vec_get(net->layers, net->layers->size - 1);
        last = ll->unit_nb;
    }
    Layer * lyr = lyr_create(last, unit_nb);
    vec_add(net->layers, lyr);
    return net;
}
