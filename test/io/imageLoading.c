#include "../../src/io/bmpReader.h"
#include "../../src/utils/error.h"

int main()
{
    BITMAP * bmp = loadBitmap("res/normal_text.bmp");
    if(bmp == NULL)
    {
        fail("Fail to load the bitmap file");
    }
    if(bmp->info->width != 303 || bmp->info->height != 72)
    {
        fail("Wrong bitmap dimension");
    }
    freeBitmap(bmp);
    if(bmp != NULL)
    {
        fail("Fail to free the bitmap");
    }
    return 0;
}
