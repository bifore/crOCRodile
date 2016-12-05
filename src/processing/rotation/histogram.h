//
// Created by tristan9 on 12/4/16.
//

#ifndef CROCRODILE_HISTOGRAM_H
#define CROCRODILE_HISTOGRAM_H

#include "../../util/maths.h"

void grayHistogram(Image *surface, int *array);
void vHistogram(Image *surface, int *array);
void vHistogramBounds(Image *surface, int *array, SDL_Rect *bounds);
void hHistogram(Image *surface, int *array);
void hHistogramBounds(Image *surface, int *array, SDL_Rect *bounds);
void rHistogram(Image *surface, int *array, int angle);
Matrix *hrlsa(Matrix *matrix);
Matrix *vrlsa(Matrix *matrix);

#endif //CROCRODILE_HISTOGRAM_H
