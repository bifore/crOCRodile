/*
** Created by tristan9 on 10/30/16.
*/

#ifndef PROCESSING_BINARIZATOR_H_
#define PROCESSING_BINARIZATOR_H_

#include "../util/image.h"

Image *binarize(GdkPixbuf *file, float percent_tolerance);

#endif /* PROCESSING_BINARIZATOR_H_ */
