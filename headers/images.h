#include <stdint.h>

typedef struct // structure to store the width height and pixel data(in a 3x3 matrix) of the image
{
    uint32_t width;
    uint32_t height;
    uint8_t ***pdata;
} image;

image createImage(uint32_t, uint32_t); // decalation of various functions used in images.c file
void resetImage(image *);
void copyimage(image *, image *);
