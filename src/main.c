#include "io/image.h"

#include "defaults.h"


int main()
{
    GdkPixbuf *img = img_load(TESTSETSIMPLE IMAGE_LOREM ".bmp");
    Image *original = img_create(img);
    //img_crop_border(original, true);

    img_save_IMAGE(
            original,
            TESTSETSIMPLE IMAGE_LOREM "_binarize.bmp",
            original->width,
            original->height
    );
}
