#include "io/image.h"
#include "processing/canny.h"

int main()
{
    GdkPixbuf *img = img_load("normal_text.bmp");
    Canny_filter *cf = canny(img);
    for(int y = 0; y < cf->h; ++y)
    {
        for(int x = 0; x < cf->w; ++x)
            if(cf->data[y * cf->w + x])
                printf("#");
            else
                printf(" ");
        printf("\n");

    }
    canny_free(cf);
    g_object_unref(img);
}
