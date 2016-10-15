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
 * @param TO - DO
 * @param TO - DO
 * @param TO - DO
 * @param TO - DO
 * @returns TODO 
 * @description TODO
 */
void specular_highlight(double *normal, 
						double *light_pos, 
						double *light_color, 
						double *obj_specular_color, 
						double *reflection_of_light,
						double *rd,
						double *vector){
	//incident ray, reflected ray
	double scalar1, scalar2;
	
	scalar1 = vector_dot_product(normal, light_pos);
	scalar2 = vector_dot_product(rd, reflection_of_light);
	
	if((scalar1 > 0) && (scalar2 > 0)){
        vector[0] = scalar2 * light_color[0] * obj_specular_color[0];
        vector[1] = scalar2 * light_color[1] * obj_specular_color[1];
        vector[2] = scalar2 * light_color[2] * obj_specular_color[2];			
	
	} else {
        vector[0] = 0;
        vector[1] = 0;
        vector[2] = 0;
		
	}
	
}


/**
 * @param TO - DO
 * @param TO - DO
 * @param TO - DO
 * @param TO - DO
 * @returns TODO 
 * @description 
 */
void diffuse_reflection(double *normal, double *light_pos, double *light_color, double *obj_diffuse_color, double *vector){
	double scalar;

	scalar = vector_dot_product(normal, light_pos);
	
	if(scalar > 0) {
		vector[0] = scalar * obj_diffuse_color[0] * light_color[0];
		vector[1] = scalar * obj_diffuse_color[1] * light_color[1];
		vector[2] = scalar * obj_diffuse_color[2] * light_color[2];

	} else {
        vector[0] = 0;
        vector[1] = 0;
        vector[2] = 0;		
		
	}
	
}


/**
 * @param TO - DO
 * @param TO - DO
 * @param TO - DO
 * @param TO - DO
 * @returns TODO 
 * @description TODO
 */
double fang(){
	//TODO	
}


/**
 * @param a0 - TODO
 * @param a1 - TODO 
 * @param a2 - TODO
 * @param dl - TODO 
 * @returns TODO 
 * @description TODO
 */
double frad(double a0, double a1, double a2, double dl){
	//TODO

}

/**
 * @param - TODO
 * @param - TODO 
 * @param - TODO 
 * @returns TODO 
 * @description TODO
 */
double clamp(double number, double min, double max){
	if(number > max) {
		return max;
		
	} else if (number < min){
		return min;
		
	} else {
		return number;
		
	}
}


/**
 * @param v - double precision floating point number
 * @returns static function, no return
 * @description math operation that squares a double precision floating
 */
static inline double sqr(double v) {
	return v * v;
	
}


/**
 * @param v - an array storing vector information
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
 * @param objects - an array of objects
 * @param num_objects - used to set the counter that iterates through an array of objects 
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
 * @param ro - ray vector orgin
 * @param rd - ray vector direction
 * @param center - sphere center aka position
 * @param radius - sphere radius
 * @returns double percision float t value that represents length of the intersecting vector, and -1 if no intersection was detected.
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
 * @param ro - ray vector orgin
 * @param rd - ray vector direction
 * @param pos - position
 * @param normal - the orthogonal normal vector to the plane
 * @returns double percision float t value that represents length of the intersecting vector, and -1 if no intersection was detected.
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
 * @param objects - collection of objects read in from the json parser
 * @param image - is an Image object used to store image data
 * @param num_objects - number of objects read in from the json parser use to set iteration values for, for loops.
 * @returns Image - which is the image pointer to the image object that is used to store the image data for write purposes.
 * @description this function implements the raycasting portion of this application it performs the calculations for pixel scaling, 
 * and logic that uses the scene data to detect object ray intersections, colors pixels related to the object data, and stores the 
 * collection of information into an image data buffer to be written using a ppm write function.
 */
