/**
 * Author: Jarid Bredemeier
 * Email: jpb64@nau.edu
 * Date: Tuesday, September 20, 2016
 * File: raycaster.c
 * Copyright © 2016 All rights reserved 
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "..\ppm\ppm.h"
#include "..\json\json.h"
#include "raycaster.h"

/**
 * sqr
 *
 * @param v - double precision floating point number
 * @returns static function, no return
 * @description math operation that squares a double precision floating
 */
static inline double sqr(double v) {
	return v * v;
	
}


/**
 * normalize
 *
 * @param v an array storing vector information
 * @returns static function, no return 
 * @description vector normalization, divide each component by its magnitude
 */
static inline void normalize(double *v) {
	double len = sqrt(sqr(v[0]) + sqr(v[1]) + sqr(v[2]));
	v[0] /= len;
	v[1] /= len;
	v[2] /= len;
	
}


/**
 * get_camera
 *
 * @param objects[]
 * @param num_objects used to set the counter that iterates through an array of objects 
 * @returns the index value within the array the camera was found, -1 otherwise if the camera was not found
 * @description iterates through a scene or array of objects looking for an object with type
 * camera and returns the index of that object in the scene array or -1 if no camera was found.
 */
int get_camera(Object objects[], int num_objects) {
	int i;
	
	for(i = 0; i < num_objects; i++){
		if(strcmp((objects[i].type), "camera") == 0) {
			return (i);
			
		}
		
	}
	
	return(-1);
	
}


/**
 * sphere_intersection
 *
 * @param ro - ray vector orgin
 * @param rd - ray vector direction
 * @param center - sphere center aka position
 * @param radius - sphere radius
 * @returns double percision float t value that represents length of the intersecting vector, and -1 if
 * no intersection was detected.
 * @description this function detects the distance a ray vector intersects the sphere
 */     
double sphere_intersection(double *ro, double *rd, double *center, double radius){
	double a, b, c, discriminant, t1, t0;

	// Step 1.) Find the equation for the object you are interested in..  
	// Step 2.) Parameterize the equation with a center point
	// Step 3.) Substitute the eq for a ray into our object equation.
	// Step 4.) Solve for t.
	// Step 4a.) Rewrite the equation (flatten).
	
	a = (sqr(rd[0]) + sqr(rd[1]) + sqr(rd[2]));
	b = (2 * (rd[0] * (ro[0] - center[0]) + rd[1] * (ro[1] - center[1]) + rd[2] * (ro[2] - center[2])));
	c = sqr(ro[0] - center[0]) + sqr(ro[1] - center[1]) + sqr(ro[2] - center[2]) - sqr(radius);

	discriminant  = sqr(b) - 4 * a * c;
	
	if(discriminant < 0) {
		// Has no solution
		return (-1);
		
	}

	// Quadratic formula
	t1 = (-1 * b + sqrt(sqr(b) - 4 * a * c)) / (2 * a);
	t0 = (-1 * b - sqrt(sqr(b) - 4 * a * c)) / (2 * a);
	
	if(t0 >= 0){
		return t0;
		
	} else if(t1 >= 0){
		return t1;
		
	} else {
		return (-1);
		
	}

}


/**
 * plane_intersection
 *
 * @param ro - ray vector orgin
 * @param rd - ray vector direction
 * @param pos - position
 * @param normal - the orthogonal normal vector to the plane
 * @returns double percision float t value that represents length of the intersecting vector, and -1 if
 * no intersection was detected.
 * @description this function detects the distance a ray vector intersects the plane
 */
double plane_intersection(double *ro, double *rd, double *pos, double *normal){
	// normal defines the orientation of the plane
	// the property that the dot product of two perpendicular vectors is equal to 0
	// p0 = plane position
	// (p - p0) * normal = 0
	// p = ro + rd + t
	// (ro + rd * t - p0) * normal = 0
	// ((ppos - ro) * normal) / (rd * normal) <- Dot product - a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
	double numerator, denominator, t;
	
	normalize(normal);

	numerator = (normal[0] * (pos[0] - ro[0])) + (normal[1] * (pos[1] - ro[1])) + (normal[2] * (pos[2] - ro[2])); 
	denominator = (normal[0] * rd[0]) + (normal[1] * rd[1]) + (normal[2] * rd[2]);
	
	t = numerator / denominator;
	
	if(t >= 0) {
		return t;
		
	} else {
		return (-1);
		
	}
	
}


