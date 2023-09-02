#include <stdint.h>
#include <stdio.h>
#include "../images.c"

#define FAILURE 1
#define SUCCESS 0

typedef struct
{
    uint8_t id[2]; // this is id of the image which is 'BM' for bmp file
} bmpid;

typedef struct
{
    uint32_t size;     // total size of the file
    uint32_t reserved; // reserved(=0)
    uint32_t offset;   // offset from beginning of file to beginning of the pixel values
} header;

typedef struct
{
    uint32_t info_sz;      // size of this header(=40 bytes)
    uint32_t width;        // width of image in pixel
    uint32_t height;       // height of image in pixel
    uint16_t nplanes;      // no of planes used(=1)
    uint16_t bpp;          // no of bits used to represent a color in a pixel(usually 24bits(3 bytes) for rgb)
    uint32_t compr_type;   // type of compression if any (=0 if no compression)
    uint32_t px_size;      // size of the file used for storing pixel values
    uint32_t hres;         // horizontal resolution (pixels/meter)
    uint32_t vres;         // vertical resolution (pixels/meter)
    uint32_t nused_colors; // no of used colors. For a 8-bit / pixel bitmap this will be 256
    uint32_t nimp_colors;  // no of imp colors(0= all)  (generally ignored)
} info;

uint32_t read_bmp_to_struct(char *, image *);
uint32_t save_img_to_file(const char *, image *);
void save_4bytes(uint32_t, FILE *);