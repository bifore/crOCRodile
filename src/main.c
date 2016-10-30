#include "io/image.h"
#include "defaults.h"

#define PI 3.14159265358979323846

int main()
{
    Image *img_binarized = img_load_IMAGE(TEST_SET_FOLDER TEST_SET_IMAGE_ROTATION_NOSUFFIX ".jpg");
    img_crop_border(img_binarized, true);

    img_save_IMAGE(
            img_binarized,
            TEST_SET_FOLDER TEST_SET_OUT_FOLDER TEST_SET_IMAGE_ROTATION_NOSUFFIX "_norotate.jpg",
            img_binarized->width,
            img_binarized->height
    );

    Image *rotated = img_rotate(img_binarized, PI * 45. / 180.);
    img_save_IMAGE(
            rotated,
            TEST_SET_FOLDER TEST_SET_OUT_FOLDER TEST_SET_IMAGE_ROTATION_NOSUFFIX "_rotated.jpg",
            rotated->width,
            rotated->height
    );

    img_free(img_binarized);
    return 0;
}
