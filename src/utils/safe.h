#ifndef UTILS_SAFE_H_
#define UTILS_SAFE_H_

#include <stdio.h>

void initSafe();
void * smalloc(int size);
void sfree(void * ptr);

#endif /* !UTILS_SAFE_H_ */