/**
 * raycaster
 *
 * @param objects[] - collection of objects read in from the json parser
 * @param image - is an Image object used to store image data
 * @param num_objects - number of objects read in from the json parser use to set
 * iteration values for, for loops.
 * @returns Image - which is the image pointer to the image object that is used to store
 * the image data for write purposes.
 * @description this function implements the raycasting portion of this application it performs
 * the calculations for pixel scaling, and logic that uses the scene data to detect object ray
 * intersections, colors pixels related to the object data, and stores the collection of information
 * into an image data buffer to be written using a ppm write function.
 */
Image* raycaster(Object objects[], Image *image, int num_objects) {
	double pixel_height, pixel_width;
	double cx, cy;
	double h, w;
	double t, best_t;
	double red, green, blue;
	int row, column, index, t_object;
	double rd[3];

	// Set ray orgin
	double ro[3] = {0, 0, 0};
	
	// Set center x & y
	cx = 0;
	cy = 0;
	
	// Check scene for a camera
	index = get_camera(objects, num_objects);
	
	if(index == (-1)){
		// Missing camera
		fprintf(stderr, "Error, no camera object was found.\n");
		exit(-1);
		
	} else {
		// Get camera height and width
		h = objects[index].properties.camera.height;
		w = objects[index].properties.camera.width;
		
		// Scale pixels
		pixel_height = h / (image->height);
		pixel_width = w / (image->width);
		
	}

	for(row = 0; row < (image->height); row++) {
		
		for(column = 0; column < (image->width); column++) {
	
			rd[0] = (cx - (w / 2.0) + pixel_width * (column + 0.5));
			rd[1] = - 1 * (cy - (h / 2.0) + pixel_height * (row + 0.5));
			rd[2] = 1.0;
			
			// Normalize ray direction
			normalize(rd);
			best_t = INFINITY;
			
			for(index = 0; index < num_objects; index++) {
				t = 0;
				
				if((objects[index].type) != NULL) {
					if(strcmp((objects[index].type), "sphere") == 0){
						t = sphere_intersection(ro, rd, objects[index].properties.sphere.position, objects[index].properties.sphere.radius);
						//printf("Best t - sphere: %lf\n", t);
						//printf("Position: %lf %lf %lf\n\n", objects[index].properties.sphere.position[0], objects[index].properties.sphere.position[1], objects[index].properties.sphere.position[2]);
					
					} else if(strcmp((objects[index].type), "plane") == 0) {
						t = plane_intersection(ro, rd, objects[index].properties.plane.position, objects[index].properties.plane.normal);
						//printf("Best t - plane: %lf\n", t);
						//printf("Position: %lf %lf %lf\n\n", objects[index].properties.plane.position[0], objects[index].properties.plane.position[1], objects[index].properties.plane.position[2]);
				
					}
					
					// Get the best t value and object index
					if ((t > 0) && (t < best_t)){
						best_t = t;
						t_object = index;
					
					}
					
				}
				
				
			} // EoObject iteration loop
			
			if((best_t > 0) && (best_t != INFINITY)){
				
				if(strcmp(objects[t_object].type, "sphere") == 0) {
					red = objects[t_object].properties.sphere.color[0] * (image->max_color);
					image->image_data[(image->width) * row + column].red = red;
					
					green = objects[t_object].properties.sphere.color[1] * (image->max_color);
					image->image_data[(image->width) * row + column].green = green;
					
					blue = objects[t_object].properties.sphere.color[2] * (image->max_color);
					image->image_data[(image->width) * row + column].blue = blue;
				
				} else if(strcmp(objects[t_object].type, "plane") == 0){
					red = objects[t_object].properties.plane.color[0] * (image->max_color);
					image->image_data[(image->width) * row + column].red = red;
					
					green = objects[t_object].properties.plane.color[1] * (image->max_color);
					image->image_data[(image->width) * row + column].green = green;
					
					blue = objects[t_object].properties.plane.color[2] * (image->max_color);
					image->image_data[(image->width) * row + column].blue = blue;
					
				}
				
			}
			
		} // EoRow Loop
		
	} // EoColumn Loop 

	return image;
	
}