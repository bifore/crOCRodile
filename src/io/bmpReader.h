#ifndef IO_BMP_READER_H_
#define IO_BMP_READER_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#pragma pack(push, 1)
typedef struct
{
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
} BITMAP_HEADER;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct
{
    uint32_t headerSize;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bitCount;
    uint32_t compression;
    uint32_t imageBitSize;
    uint32_t xRes;
    uint32_t yRes;
    uint32_t nbColors;
    uint32_t nbImportantColors;
} BITMAP_INFO_HEADER;
#pragma pack(pop)

typedef struct
{
    BITMAP_HEADER * header;
    BITMAP_INFO_HEADER * info;
    unsigned char * raster;
} BITMAP;

BITMAP * loadBitmap(char * path);
void freeBitmap(BITMAP * bitmap);

#endif /* !IO_BMP_READER_H_ */
