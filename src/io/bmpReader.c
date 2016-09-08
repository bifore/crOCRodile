#include "bmpReader.h"
#include "../utils/error.h"

BITMAP * loadBitmap(char * path)
{
    FILE * file = fopen(path, "r");
    if(file == NULL)
    {
        fail("Unable to open the file");
    }

    BITMAP_HEADER * header;
    //TODO myMalloc
    header = (BITMAP_HEADER *) malloc(sizeof(BITMAP_HEADER));
    fread(header, sizeof(BITMAP_HEADER), 1, file);
    if(header->type != 0x4D42)
    {
        fail("Wrong file type");
    }

    BITMAP_INFO_HEADER * info;
    //TODO myMalloc
    info = (BITMAP_INFO_HEADER *) malloc(sizeof(BITMAP_INFO_HEADER));
    fread(info, sizeof(BITMAP_INFO_HEADER), 1, file);

    //jump too the beginning of the raster
    fseek(file, header->offset, SEEK_SET);

    //TODO myMalloc
    unsigned char * raster = (unsigned char *) malloc(info->imageBitSize);
    if(!raster)
    {
        fail("Unable to allocate the raster");
    }
    fread(raster, info->imageBitSize, 1, file);

    fclose(file);

    //TODO myMalloc
    BITMAP * bitmap = (BITMAP *) malloc(sizeof(BITMAP));
    bitmap->header = header;
    bitmap->info = info;
    bitmap->raster = raster;
    return bitmap;
}

void freeBitmap(BITMAP * bitmap)
{
    //TODO myFree
    free(bitmap->header);
    free(bitmap->info);
    free(bitmap->raster);
    free(bitmap);
}
