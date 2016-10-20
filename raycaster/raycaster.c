/**
 * Author: Jarid Bredemeier
 * Email: jpb64@nau.edu
 * Date: Thursday, October 6, 2016
 * File: raycaster.c
 * Copyright © 2016 All rights reserved 
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "..\math\vector_math.h"
#include "..\ppm\ppm.h"
#include "..\json\json.h"
#include "raycaster.h"

/**
 * TODO
 * 
 * @param normal - 
 * @param new_rd - 
 * @param reflected_vector - 
 * @param rd - 
 * @param specular_color -
 * @param light_color- 
 * @param color -
 */
void specular_highlight(double *normal, double *new_rd, double *reflected_vector, double *rd, double *specular_color, double *light_color, double *color) {
    double scalar1 = 0.0, scalar2 = 0.0, scalar3 = 0.0;
	
	scalar1 = vector_dot_product(normal, new_rd);
	scalar2 = vector_dot_product(rd, reflected_vector);
	
    if ((scalar1 > 0) && (scalar2 > 0)) {
        scalar3 = pow(scalar2, 25);
        color[0] = scalar3 * specular_color[0] * light_color[0];
        color[1] = scalar3 * specular_color[1] * light_color[1];
        color[2] = scalar3 * specular_color[2] * light_color[2];
		
    } else {
        color[0] = 0;
        color[1] = 0;
        color[2] = 0;
		
    }
	
}


/**
 * TODO
 * 
 * @param normal -
 * @param new_rd - 
 * @param light_color -
 * @param diffuse_color - 
 * @param color -
 */
void diffuse_reflection(double *normal, double *new_rd, double *light_color, double *diffuse_color, double *color) {
	double scalar = 0.0;

	scalar = vector_dot_product(normal, new_rd);
	
	if(scalar > 0) {
		color[0] = scalar * diffuse_color[0] * light_color[0];
		color[1] = scalar * diffuse_color[1] * light_color[1];
		color[2] = scalar * diffuse_color[2] * light_color[2];

	} else {
		color[0] = 0;
		color[1] = 0;
		color[2] = 0;		
		
	}
	
}


/**
 * TODO
 * 
 * @param a0 - 
 * @param theta - 
 * @param direction - 
 * @param distance - 
 * @returns
 */
double fang(double a0, double theta, double *direction, double *distance) {
	double scalar = 0.0;
	double new_distance[3] = {0, 0, 0};
	
	// Check light type
	if((theta == 0.0) && (direction[0] == 0) && (direction[1] == 0) && (direction[2] == 0)) {
		return (1.0);
		
	} else {
		vector_scale(distance, -1, new_distance);
		scalar = vector_dot_product(direction, new_distance);
		
		if(scalar >= cos((theta * M_PI) / 180)) { // <= Convert degrees into radians; (degrees * pi) / 180
			return (pow(scalar, a0));
			
		} else {
			return (0);
			
		}
		
	}
		
}


/**
 * TODO
 * 
 * @param a0 - 
 * @param a1 - 
 * @param a2 - 
 * @param distance - 
 * @returns
 */
double frad(double a0, double a1, double a2, double distance) {
	double scalar = 0.0;
	
	if(distance < INFINITY) {
		return ((1)/(a0 + (a1 * distance) + (a2 * pow(distance, 2))));
		
	} else {
		// Some default value, distance = infinity
		return (1.0);
		
	}

}


/**
 * Clamp takes a number and enforces a lower and upper limit by
 * 
 * @param number - the number to be clamped
 * @param min - number that is the lower limit
 * @param max - number that is the upper limit
 * @returns min if value is below lower limit, max if number is above upper limit, number otherwise
 */
double clamp(double number, double min, double max) {
	if(number > max) {
		return max;
		
	} else if (number < min) {
		return min;
		
	} else {
		return number;
		
	}
}


/**
 * Iterates through a scene or array of objects looking for an object with type camera and returns the index 
 * of that object in the scene array or -1 if no camera was found.
 *
 * @param objects - an array of objects
 * @param num_objects - used to set the counter that iterates through an array of objects 
 * @returns the index value within the array the camera was found, -1 otherwise if the camera was not found
 */
int get_camera(Object objects[], int num_objects) {
	int i;
	
	for(i = 0; i < num_objects; i++) {
		if(strcmp((objects[i].type), "camera") == 0) {
			return (i);
			
		}
		
	}
	
	return(-1);
	
}


/**
 * This function detects the distance a ray vector intersects the sphere.
 *
 * @param ro - ray vector orgin
 * @param rd - ray vector direction
 * @param center - sphere center aka position
 * @param radius - sphere radius
 * @returns double percision float t value that represents length of the intersecting vector, and -1 if no intersection was detected.
 */     
