#include "io/image.h"
#include "defaults.h"


int main()
{
    Image *img_binarized = img_load_IMAGE(TEST_SET_FOLDER TEST_SET_IMAGE_FULLTEST_NOSUFFIX ".jpg");
    img_crop_border(img_binarized, true);
    img_save_IMAGE(
            img_binarized,
            TEST_SET_FOLDER TEST_SET_OUT_FOLDER TEST_SET_IMAGE_FULLTEST_NOSUFFIX ".jpg",
            img_binarized->width,
            img_binarized->height
    );
    return 0;
}
