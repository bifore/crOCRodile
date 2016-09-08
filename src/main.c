#include "main.h"
#include "io/bmpReader.h"

int main()
{
    BITMAP * bmp = loadBitmap("normal_text.bmp");
    freeBitmap(bmp);
    return 0;
}
