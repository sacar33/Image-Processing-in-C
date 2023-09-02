#include <stdio.h>
#include "img_processing.c"

int main()
{
    // declaring two objects file1 for input image and file 2 for output image
    image file1, file2;

    // creating input and output pointers for ease
    image *input = &file1;
    image *output = &file2;

    // reads the file and stores the data in input
    // Prototype:
    // read_bmp_to_struct("path_to_your_input_bmp_file", address of the object(or simply pointer to that obj));
    read_bmp_to_struct("sample images/sample1.bmp", input);
    // copies input and stores in output
    copyimage(input, output);

    // apply gaussian blur to input and save in output
    //  Prototype:
    //  gaussian_blur(input,output,size of kernel matrix(nxn)[max=11],sigma(higher the value more the blurring effect));
    gaussian_blur(input, output, 7, 2);

    // save the data stored in output to a file
    // Prototype:
    //  save_img_to_file("Output_file_path", address of the object(or simply pointer to that obj))
    save_img_to_file("output images/blur.bmp", output);

    copyimage(input, output);
    rgb_to_grayscale(output);
    save_img_to_file("output images/grey.bmp", output);

    return 0;
}