Image* raycaster(Object objects[], Image *image, int num_objects) {
	double pixel_height, pixel_width;
	double cx, cy;
	double h, w;
	double t, best_t;
	double red, green, blue;
	int row, column, index, closest_obj;
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
					
					} else if(strcmp((objects[index].type), "plane") == 0) {
						t = plane_intersection(ro, rd, objects[index].properties.plane.position, objects[index].properties.plane.normal);
				
					}
					
					// Get the best t value and object index
					if ((t > 0) && (t < best_t)){
						best_t = t;
						closest_obj = index;
					
					}
					
				}
				
				
			} // EoObject iteration loop
			
			if((best_t > 0) && (best_t != INFINITY)){
				
				if(strcmp(objects[closest_obj].type, "sphere") == 0) {
					red = objects[closest_obj].properties.sphere.color[0] * (image->max_color);
					image->image_data[(image->width) * row + column].red = red;
					
					green = objects[closest_obj].properties.sphere.color[1] * (image->max_color);
					image->image_data[(image->width) * row + column].green = green;
					
					blue = objects[closest_obj].properties.sphere.color[2] * (image->max_color);
					image->image_data[(image->width) * row + column].blue = blue;
				
				} else if(strcmp(objects[closest_obj].type, "plane") == 0){
					red = objects[closest_obj].properties.plane.color[0] * (image->max_color);
					image->image_data[(image->width) * row + column].red = red;
					
					green = objects[closest_obj].properties.plane.color[1] * (image->max_color);
					image->image_data[(image->width) * row + column].green = green;
					
					blue = objects[closest_obj].properties.plane.color[2] * (image->max_color);
					image->image_data[(image->width) * row + column].blue = blue;
					
				}
				
				/*   We have the closest object..

					double* color = malloc(sizeof(double)*3);
					color[0] = 0; // ambient_color[0];
					color[1] = 0; // ambient_color[1];
					color[2] = 0; // ambient_color[2];

					for (int j=0; light[j] != NULL; j+=1) {
					  // Shadow test
					  Ron = closest_t * Rd + Ro;
					  Rdn = light_position - Ron;
					  closest_shadow_object = ...;
					  for (int k=0; object[k] != NULL; k+=1) {
					if (object[k] == closest_object) continue;
					// 
					switch(...) {
					case SPHERE:
					  t = sphere_intersect(..);
					  break;
					case PLANE:
					  t = plane_intersect(..);
					  break;
					default:
					  // ERROR
					  break;
					}
					if (best_t > distance_to_light) {
					  continue;
					}
					  }
					  if (closest_shadow_object == NULL) {
					// N, L, R, V
					N = closest_object->normal; // plane
					N = Ron - closest_object->center; // sphere
					L = Rdn; // light_position - Ron;
					R = reflection of L;
					V = Rd;
					diffuse = ...; // uses object's diffuse color
					specular = ...; // uses object's specular color
					color[0] += frad() * fang() * (diffuse + specular);
					color[1] += frad() * fang() * (diffuse + specular);
					color[2] += frad() * fang() * (diffuse + specular);
					  }
					}
					// The color has now been calculated

					buffer[...].r = (unsigned char)(255 * clamp(color[0]));
					buffer[...].g = (unsigned char)(255 * clamp(color[1]);
					buffer[...].b = (unsigned char)(255 * clamp(color[2]);	 */			
				

				

			
				// 1.) Calculate new ray orgin
				// 2.) Iterate over lights
				// 3.) Calculate new ray direction
				// 4.) Shadow test, where you do not apply the lights

				//int i, j;
				double new_ray_orgin[3];
				double new_ray_direction[3];
				
				vector_scale(rd, best_t, new_ray_direction);
				vector_add(new_ray_direction, ro, new_ray_direction);
				
				// Can you see the light source?
				for(index = 0; index < num_objects; index++) {
					if((objects[index].type) != NULL) {
						if(strcmp(objects[index].type, "light") == 0){
							// Check light type
							
						
						}
						
					}
					
				}
				
				
				
				
				
				
			}
			
		} // EoRow Loop
		
	} // EoColumn Loop 

	return image;
	
}