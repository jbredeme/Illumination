/**
 * Author: Jarid Bredemeier
 * Email: jpb64@nau.edu
 * Date: Tuesday, September 20, 2016
 * File: ppm.c
 * Copyright © 2016 All rights reserved 
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "ppm.h"

/**
 * check_rgb_bits
 *
 * @param red - integer value represents the red color channel
 * @param green - integer value represents the green color channel
 * @param blue - integer value represents the blue color channel
 * @param max - integer value that represents the color channels maximum value
 * @returns min - integer value that represents the color channels maximum value
 * @description this function checks if three integer values against a maximum and minimum values
 * and its primary function to check color channels.
 */
int check_rgb_bits(int red, int green, int blue, int max, int min) {
	if(((red > max) || (red < min)) || ((green > max) || (green < min)) || ((blue > max) || (blue < min))){
		return(1);
		
	} else {
		return(0);
		
	}
		
}


/**
 * read_image
 *
 * @param filename - string pointer that represents a file name
 * @param image - an image structure
 * @returns void
 * @description takes in two pointers as parameters, a filename of a ppm image and image structure
 * use to store data read in from the ppm image file.
 */
void read_image(char *filename, Image *image) {
	
    char buffer[64];
	FILE *fpointer;
	int row, column, red, green, blue;
	
	// Open file steam for reading
	fpointer = fopen(filename, "r");
	
	// Check to see if file was opened successfully
	if(fpointer == NULL) {
		fprintf(stderr, "Error, unable to open file.\n");

		// Close file stream flush all buffers
		fclose(fpointer);
		exit(-1);
		 
	} else {
		// Make sure we are reading from the beginning of the file
		rewind(fpointer);

		// Read in the first to characters
		buffer[0] = fgetc(fpointer);
		buffer[1] = fgetc(fpointer);

		// Check the magic number
		if((buffer[0] == 'P') && (buffer[1] == '6')) {
			image->magic_number = "P6";

		} else if((buffer[0] == 'P') && (buffer[1] == '3')) {
			image->magic_number = "P3";

		} else {
			 fprintf(stderr, "Error, unacceptable image format while reading in the file.\n Magic number must be P6 or P3.\n");
			 exit(-2);
			 
		}

		// Ignore comments, whitespaces, carrage returns, and tabs
		while(isdigit(buffer[0]) == 0){
			// If you run into a comment proceed till you reach an newline character
			if(buffer[0] == '#') {
				do {
					buffer[0] = fgetc(fpointer);
					
				} while(buffer[0] != '\n');

			} else {
				buffer[0] = fgetc(fpointer);
				
			}

		}
		
		// Move back one character, tried using 
		ungetc(buffer[0], fpointer);

		// Read in <width> whitespace <height>
		if(fscanf(fpointer, "%d %d", &image->width, &image->height) != 2) {
			 fprintf(stderr, "Error, invalid width and/or height while reading in the file.\n");
			 exit(-2);
			 
		}
		
		// Read in <maximum color value>
		if(fscanf(fpointer, "%d", &image->max_color) != 1) {
			 fprintf(stderr, "Error, invalid maximum color value.\n");
			 exit(-2);
			 
		}

		// Validate 8-bit color value
		if((image->max_color > 255) || (image->max_color < 0)) {
			 fprintf(stderr, "Error, input file's maximum color value is not 8-bits per channel.\n");
			 exit(-2);
			 
		}
		
		// Allocated memory size for image data
		image->image_data = malloc(sizeof(Pixel) * image->width * image->height);

		// If magic number is P6 fread, if magic number is P3 for loop
		if(image->magic_number[1] == '6') {
			// Advance the file pointer by one char
			fgetc(fpointer);
			
			// Read in raw image data
			fread(image->image_data, sizeof(Pixel), image->width * image->height, fpointer);
						
		} else if(image->magic_number[1] == '3') {
			// Read in ascii image data
			for(row = 0; row < image->height; row++) {
				for(column = 0; column < image->width; column++) {					

					// Have to store values into int, larger values more then 1 byte will be truncated
					// therefore making it not possible to check for color channels values over 8-bits	
					fscanf(fpointer, "%d", &red);
					fscanf(fpointer, "%d", &green);
					fscanf(fpointer, "%d", &blue);					
					
					if(check_rgb_bits(red, green, blue, 255, 0) == 1) {
						fprintf(stderr, "Error, a channel color value is not 8-bits.\n");
						exit(-3);
						
					} else {
						image->image_data[(image->width) * row + column].red = red;
						image->image_data[(image->width) * row + column].green = green;
						image->image_data[(image->width) * row + column].blue = blue;
						
					}					
		
				}
				
			}
			
		} else {
			fprintf(stderr, "Error, invalid magic number.\n");
			exit(-2);
			
		}

		// Close file stream flush all buffers
		fclose(fpointer);	
		
	}
	
}


/**
 * write_p6_image
 *
 * @param filename - string pointer that represents a file name
 * @param image - an image structure
 * @returns void
 * @description this function writes ASCII data into ppm p6 format. Accepts two parameters, a pointer to a
 * file stream and a poiner to an image structure. Writes to the file stream using fwrite.
 */
void write_p6_image(char *filename, Image *image) {
	FILE *fpointer;
	fpointer = fopen(filename, "wb");
	
	if(fpointer == NULL) {
		fprintf(stderr, "Error, unable to open file.\n");

		// Close file stream flush all buffers
		fclose(fpointer);
		exit(-1);
		 
	} else {
		fprintf(fpointer, "%s\n", "P6");
		fprintf(fpointer, "%d %d\n", image->width, image->height);
		fprintf(fpointer, "%d\n", image->max_color);
			
		// ASCII code is a 7-bit code stored in a byte
		fwrite(image->image_data, sizeof(Pixel), image->width * image->height, fpointer);
		
		// Close file stream flush all buffers
		fclose(fpointer);
		
		
	}
	
}


/**
 * write_p3_image
 *
 * @param filename - string pointer that represents a file name
 * @param image - an image structure
 * @returns void
 * @description this function writes raw data into ppm p3 ASCII format. Accepts two parameters, a pointer to a
 * file stream and a poiner to an image structure. This function uses fprintf to write to the file stream
 * using a nested loop, converting integers to strings using sprintf then wites the string to an 
 * output.
 */
void write_p3_image(char *filename, Image *image) {
	int row, column;
	char buffer[64];
	FILE *fpointer;
	
	fpointer = fopen(filename, "w");
	
	if(fpointer == NULL) {
		fprintf(stderr, "Error, unable to open file.\n");

		// Close file stream flush all buffers
		fclose(fpointer);
		exit(-1);
		 
	} else {
		fprintf(fpointer, "%s\n", "P3");
		fprintf(fpointer, "%d %d\n", image->width, image->height);
		fprintf(fpointer, "%d\n", image->max_color);
		
		// Read in ascii image data
		for(row = 0; row < (image->height); row++) {
			for(column = 0; column < (image->width); column++) {
				sprintf(buffer, "%d", image->image_data[(image->width) * row + column].red);
				fprintf(fpointer, "%s\n", buffer);
				
				sprintf(buffer, "%d", image->image_data[(image->width) * row + column].green);
				fprintf(fpointer, "%s\n", buffer);
				
				sprintf(buffer, "%d", image->image_data[(image->width) * row + column].blue);
				fprintf(fpointer, "%s\n", buffer);				
		
			}
			
		}
		
		// Close file stream flush all buffers
		fclose(fpointer);
		
	}
	
}