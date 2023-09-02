#include <stdio.h>
#include <stdint.h>
#include "headers/bmp.h"

uint32_t read_bmp_to_struct(char *filename, image *img)
{
    // To make sure the img is fresly new or to delete any values stored in img
    resetImage(img);

    bmpid bmp_id;
    header _header;
    info _info;

    FILE *stream = fopen(filename, "rb");

    if (stream == NULL)
        return FAILURE;

    if (fread(bmp_id.id, sizeof(uint16_t), 1, stream) != 1)
    {
        fclose(stream);
        return FAILURE;
    }

    // checking if the id of the file is indeed 'BM'
    if (*((uint16_t *)bmp_id.id) != 0x4D42)
    {
        fclose(stream);
        return FAILURE;
    }

    if (fread(&_header, sizeof(header), 1, stream) != 1)
    {
        fclose(stream);
        return FAILURE;
    }

    if (fread(&_info, sizeof(info), 1, stream) != 1)
    {
        fclose(stream);
        return FAILURE;
    }

    if (_info.compr_type != 0)
        fprintf(stderr, "Warning!!! Compression type is not supported");

    if (fseek(stream, _header.offset, SEEK_SET))
    {
        fclose(stream);
        return FAILURE;
    }
    *img = createImage(_info.width, _info.height);
    uint8_t temp[img->height][img->width][3];

    // fread(temp,1 , _info.px_size, stream);

    int bpr = (_info.bpp * _info.width) / 8;
    for (int i = 0; i < _info.height; i++)
    {
        fread(temp[i], 1, bpr, stream);
    }

    for (int i = 0; i < img->height; i++)
    {
        for (int j = 0; j < img->width; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                img->pdata[img->height - 1 - i][j][3 - 1 - k] = temp[i][j][k];
            }
        }
    }
    fclose(stream);
    return SUCCESS;
}

void save_4bytes(uint32_t val, FILE *stream)
{
    fprintf(stream, "%c", (val & 0x000000FF) >> 0 * 8);
    fprintf(stream, "%c", (val & 0x0000FF00) >> 1 * 8);
    fprintf(stream, "%c", (val & 0x00FF0000) >> 2 * 8);
    fprintf(stream, "%c", (val & 0xFF000000) >> 3 * 8);
}

uint32_t save_img_to_file(const char *filename, image *img)
{
    uint32_t padding = 4 - ((img->width * 3) % 4);
    if (padding == 4)
        padding = 0;
    uint32_t padded_size = ((img->width * 3) + padding) * img->height;

    // uint32_t padded_size=img->width*3 * img->height;

    FILE *stream = fopen(filename, "wb");
    fprintf(stream, "BM");
    save_4bytes(padded_size + 54, stream);
    save_4bytes(0, stream);
    save_4bytes(54, stream);
    save_4bytes(40, stream);
    save_4bytes(img->width, stream);
    save_4bytes(img->height, stream);

    fprintf(stream, "%c", 1);
    fprintf(stream, "%c", 0);

    fprintf(stream, "%c", 24);
    fprintf(stream, "%c", 0);

    save_4bytes(0, stream);
    save_4bytes(padded_size, stream);
    save_4bytes(0, stream);
    save_4bytes(0, stream);
    save_4bytes(0, stream);
    save_4bytes(0, stream);

    for (int i = 0; i < img->height; i++)
    {
        for (int j = 0; j < img->width; j++)
        {
            for (int k = 2; k >= 0; k--)
            {
                fputc(img->pdata[img->height - 1 - i][j][k], stream);
            }
        }
    }
    fclose(stream);
    return SUCCESS;
}