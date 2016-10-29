#include "io/image.h"
#include "processing/canny.h"

#include "ann/network.h"


int main()
{
    GdkPixbuf *img = img_load("Lorem-Droid-Mono-justify.bmp");
    Canny_filter *cf = canny(img);
    guchar *data = (guchar *) malloc(sizeof(guchar) * cf->w * cf->h * 3);
    int a = 0;
    for(int i = 0; i < cf->w * cf->h; ++i)
        for(int u = 0; u < 3; ++u)
            if(cf->data[i])
                data[++a] = 255;
            else
                data[++a] = 0;
    img_save(data, "normal_text_edges.png", cf->w, cf->h);
    free(data);
    g_object_unref(img);
    canny_free(cf);

}
