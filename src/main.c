#include "io/image.h"
#include "processing/canny.h"
#include "util/image.h"

#include "ann/network.h"
#include "defaults.h"


int main()
{
    GdkPixbuf *img = img_load(TESTSETSIMPLE IMAGE_LOREM ".bmp");
    Image * original = img_create(img);
    //img_crop_border(original, true);

    img_save_IMAGE(
            original,
            TESTSETSIMPLE IMAGE_LOREM "_binarize.bmp",
            original->width,
            original->height
    );
}
