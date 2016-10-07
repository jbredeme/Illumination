/**
 * Author: Jarid Bredemeier
 * Email: jpb64@nau.edu
 * Date: Tuesday, September 20, 2016
 * File: ppm.h
 * Copyright © 2016 All rights reserved
 */
 
#ifndef ppm_h
#define ppm_h

/**
 * Pixel
 *
 * @description three 1 byte unsigned characters used to store RGB color
 * values of a pixel.
 */
typedef struct Pixel {
    unsigned char red, green, blue;

} Pixel;

/**
 * Image
 *
 * @description structure used to represent portable pixmap image data in
 * system memory.
 */
typedef struct Image {
    char *magic_number;
    int width, height;
    int max_color;
    Pixel *image_data;

} Image;

// function declarations
void write_p6_image(char *filename, Image *image);
void write_p3_image(char *filename, Image *image);
 
#endif