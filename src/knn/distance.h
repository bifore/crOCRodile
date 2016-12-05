#ifndef KNN_DISTANCE_H_
#define KNN_DISTANCE_H_

#include "../util/image.h"
#include "../util/vector.h"

float euclidean(Image *a, Image *b);

void writeDistance(char *path, Vector *img_vec);
void readDistance(char *path, int font);

Image *knn(int k, Image *img, float (*dist) (Image *, Image *));

void addImage(Image *img);
Vector *dist_getVect();

#endif /* KNN_DISTANCE_H_ */
