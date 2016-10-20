/**
 * Author: Jarid Bredemeier
 * Email: jpb64@nau.edu
 * Date: Thursday, October 6, 2016
 * File: json.h
 * Copyright © 2016 All rights reserved 
 */
 
#ifndef json_h
#define json_h

#define MAX_OBJECTS 128
#define MAX_COLOR 255

/**
 * Stores values for height and width properties of an camera
 * object
 */
typedef struct Camera {
	double width;
	double height;
	
} Camera;


/**
 * Stores three arrays two of which represent a plane's position, and normal vector
 * in Euclidean space (x, y, z). The color array represents the 3 byte color channel of RGB
 */
typedef struct Plane {
	double color[3];
	double position[3];
	double diffuse_color[3];
	double specular_color[3];		
	double normal[3];
	
} Plane;


/**
 * Stores two arrays one of which represent a Sphere's position in Euclidean space (x, y, z).
 * The color array represents the 3 byte color channel of RGB. And finally the radius of the sphere.
 */
typedef struct Sphere {
	double color[3];
	double position[3];
	double diffuse_color[3];
	double specular_color[3];	
	double radius;
	
} Sphere;


/**
 * Stores three arrays one of which represent a Light's position in Euclidean space (x, y, z).
 * The color array represents the 3 byte color channel of RGB, and the direction array represents
 * a direction vector of spotlights. Additionally there is a theta value used to store angle 
 * information in radians from the direction vector, and radial and angular values used in 
 * calculating radial and angular attunation.
 */
typedef struct Light {
	double color[3];
	double position[3];
	double direction[3];
	double theta;
	double radial_a0;
	double radial_a1;
	double radial_a2;
	double angular_a0;
	
} Light;


/**
 * Stores a character pointer to a string that represents the name of the type. Object also
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
		Light light;
		
	} properties;

} Object;

// function declarations
int json_read_scene(FILE *fpointer, Object objects[]);
 
#endif