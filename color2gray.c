/*
Convert an image to grayscale.

*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

void colorToGrayscale(unsigned char *gray_img, unsigned char * color_img, int width, int height);

const int IS_PNG = 1;
const int IS_JPG = 2;
const int DESIRED_CHANNELS = 3;
const int MAX_NAME_LENGTH = 500;
int main(int argc, char *argv[]) {
    if (argc < 4){
        printf("Usage: color2Grayscale ${input color image file} ${output grayscale image file} ${image type}\n Image Types:\n\t1: PGN\n\t2: JPG");
	exit(-1);
    }

    int width, height, channels, type;
    char  in_name[MAX_NAME_LENGTH], out_name[MAX_NAME_LENGTH];
    strcpy(in_name, argv[1]);
    strcpy(out_name, argv[2]);
    type = atoi(argv[3]);
// load and conver the image to 3 channels (ignore the transparancy channel)
    unsigned char *color_img = stbi_load(in_name, &width, &height, &channels, 0); 
    if(color_img == NULL) {
        printf("Error in loading the image\n");
        exit(-1);
    }
    printf("Loaded image %s with a width of %dpx, a height of %dpx and %d channels\n", in_name, width, height, channels);

    // Convert the input image to gray
    int gray_channels = channels == 4 ? 2 : 1;
    size_t gray_img_size = width * height * gray_channels;

    unsigned char *gray_img = (unsigned char *)malloc(gray_img_size);
    if(gray_img == NULL) {
        printf("Unable to allocate memory for the gray image.\n");
        exit(1);
    }
    printf("Create a image array with a width of %dpx, a height of %dpx and %d channels\n", width, height, gray_channels);

   colorToGrayscale(gray_img, color_img, width, height);

    if (type == IS_PNG)
    	stbi_write_png(out_name, width, height, gray_channels, gray_img, width * gray_channels);
    else
        if (type == IS_JPG)
        /*The last parameter of the stbi_write_jpg function is a quality parameter that goes from 1 to 100.
         Since JPG is a lossy image format, you can chose how much data is dropped at save time. 
         Lower quality means smaller image size on disk and lower visual image quality.*/
            stbi_write_jpg(out_name, width, height, gray_channels, gray_img, 100); 
    printf("Wrote image %s with a width of %dpx, a height of %dpx and %d channels\n", out_name, width, height, channels);

    stbi_image_free(gray_img);
}

void colorToGrayscale(unsigned char *gray_img, unsigned char * color_img, int width, int height){
    unsigned char pixel[DESIRED_CHANNELS];
    for (int row = 0; row < height; row++)
    {
    for (int column = 0; column < width; column++)
    {
        int greyColor = row*width + column;
        int rgbColor = greyColor*DESIRED_CHANNELS;
        /*convert color pixel from color_img to gray scale pixel by pixel and
         In color_img, a pixel has been represented by a tuple of (r, g,b) values*/
        pixel[0] = color_img[rgbColor];
        pixel[1] = color_img[rgbColor + 1];
        pixel[2] = color_img[rgbColor + 2];
        
        gray_img[greyColor] = pixel[0]*0.21 + pixel[1]*0.72 + pixel[2]*0.07;
    }
    }
    

}
