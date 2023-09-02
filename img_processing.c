#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "bmp.c"

void generate_kernel(double k[][11], int, double);
void convolve(image *, image *, int, double kernel[][11]);
void gaussian_blur(image *, image *, int, double);
uint8_t greyscale(uint8_t rgb[]);
void rgb_to_grayscale(image *);

// this function generates the values of kernel matrix by some mathematical calculations
void generate_kernel(double k[][11], int N, double sigma)
{
    // array for gauss value
    double gaussian_x[11][11];
    double gaussian_y[11][11];
    double sum = 0;

    // initializes to N/2z
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            gaussian_x[j][i] = i - (N / 2);
            gaussian_y[j][i] = j - (N / 2);
        }
    }

    // adds gaussian blur
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            double x = (pow(gaussian_x[j][i], 2) / (2 * pow(sigma, 2)));
            double y = (pow(gaussian_y[j][i], 2) / (2 * pow(sigma, 2)));
            k[j][i] = exp(-1 * (x + y));
            sum += k[j][i];
        }
    }

    // normalize kernel values
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            k[j][i] /= sum;
        }
    }
    /*
        Some simple examples of gaussian kernel matrices:
        double kernel[3][3]={
        {1,2,1},
        {2,4,2},
        {1,2,1}
        };

        double kernel[5][5]={
        {1,  4,  7,  4,  1},
        {4, 16, 26, 16,  4},
        {7, 26, 41, 26,  7},
        {4, 16, 26, 16,  4},
        {1,  4,  7,  4,  1}
        };

        double kernel[7][7]={
        {  0,  0,  1,  2,  1,  0,  0},
        {  0,  3, 13, 22, 13,  3,  0},
        {  1, 13, 59, 97, 59, 13,  1},
        {  2, 22, 97,159, 97, 22,  2},
        {  1, 13, 59, 97, 59, 13,  1},
        {  0,  3, 13, 22, 13,  3,  0},
        {  0,  0,  1,  2,  1,  0,  0}
        };
    */
}

uint8_t greyscale(uint8_t rgb[3])
{
    return (uint8_t)((0.3 * rgb[0]) + (0.59 * rgb[1]) + (0.11 * rgb[2]));
};

void convolve(image *input, image *output, int num, double kernel[][11])
{
    uint32_t ht = input->height;
    uint32_t wt = input->width;

    uint32_t padding = 2 * (num / 2);
    uint8_t padded[ht + padding][wt + padding][3];
    uint16_t temp[ht][wt][3];

    // initializes the values of padded matrix to be 0
    for (int i = 0; i < ht + padding; i++)
    {
        for (int j = 0; j < wt + padding; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                padded[i][j][k] = 0;
            }
        }
    }

    // copies the value of pixel values of input image to padded matrix in appropriate location
    for (int i = 0; i < ht; i++)
    {
        for (int j = 0; j < wt; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                padded[padding / 2 + i][padding / 2 + j][k] = input->pdata[i][j][k];
            }
        }
    }

    // initializes the value of temp matrix to 0
    for (int i = 0; i < ht; i++)
    {
        for (int j = 0; j < wt; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                temp[i][j][k] = 0;
            }
        }
    }

    // algorithm for convolving the matrix padded with kernel matrix
    for (int i = padding / 2; i < padding / 2 + ht; i++)
    {
        for (int j = padding / 2; j < padding / 2 + wt; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                for (int m = 0; m < num; m++)
                {
                    for (int n = 0; n < num; n++)
                    {
                        temp[i - padding / 2][j - padding / 2][k] += padded[i + m - padding / 2][j + n - padding / 2][k] * kernel[m][n];
                    }
                }
            }
        }
    }

    // copies the contents to temp to pdata of output image
    for (int i = 0; i < ht; i++)
    {
        for (int j = 0; j < wt; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                if (temp[i][j][k] > 255)
                    temp[i][j][k] = 255;
                else if (temp[i][j][k] < 0)
                    temp[i][j][k] = 0;
                output->pdata[i][j][k] = (uint8_t)temp[i][j][k];
            }
        }
    }
}

void gaussian_blur(image *input, image *output, int num, double sigma)
{
    // declaration of kernel matrix of size 11x11
    double kernel[11][11];
    // initialization of the values to 0
    for (int i = 0; i < 11; i++)
    {
        for (int j = 0; j < 11; j++)
        {
            kernel[i][j] = 0;
        }
    }
    // generate values and stores them in kernel matrix
    generate_kernel(kernel, num, sigma);
    // convolves the input with kernel and save it to output
    convolve(input, output, num, kernel);
}

void rgb_to_grayscale(image *output)
{
    for (int i = 0; i < output->height; i++)
    {
        for (int j = 0; j < output->width; j++)
        {
            output->pdata[i][j][0] = greyscale(output->pdata[i][j]);
            output->pdata[i][j][1] = greyscale(output->pdata[i][j]);
            output->pdata[i][j][2] = greyscale(output->pdata[i][j]);
        }
    }
}