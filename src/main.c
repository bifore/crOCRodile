#include "main.h"
#include "io/bmpReader.h"
#include "utils/image.h"
#include "utils/vector.h"

//test
#include "ann/network.h"

int main()
{
    BITMAP * bmp = loadBitmap("res/normal_text_small.bmp");
    Image * original = img_create(bmp);
    freeBitmap(bmp);


    Image * image = img_crop_border(original, false);

    Vector * chars = vec_create(10);
    Image * character = img_extract_character(image);
    while(character != NULL)
    {
        vec_add(chars, (void *) character);
        character = img_extract_character(image);
    }
    printf("character number -> %i\n", chars->size);

    for(int i = 0; i < chars->size; ++i)
        img_free((Image *) vec_get(chars, i));
    vec_free(chars, false);

    img_free(original);
    img_free(image);


    // test
    Network * net = net_create(400);
    net_addLayer(net, 225);
    net_addLayer(net, 75);
    net_addLayer(net, 26);
    net_free(net);

    return 0;
}
