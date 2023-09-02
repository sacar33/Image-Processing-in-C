#include "headers/images.h"
#include <string.h> //for using memset
#include <stdlib.h> //for DMA

image createImage(uint32_t width, uint32_t height)
{
    // creating an obj of struct image
    image img;

    // calculating the total size needed for storing pixel values of the image
    uint32_t size = width * height * 3;

    if (size > 0)
    {
        img.width = width;
        img.height = height;

        // allocating size needed to store pixel values in 3x3 matrix "pdata"
        img.pdata = (uint8_t ***)malloc(height * sizeof(uint8_t **));
        for (int i = 0; i < height; i++)
        {
            img.pdata[i] = (uint8_t **)malloc(width * sizeof(uint8_t *));
            for (int j = 0; j < width; j++)
            {
                img.pdata[i][j] = (uint8_t *)malloc(3 * sizeof(uint8_t));
            }
        }

        // this makes sure that the values of pixel is 0 initially
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                memset(img.pdata[i][j], 0, 3);
            }
        }
    }
    // returning the created obj
    return img;
}

// this function frees the memory allocated for storing pixel values pointed by the triple pointer pdata and assigns it to null as well as assign height and width 0
void resetImage(image *img)
{
    if (img->pdata != NULL)
    {
        for (int i = 0; i < img->height; i++)
        {
            for (int j = 0; j < img->width; j++)
            {
                free(img->pdata[i][j]);
                img->pdata[i][j] = NULL;
            }
            free(img->pdata[i]);
            img->pdata[i] = NULL;
        }
        free(img->pdata);
        img->pdata = NULL;
    }
    img->height = 0;
    img->width = 0;
}

// simply copies values of in to out
void copyimage(image *in, image *out)
{
    *out = createImage(in->width, in->height);
    for (int i = 0; i < in->height; i++)
    {
        for (int j = 0; j < in->width; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                out->pdata[i][j][k] = in->pdata[i][j][k];
            }
        }
    }
}