double sphere_intersection(double *ro, double *rd, double *center, double radius) {
	double a = 0.0, b = 0.0, c = 0.0, discriminant = 0.0, t1 = 0.0, t0 = 0.0;
	
	// Step 1.) Find the equation for the object you are interested in..  
	// Step 2.) Parameterize the equation with a center point
	// Step 3.) Substitute the eq for a ray into our object equation.
	// Step 4.) Solve for t.
	// Step 5.) Rewrite the equation (flatten).
	
	a = pow(rd[0], 2) + pow(rd[1], 2) + pow(rd[2], 2);
	b = (2 * (rd[0] * (ro[0] - center[0]) + rd[1] * (ro[1] - center[1]) + rd[2] * (ro[2] - center[2])));
	c = pow((ro[0] - center[0]), 2) + pow((ro[1] - center[1]), 2) + pow((ro[2] - center[2]), 2) - pow(radius, 2);
	
	discriminant = pow(b, 2) - 4 * a * c;
	
	if(discriminant < 0) {
		return (-1); // <= has no solution
		
	}

	// Quadratic formula
	t1 = (-1 * b + sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
	t0 = (-1 * b - sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
	
	if(t0 >= 0) {
		return t0;
		
	} else if(t1 >= 0) {
		return t1;
		
	} else {
		return (-1);
		
	}

}


/**
 * This function detects the distance a ray vector intersects the plane.
 *
 * @param ro - ray vector orgin
 * @param rd - ray vector direction
 * @param pos - position
 * @param normal - the orthogonal normal vector to the plane
 * @returns double percision float t value that represents length of the intersecting vector, and -1 if no intersection was detected.
 */
double plane_intersection(double *ro, double *rd, double *pos, double *normal) {
	// normal defines the orientation of the plane
	// the property that the dot product of two perpendicular vectors is equal to 0
	// p0 = plane position
	// (p - p0) * normal = 0
	// p = ro + rd + t
	// (ro + rd * t - p0) * normal = 0
	// ((ppos - ro) * normal) / (rd * normal) <- Dot product - a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
	double numerator = 0.0, denominator = 0.0, t = 0.0;
	
	normalize(normal);
	
	numerator = (normal[0] * (pos[0] - ro[0])) + (normal[1] * (pos[1] - ro[1])) + (normal[2] * (pos[2] - ro[2])); 
	denominator = (normal[0] * rd[0]) + (normal[1] * rd[1]) + (normal[2] * rd[2]);
	
	t = numerator / denominator;
	
	if(t >= 0) {
		return (t);
		
	} else {
		return (-1);
		
	}
	
}


/**
 * This function implements the raycasting portion of this application it performs the calculations for pixel scaling, and logic that uses the 
 * scene data to detect object ray intersections, colors pixels related to the object data, and stores the  collection of information into an 
 * image data buffer to be written using a ppm write function.
 *
 * @param objects - collection of objects read in from the json parser
 * @param image - is an Image object used to store image data
 * @param num_objects - number of objects read in from the json parser use to set iteration values for, for loops.
 * @returns Image - which is the image pointer to the image object that is used to store the image data for write purposes.
 */
Image* raycaster(Object objects[], Image *image, int num_objects) {
	double pixel_height, pixel_width;
	double h, w;
	double red, green, blue, distance, best_distance;
	int row, column, index, closest_object;
	
	// Coloring vector
	double pixel_coloring[3];
	
	// Set ray orgin
	double ro[3] = {0, 0, 0};
	double rd[3] = {0, 0, 0};
	
	// Set center x & y
	double cx = 0;
	double cy = 0;
	
	// Get the index of the camera
	index = get_camera(objects, num_objects);
	
	// // Check scene for a camera, -1 means camera is missing
	if(index == (-1)) {
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

	// Iterate over pixel matrix
	for(row = 0; row < (image->height); row++) {
		for(column = 0; column < (image->width); column++) {
			rd[0] = (cx - (w / 2.0) + pixel_width * (column + 0.5));
			rd[1] = - 1 * (cy - (h / 2.0) + pixel_height * (row + 0.5));
			rd[2] = 1.0;
			
			normalize(rd); // <= Normalize ray direction
			best_distance = INFINITY;
			
			// Ambient color
			pixel_coloring[0] = 0;
			pixel_coloring[1] = 0;
			pixel_coloring[2] = 0;
			
			// Execute object intersection test
			for(index = 0; index < num_objects; index++) {
				distance = 0;
				
				if((objects[index].type) != NULL) { // <= Check against type nulls
					if(strcmp((objects[index].type), "sphere") == 0) {
						distance = sphere_intersection(ro, rd, objects[index].properties.sphere.position, objects[index].properties.sphere.radius);
					
					} else if(strcmp((objects[index].type), "plane") == 0) {
						distance = plane_intersection(ro, rd, objects[index].properties.plane.position, objects[index].properties.plane.normal);
				
					}
					
					if ((distance > 0) && (distance < (best_distance))) {
						closest_object = index;		// <= array index of object
						best_distance = distance;	// <= closest distance value
						
					}
					
				}
				
			}
			
			// Object intersection detected
			if((best_distance > 0) && (best_distance != INFINITY)) {
 			    double new_ro[3] = {0, 0, 0};
				double new_rd[3] = {0, 0, 0};
				double normal[3] = {0, 0, 0};
				double diffuse_color[3] = {0, 0, 0};
				double specular_color[3] = {0, 0, 0};

				double reflection_vector[3] = {0, 0, 0};
				int index2;
				double distance2 = 0.0, best_distance2 = 0.0, light_distance = 0.0;

				
				
 				// Establish orgin for the new ray
				vector_scale(rd, best_distance, new_ro);
				vector_add(ro, new_ro, new_ro);
				
				// Iterate through light objects
				for(index = 0; index < num_objects; index++) {
					if(strcmp(objects[index].type, "light") == 0) {
						
						// Calcuate new ray direction
						vector_subtract(objects[index].properties.light.position, new_ro, new_rd);
						light_distance = vector_length(new_rd);
						normalize(new_rd);	//<= Normalize new ray direction
						
						best_distance2 = INFINITY;
						
						// Execute intersection test
						for(index2 = 0; index2 < num_objects; index2++) {
							distance2 = 0;	// <= reset distance each iteration
							
							if(closest_object != index2) {				// <= prevent self intersecting
								if((objects[index2].type) != NULL) { 	// <= Check against type nulls
									if(strcmp((objects[index2].type), "sphere") == 0) {
										distance2 = sphere_intersection(new_ro, new_rd, objects[index2].properties.sphere.position, objects[index2].properties.sphere.radius);
									
									} else if(strcmp((objects[index2].type), "plane") == 0) {
										distance2 = plane_intersection(new_ro, new_rd, objects[index2].properties.plane.position, objects[index2].properties.plane.normal);
								
									}
									
									if(distance2 <= light_distance) {
										if ((distance2 > 0) && (distance2 < (best_distance2))) {
											best_distance2 = distance2;	// <= closest distance value
											
										}
										
									}
									
								}
								
							}
							
						} // EoObject iteration loop						
						
						//
						if(best_distance2 == INFINITY) {
							if((objects[closest_object].type) != NULL) {
								if(strcmp((objects[closest_object].type), "sphere") == 0) {
									vector_copy((objects[closest_object].properties.sphere.position), normal);
									vector_subtract(new_ro, objects[closest_object].properties.sphere.position, normal);
									vector_copy((objects[closest_object].properties.sphere.diffuse_color), diffuse_color);
									vector_copy((objects[closest_object].properties.sphere.specular_color), specular_color);
									
								} else if(strcmp((objects[closest_object].type), "plane") == 0) {
									vector_copy((objects[closest_object].properties.plane.normal), normal);
									vector_copy((objects[closest_object].properties.plane.diffuse_color), diffuse_color);
									vector_copy((objects[closest_object].properties.plane.specular_color), specular_color);									

								}
								
							} 

							normalize(normal); //<= Normalize normal
							normalize(new_rd); //<= Normalize new ray direction
							vector_reflection(new_rd, normal, reflection_vector);
							
							double diffuse_out[3] = {0, 0, 0};
							double specular_out[3] = {0, 0, 0};							
							
							diffuse_reflection(normal, new_rd, (objects[index].properties.light.color), diffuse_color, diffuse_out);
							specular_highlight(normal, new_rd, reflection_vector, rd, specular_color, (objects[index].properties.light.color), specular_out);
							
							double fang_out = 0.0, 
							double frad_out = 0.0;
							
							fang_out = fang((objects[index].properties.light.radial_a0), (objects[index].properties.light.theta), (objects[index].properties.light.direction), new_rd); 
							frad_out = frad((objects[index].properties.light.radial_a0), (objects[index].properties.light.radial_a1), (objects[index].properties.light.radial_a2), light_distance);
							
							pixel_coloring[0] = fang_out * frad_out * (diffuse_out[0] + specular_out[0]);
							pixel_coloring[1] = fang_out * frad_out * (diffuse_out[1] + specular_out[1]);
							pixel_coloring[2] = fang_out * frad_out * (diffuse_out[2] + specular_out[2]);
							
						}
						
					}
			
				}
				
				// Apply coloring to a pixel
				if(strcmp(objects[(closest_object)].type, "sphere") == 0) {
					red = clamp(pixel_coloring[0], 0, 1) * (image->max_color);
					green = clamp(pixel_coloring[1], 0, 1) * (image->max_color);
					blue = clamp(pixel_coloring[2], 0, 1) * (image->max_color);
					
					image->image_data[(image->width) * row + column].red = red;
					image->image_data[(image->width) * row + column].green = green;
					image->image_data[(image->width) * row + column].blue = blue;

				} else if(strcmp(objects[(closest_object)].type, "plane") == 0) {
					red = clamp(pixel_coloring[0], 0, 1) * (image->max_color);
					green = clamp(pixel_coloring[1], 0, 1) * (image->max_color);
					blue = clamp(pixel_coloring[2], 0, 1) * (image->max_color);
					
					image->image_data[(image->width) * row + column].red = red;
					image->image_data[(image->width) * row + column].green = green;
					image->image_data[(image->width) * row + column].blue = blue;
					
				}
				
			}
			
		} // EoRow Loop
		
	} // EoColumn Loop 

	return image;
	
}