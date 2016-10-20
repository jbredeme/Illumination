/**
 * Author: Jarid Bredemeier
 * Email: jpb64@nau.edu
 * Date: Thursday, October 6, 2016
 * File: main.c
 * Copyright © 2016 All rights reserved
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "ppm\ppm.h"
#include "json\json.h"
#include "raycaster\raycaster.h"

// Allocate object array, specifications do not support more then 128 objects in a scene
Object objects[MAX_OBJECTS];

/**
 * main
 *
 * @param argc - contains the number of arguments passed to the program
 * @param argv - a one-dimensional array of strings
 * @returns 0 upon successful completion 
 * @description main function called by the operating system when the user runs the program. 
 */
int main(int argc, char *argv[]){
	int num_objects, count, index;
	FILE *fpointer;
	Image *ppm_image;
	
	// Allocate memory for Image
	ppm_image = (Image *)malloc(sizeof(Image));
	if(ppm_image == NULL) {
		fprintf(stderr, "Failed to allocate memory.\n");
		exit(-1);
		
	}
	
	// Validate command line input(s)
	if(argc != 5){
		fprintf(stderr, "Error, incorrect usage!\nCorrect usage pattern is: raycast width height input.json output.ppm.\n");
		exit(-1);
		
	} else {
		// Loop through the first two inputs to check if they are integers
		for(index = 1; index < 3; index++){
			for(count = 0; count < strlen(argv[index]); count++) {
				if((!(isdigit((argv[index])[count]))) && (((argv[index])[count]) != '.')){
					fprintf(stderr, "Error, incorrect width and/or height value(s).\n");
					exit(-1);
					
				}
				
			}

		}		
		
	}

	// Open json file for reading
	fpointer = fopen(argv[3], "r");
		
	if(fpointer == NULL) {
		fprintf(stderr, "Error, could not open file.\n");
		// Close file stream flush all buffers
		fclose(fpointer);
		exit(-1);
		
	} else {
		// Set Image properties
		ppm_image->width = atoi(argv[1]);
		ppm_image->height = atoi(argv[2]);
		ppm_image->max_color = MAX_COLOR;
		
		// Allocate memory size for image data
		ppm_image->image_data = malloc(sizeof(Pixel) * ppm_image->width * ppm_image->height);
		if((ppm_image->image_data) == NULL) {
			fprintf(stderr, "Failed to allocate memory.\n");
			exit(-1);

		}
		
		// Read in json scene return number of objects
		num_objects = json_read_scene(fpointer, objects);
		
		if(num_objects <= 0) {
			// Empty Scene
			
		} else {
			// Print objects read in from the json file
			print_scene(objects, num_objects);
			
			// Raycast scene, write out to ppm6 image
			write_p6_image(argv[4], raycaster(objects, ppm_image, num_objects));
			
			// Deallocate memory previously allocated by calls to malloc
			free(ppm_image->image_data);
			free(ppm_image);
		}
		
	}

	return(0);
	
} 