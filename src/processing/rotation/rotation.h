//
// Created by tristan9 on 12/4/16.
//

#ifndef CROCRODILE_ROTATION_H
#define CROCRODILE_ROTATION_H
#define PI 3.14159265358979323846

#include <math.h>
#include "../../util/image.h"

int max_array_pos(float *a, int nb);

float get_variance(int *array);

int find_rotation_angle(Image *image);

int max_bounds(SDL_Rect *histo, int length);

Vector *detect_blocks(Image *surface);

SDL_Rect *get_lines(Image *surface, int *nb, SDL_Rect *bounds);

void draw_blocks(Image *surface, Vector *vect);

void reset_array(int *array, int length);

SDL_Rect *get_chars(Image *surface, SDL_Rect *lines, int length, int *nb);

void trim_chars(Image *surface, SDL_Rect *chars, int length);

double img_get_orientation_factor_radians(Image *image);

Image *img_autorotate(Image *img);

Image *img_rotate(Image *img, double radians);

#endif //CROCRODILE_ROTATION_H
