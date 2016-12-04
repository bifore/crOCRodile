//
// Created by tristan9 on 12/4/16.
//

#ifndef CROCRODILE_HISTOGRAM_H
#define CROCRODILE_HISTOGRAM_H

void grayHistogram(Image *surface, int *array);
void vHistogram(Image *surface, int *array);
void vHistogramBounds(Image *surface, int *array, SDL_Rect *bounds);
void hHistogram(Image *surface, int *array);
void hHistogramBounds(Image *surface, int *array, SDL_Rect *bounds);
void rHistogram(Image *surface, int *array, int angle);
int **hrlsa(int **matrix, int width, int height);
int **vrlsa(int **matrix, int width, int height);

#endif //CROCRODILE_HISTOGRAM_H
