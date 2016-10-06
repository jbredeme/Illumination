/**
 * Author: Jarid Bredemeier
 * Email: jpb64@nau.edu
 * Date: Thursday, October 6, 2016
 * File: json.h
 * Copyright © 2016 All rights reserved 
 */
 
#ifndef json_h
#define json_h

/**
 * Camera
 *
 * @description stores values for height and width properties of an camera
 * object
 */
typedef struct Camera {
	double width;
	double height;
	
} Camera;


/**
 * Plane
 *
 * @description stores three arrays two of which represent a plane's position, and normal vector
 * in Euclidean space (x, y, z). The color array represents the 3 byte color channel of RGB
 */
typedef struct Plane {
	double color[3];
	double position[3];
	double normal[3];
	
} Plane;


/**
 * Sphere
 *
 * @description stores two arrays one of which represent a Sphere's position in Euclidean space (x, y, z).
 * The color array represents the 3 byte color channel of RGB. And finally the radius of the sphere.
 */
typedef struct Sphere {
	double color[3];
	double position[3];
	double radius;
	
} Sphere;


/**
 * Object
 *
 * @description stores a character pointer to a string that represents the name of the type. Object also
 * unions Camera, Plane, and Sphere typedef as part of larger collection of structures. The ordering of
 * of properties in Sphere and Plane for example mimic a condition known as polymorphism where the space
 * for color[3] is not allocated twice but just once however, the reference to the different kind of structures
 * allows for differentiation.
 */
typedef struct Object {
	char *type;
	
	union properties {
		Camera camera;
		Plane plane;
		Sphere sphere;
		
	} properties;

} Object;

// function declarations
int json_read_scene(FILE *fpointer, Object objects[]);
 
#endif