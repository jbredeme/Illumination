/**
 * Author: Jarid Bredemeier
 * Email: jpb64@nau.edu
 * Date: Tuesday, September 20, 2016
 * File: main.c
 * Copyright © 2016 All rights reserved
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "json\json.h"
#include "ppm\ppm.h"
#include "raycaster\raycaster.h"

// Allocate object array, specifications do not support more then 128 objects in a scene
Object objects[128];
int maximum_color;

/**
 * main
 *
 * @param argc - contains the number of arguments passed to the program
 * @param argv - a one-dimensional array of strings
 * @returns 0 upon successful completion 
 * @description main function called by the operating system when the user runs the program. 
 */
int main(int argc, char *argv[]){
	FILE *fpointer;
	int num_objects, count, index;
	Image *ppm_image;
	maximum_color = 255;
	
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
		ppm_image->max_color = maximum_color;
		
		// Allocate memory size for image data
		ppm_image->image_data = malloc(sizeof(Pixel) * ppm_image->width * ppm_image->height);
		
		// Read in json scene return number of objects
		num_objects = json_read_scene(fpointer, objects);
		
		if(num_objects <= 0) {
			// Empty Scene
			
		} else {
			
			// Display json objects read in, valid for camera, sphere, and plane
			printf("\n- NUMBER OF OBJECTS: %d -\n\n", num_objects);
			for(count = 0; count < num_objects; count++) {
				
				// Account for empty object data
				if((objects[count].type) == NULL){
					printf("Type: Empty Object\n");
					printf("No properties discovered\n\n");
					
				} else {
					if(strcmp(objects[count].type, "camera") == 0){
						printf("Type: %s\n", objects[count].type);
						printf("Width: %lf\n", objects[count].properties.camera.width);
						printf("Height: %lf\n\n", objects[count].properties.camera.height);
					}
					
					if(strcmp(objects[count].type, "sphere") == 0){
						printf("Type: %s\n", objects[count].type);
						printf("Radius: %lf\n", objects[count].properties.sphere.radius);
						printf("Color: %lf %lf %lf\n", objects[count].properties.sphere.color[0], objects[count].properties.sphere.color[1], objects[count].properties.sphere.color[2]);
						printf("Position: %lf %lf %lf\n\n", objects[count].properties.sphere.position[0], objects[count].properties.sphere.position[1], objects[count].properties.sphere.position[2]);
						
					}
					
					if(strcmp(objects[count].type, "plane") == 0){
						printf("Type: %s\n", objects[count].type);
						printf("Color: %lf %lf %lf\n", objects[count].properties.plane.color[0], objects[count].properties.plane.color[1], objects[count].properties.plane.color[2]);
						printf("Position: %lf %lf %lf\n", objects[count].properties.plane.position[0], objects[count].properties.plane.position[1], objects[count].properties.plane.position[2]);
						printf("Normal: %lf %lf %lf\n\n", objects[count].properties.plane.normal[0], objects[count].properties.plane.normal[1], objects[count].properties.plane.normal[2]);			
					
					}
					
				}
				
			}
			// Raycast scene, write out to ppm6 image
			write_p6_image(argv[4], raycaster(objects, ppm_image, num_objects));
			
		}
		
	}

	return(0);
	
} 