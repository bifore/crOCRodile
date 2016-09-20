#ifndef ANN_NETWORK_H_
#define ANN_NETWORK_H_

#include <stdlib.h>
#include <stdbool.h>

#include "../utils/vector.h"

typedef struct {
    float * input;
    int inputSize;
    Vector * layers;

} Network;

typedef struct {
    float * weights;
    int inputSize;
    int unit_nb;
} Layer;

Network * net_create(int intputSize);
void net_free(Network * net);

Layer * lyr_create(int inputSize, int unit_nb);
void lyr_free(Layer * lyr);

Network * net_addLayer(Network * net, int unit_nb);

#endif /* !ANN_NETWORK_H_ */
