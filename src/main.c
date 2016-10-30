#include "io/image.h"
#include "processing/canny.h"
#include "util/image.h"

#include "ann/network.h"
#include "defaults.h"


int main()
{
    GdkPixbuf *img = img_load(IMAGE_LOREM ".bmp");
    Canny_filter *cf = canny(img);
    guchar *data = (guchar *) malloc(sizeof(guchar) * cf->w * cf->h * 3);
    int a = 0;
    for(int i = 0; i < cf->w * cf->h; ++i)
        for(int u = 0; u < 3; ++u)
            if(cf->data[i])
                data[++a] = 255;
            else
                data[++a] = 0;
    img_save(data, IMAGE_LOREM IMAGE_FINAL_SUFFIX, cf->w, cf->h);
    free(data);

    Image * original = img_create(img);
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
    {
        Image *img = (Image *) vec_get(chars, i);
        Image *norm = img_normalize(img, 20);
        img_print(norm);
        img_free(img);
        img_free(norm);
    }
    vec_free(chars, false);

    img_free(original);
    img_free(image);

    g_object_unref(img);
    canny_free(cf);

}
