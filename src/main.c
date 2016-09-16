#include "main.h"
#include "io/bmpReader.h"
#include "utils/image.h"
#include "utils/vector.h"

int main()
{
    BITMAP * bmp = loadBitmap("res/normal_text_small.bmp");
    Image * original = img_create(bmp);
    freeBitmap(bmp);

    img_print(original);

    Image * image = img_crop_border(original, false);
    img_print(image);

    Vector * chars = vec_create(10);
    Image * character = img_extract_character(image);
    while(character != NULL)
    {
        vec_add(chars, (void *) character);
        img_print(character);
        character = img_extract_character(image);
    }
    printf("character number -> %i\n", chars->size);

    for(int i = 0; i < chars->size; ++i)
        img_free((Image *) vec_get(chars, i));
    vec_free(chars, false);

    img_free(original);
    img_free(image);

    return 0;
}
