/**
 * Author: Jarid Bredemeier
 * Email: jpb64@nau.edu
 * Date: Thursday, October 6, 2016
 * File: json.c
 * Copyright © 2016 All rights reserved 
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "json.h"

// Line number for error checking purposes
int line_num;

/**
 * @param file pointer
 * @returns interger value of the ascii character read in
 * @description Reads in a character from an input stream, checks if the character is a newline, 
 * carriage return, or linefeed and adds 1 to the line number counter (line_num).
 * If an end-of-file is encountered, prompt the user exit program.
 */
int get_char(FILE *fpointer) {
	int token = getc(fpointer);
	
	if((token == '\n') || (token == '\r') || (token == '\f')) {
		line_num = line_num + 1;
		
	} else if(token == EOF) {
		fprintf(stderr, "Error, line number %d; unexpected end-of-file.\n", line_num);
		// Close file stream flush all buffers
		fclose(fpointer);
		exit(-1);
		
	}
	
	return token;
	
} 
 
 
/**
 * @param file pointer
 * @returns void
 * @description Reads in a character from an input stream, checks if the character is a newline, 
 * carriage return, or linefeed and adds 1 to the line number counter (line_num).
 * If an end-of-file is encountered, prompt the user exit program.
 */
void skip_whitespace(FILE *fpointer) {
	int token = get_char(fpointer);
	
	while(isspace(token) != 0){
		token = get_char(fpointer);
		
	}
	
	ungetc(token, fpointer);
	
}

 
/**
 * @param file pointer
 * @returns string of characters delimited by "... "
 * @description Reads in a stream of characters delimited by quotation marks. Validates against the existence
 * of escape sequence codes, strings longer then 256 characters, and non-ascii characters. 
 */
char *get_string(FILE *fpointer){
	char buffer[256];
	int token, i = 0;
	// Read in character advance the stream position indicator
	token = get_char(fpointer);
	
	if(token != '"') {
		fprintf(stderr, "Error, line number %d; unexpected character '%c', expected character '%c'.\n", line_num, token, '"');
		// Close file stream flush all buffers
		fclose(fpointer);		
		exit(-1);
		
	} else {
		// Read in character advance the stream position indicator
		token = get_char(fpointer);
		
		while(token != '"'){
			 // String exceeds the buffer size
			if(i > 256) {
				fprintf(stderr, "Error, line number %d; Strings with a length greater than 256 characters are not supported.\n", line_num);
				// Close file stream flush all buffers
				fclose(fpointer);
				exit(-1);	
				
			}
			// String contains escape sequence code(s)
			if(token == '\\') {
				fprintf(stderr, "Error, line number %d; Strings with escape character codes are not supported.\n", line_num);
				// Close file stream flush all buffers
				fclose(fpointer);
				exit(-1);
				
			}
			// String is not an ascii character
			if((token < 32) || (token > 126)) {
				fprintf(stderr, "Error, line number %d; Strings can contain ascii characters only.\n", line_num);
				// Close file stream flush all buffers
				fclose(fpointer);
				exit(-1);	
				
			}
			// Add character to the buffer
			buffer[i] = token;
			i = i + 1;
			
			// Read in character advance the stream position indicator
			token = get_char(fpointer);
			 
		}
		
	}
	 
	buffer[i] = 0;
	return strdup(buffer);
	
 }
 
 
/**
 * @param file pointer
 * @returns double
 * @description Reads in a double precsion floating point number, if none are found throws error
 * exits the program. 
 */
double get_double(FILE *fpointer){
	 double dbl;
	 
	 if(fscanf(fpointer, "%lf", &dbl) == 0) {
		fprintf(stderr, "Error, line number %d; expected numeric value.\n", line_num);
		// Close file stream flush all buffers
		fclose(fpointer);
		exit(-1);		
		
	 } else {
		 return dbl;
		 
	 }
	 
 }
 
 
/**
 * @param file pointer
 * @returns double[3]
 * @description Reads in an array with the format pattern [x, y, z] and parses into an array of	
 * doubles.
 */
double *get_vector(FILE *fpointer){
	// Allocate memory for vector array of doubles
	double *vector = malloc(3 * sizeof(double));

	int token;
	
	token = get_char(fpointer);
	
	if(token != '[') {
		fprintf(stderr, "Error, line number %d; error reading in vector. Unexpected character '%c', expected character '%c'.\n", line_num, token, '[');
		// Close file stream flush all buffers
		fclose(fpointer);		
		exit(-1);
		
	}	
	skip_whitespace(fpointer);
	
	vector[0] = get_double(fpointer);
	
	skip_whitespace(fpointer);
	
	token = get_char(fpointer);
	
	if(token != ',') {
		fprintf(stderr, "Error, line number %d; error reading in vector. Unexpected character '%c', expected character '%c'.\n", line_num, token, ',');
		// Close file stream flush all buffers
		fclose(fpointer);		
		exit(-1);
		
	}
	skip_whitespace(fpointer);
	
	vector[1] = get_double(fpointer);
	
	skip_whitespace(fpointer);
	
	token = get_char(fpointer);
	
	if(token != ',') {
		fprintf(stderr, "Error, line number %d; unexpected character '%c', expected character '%c'.\n", line_num, token, ',');
		// Close file stream flush all buffers
		fclose(fpointer);		
		exit(-1);
		
	}
	skip_whitespace(fpointer);
	
	vector[2] = get_double(fpointer);
	
	skip_whitespace(fpointer);

	token = get_char(fpointer);
	
	if(token != ']') {
		fprintf(stderr, "Error, line number %d; unexpected character '%c', expected character '%c'.\n", line_num, token, ']');
		// Close file stream flush all buffers
		fclose(fpointer);		
		exit(-1);
		
	}		
	
	return vector;
	
 }

 
/**
 * @param color_v - an array of 3 double precsion numbers 
 * @returns 0 if an element is not within the acceptable tolerances 0 to 1.0 and 1 otherwise
 * @description check if color value is within the acceptable tolerances 0 to 1.0
 */
 int color_tolerance(double color_v[]){
	int index;
	 
	for(index = 0; index < 3; index++) {
		if((color_v[index] < 0) || (color_v[index] > 1.0)) {
			return (0);
			
		}

	}

	return (1);
	
 }

 
/**
 * @param file pointer
 * @param array of Object
 * @returns integer number of item read-in
 * @description reads in a scene of objects formatted using JavaScript Object Notation (JSON)
 * - Accepts [ empty scene ]
 * - Accepts { empty objects }
 * - Accepts comma and non-comma separated objects blocks
 * - Accepts comma and non-comma separated name:value pairs
 * - Whitespace insensitive
 */ 
int json_read_scene(FILE *fpointer, Object objects[]) {
	int token, index;
	double *vector;
	char *name, *value;
	
	index = 0;
	
	// Skip whitespace(s) read in the first character
	skip_whitespace(fpointer);
	token = get_char(fpointer);
	
	// Check to see of the first character is an opening brace that denotes the start of a scene
	if(token != '[') {
		fprintf(stderr, "Error, line number %d; invalid scene definition '%c', expected character '%c'.\n", line_num, token, '[');
		// Close file stream flush all buffers
		fclose(fpointer);		
		exit(-2);
		
	}
	
	// Skip whitespace(s), read in the next character and advance the stream position indicator
	skip_whitespace(fpointer);
	token = get_char(fpointer);
		
	// Empty scene not detected! This is the Outter-Loop where we iteriate through a scene looking
	// for object defintions until a closing ']' brace is encountered denoting the end of a scene
	while(token != ']') {

		// Determine if the character read in is a valid begining of an object
		if(token != '{') {
			fprintf(stderr, "Error, line number %d; invalid object definition '%c', expected character '%c'.\n", line_num, token, '{');
			// Close file stream flush all buffers
			fclose(fpointer);		
			exit(-2);
			
		}
		
		// Skip whitespace(s), read in the next character and advance the stream position indicator
		skip_whitespace(fpointer);
		token = get_char(fpointer);
		
		// Inner-Loop: Searching for name/value pairs within each object definition util object
		// terminator has been reached
		while(token != '}') {
			// If the next character is a '"' which means a string move indicator back one position then read in the string
			if(token == '"') {
				ungetc(token, fpointer);			

			}
			
			name = get_string(fpointer);
			
			if(strcmp(name, "type") == 0){
				// Skip whitespace(s), read in the next character
				// and advance the stream position indicator
				skip_whitespace(fpointer);
				token = get_char(fpointer);
				
				if(token != ':') {
					fprintf(stderr, "Error, line number %d; invalid separator '%c', expected character '%c'.\n", line_num, token, ':');
					// Close file stream flush all buffers
					fclose(fpointer);		
					exit(-3);
					
				} else {
					skip_whitespace(fpointer);
					value = get_string(fpointer);
					objects[index].type = value;
					
				}
	   
			} else if(strcmp(name, "width") == 0) {
				skip_whitespace(fpointer);
				token = get_char(fpointer);

				if(token != ':') {
					fprintf(stderr, "Error, line number %d; invalid separator '%c', expected character '%c'.\n", line_num, token, ':');
					// Close file stream flush all buffers
					fclose(fpointer);		
					exit(-3);
					
				} else {
					skip_whitespace(fpointer);
					objects[index].properties.camera.width = get_double(fpointer);
					
				}
				
			} else if(strcmp(name, "height") == 0) {
				// Skip whitespace(s), read in the next character and advance the stream position indicator
				skip_whitespace(fpointer);
				token = get_char(fpointer);

				if(token != ':') {
					fprintf(stderr, "Error, line number %d; invalid separator '%c', expected character '%c'.\n", line_num, token, ':');
					// Close file stream flush all buffers
					fclose(fpointer);		
					exit(-3);
					
				} else {
					skip_whitespace(fpointer);
					objects[index].properties.camera.height = get_double(fpointer);
					
				}
				
			} else if(strcmp(name, "radius") == 0) {
				// Skip whitespace(s), read in the next character and advance the stream position indicator
				skip_whitespace(fpointer);
				token = get_char(fpointer);

				if(token != ':') {
					fprintf(stderr, "Error, line number %d; invalid separator '%c', expected character '%c'.\n", line_num, token, ':');
					// Close file stream flush all buffers
					fclose(fpointer);		
					exit(-1);
					
				} else {
					skip_whitespace(fpointer);
					objects[index].properties.sphere.radius = get_double(fpointer);
					
				}
			
			} else if(strcmp(name, "radial-a0") == 0) {
				// Skip whitespace(s), read in the next character and advance the stream position indicator
				skip_whitespace(fpointer);
				token = get_char(fpointer);

				if(token != ':') {
					fprintf(stderr, "Error, line number %d; invalid separator '%c', expected character '%c'.\n", line_num, token, ':');
					// Close file stream flush all buffers
					fclose(fpointer);		
					exit(-1);
					
				} else {
					skip_whitespace(fpointer);
					objects[index].properties.light.radial_a0 = get_double(fpointer);
					
				}
			
			} else if(strcmp(name, "radial-a1") == 0) {
				// Skip whitespace(s), read in the next character and advance the stream position indicator
				skip_whitespace(fpointer);
				token = get_char(fpointer);

				if(token != ':') {
					fprintf(stderr, "Error, line number %d; invalid separator '%c', expected character '%c'.\n", line_num, token, ':');
					// Close file stream flush all buffers
					fclose(fpointer);		
					exit(-1);
					
				} else {
					skip_whitespace(fpointer);
					objects[index].properties.light.radial_a1 = get_double(fpointer);
					
				}
			
			} else if(strcmp(name, "radial-a2") == 0) {
				// Skip whitespace(s), read in the next character and advance the stream position indicator
				skip_whitespace(fpointer);
				token = get_char(fpointer);

				if(token != ':') {
					fprintf(stderr, "Error, line number %d; invalid separator '%c', expected character '%c'.\n", line_num, token, ':');
					// Close file stream flush all buffers
					fclose(fpointer);		
					exit(-1);
					
				} else {
					skip_whitespace(fpointer);
					objects[index].properties.light.radial_a2 = get_double(fpointer);
					
				}
			
			} else if(strcmp(name, "angular-a0") == 0) {
				// Skip whitespace(s), read in the next character and advance the stream position indicator
				skip_whitespace(fpointer);
				token = get_char(fpointer);

				if(token != ':') {
					fprintf(stderr, "Error, line number %d; invalid separator '%c', expected character '%c'.\n", line_num, token, ':');
					// Close file stream flush all buffers
					fclose(fpointer);		
					exit(-1);
					
				} else {
					skip_whitespace(fpointer);
					objects[index].properties.light.angular_a0 = get_double(fpointer);
					
				}
			
			} else if(strcmp(name, "diffuse_color") == 0) {
				// Skip whitespace(s), read in the next character and advance the stream position indicator
				skip_whitespace(fpointer);
				token = get_char(fpointer);

				if(token != ':') {
					fprintf(stderr, "Error, line number %d; unexpected character '%c', expected character '%c'.\n", line_num, token, ':');
					// Close file stream flush all buffers
					fclose(fpointer);		
					exit(-1);
					
				} else {
					skip_whitespace(fpointer);
					vector = get_vector(fpointer);
					
					// Validates against object defintions without a type defined. That is all 
					// objects and object properties associated to a type value of NULL are ignored
					if(objects[index].type != NULL) {

						// Check color tolerance range of 0 to 1.0
						if(color_tolerance(vector) != 1) {
							fprintf(stderr, "Error, invalid color tolerance in %s color array.\n", objects[index].type);
							// Close file stream flush all buffers
							fclose(fpointer);		
							exit(-1);
							
						}
						
						if(strcmp(objects[index].type, "sphere") == 0) {
							objects[index].properties.sphere.diffuse_color[0] = vector[0];
							objects[index].properties.sphere.diffuse_color[1] = vector[1];
							objects[index].properties.sphere.diffuse_color[2] = vector[2];
							
						} else if (strcmp(objects[index].type, "plane") == 0) {
							objects[index].properties.plane.diffuse_color[0] = vector[0];
							objects[index].properties.plane.diffuse_color[1] = vector[1];
							objects[index].properties.plane.diffuse_color[2] = vector[2];
							
						}
						
					}
					
				}	 
			   
			} else if(strcmp(name, "specular_color") == 0) {
				// Skip whitespace(s), read in the next character and advance the stream position indicator
				skip_whitespace(fpointer);
				token = get_char(fpointer);

				if(token != ':') {
					fprintf(stderr, "Error, line number %d; unexpected character '%c', expected character '%c'.\n", line_num, token, ':');
					// Close file stream flush all buffers
					fclose(fpointer);		
					exit(-1);
					
				} else {
					skip_whitespace(fpointer);
					vector = get_vector(fpointer);
					
					// Validates against object defintions without a type defined. That is all 
					// objects and object properties associated to a type value of NULL are ignored
					if(objects[index].type != NULL) {

						// Check color tolerance range of 0 to 1.0
						if(color_tolerance(vector) != 1) {
							fprintf(stderr, "Error, invalid color tolerance in %s color array.\n", objects[index].type);
							// Close file stream flush all buffers
							fclose(fpointer);		
							exit(-1);
							
						}
						
						if(strcmp(objects[index].type, "sphere") == 0) {
							objects[index].properties.sphere.specular_color[0] = vector[0];
							objects[index].properties.sphere.specular_color[1] = vector[1];
							objects[index].properties.sphere.specular_color[2] = vector[2];
							
						} else if (strcmp(objects[index].type, "plane") == 0) {
							objects[index].properties.plane.specular_color[0] = vector[0];
							objects[index].properties.plane.specular_color[1] = vector[1];
							objects[index].properties.plane.specular_color[2] = vector[2];
							
						}
						
					}
					
				}	 
			   
			} else if(strcmp(name, "color") == 0) {
				// Skip whitespace(s), read in the next character and advance the stream position indicator
				skip_whitespace(fpointer);
				token = get_char(fpointer);

				if(token != ':') {
					fprintf(stderr, "Error, line number %d; invalid separator '%c', expected character '%c'.\n", line_num, token, ':');
					// Close file stream flush all buffers
					fclose(fpointer);		
					exit(-1);
					
				} else {
					skip_whitespace(fpointer);
					vector = get_vector(fpointer);
					
					// Validates against object defintions without a type defined. That is all 
					// objects and object properties associated to a type value of NULL are ignored
					if(objects[index].type != NULL) {
						
						if(strcmp(objects[index].type, "light") == 0) {
							objects[index].properties.light.color[0] = vector[0];
							objects[index].properties.light.color[1] = vector[1];
							objects[index].properties.light.color[2] = vector[2];	
						
						} else {
							// Check color tolerance range of 0 to 1.0
							if(color_tolerance(vector) != 1) {
								fprintf(stderr, "Error, invalid color tolerance in %s color array.\n", objects[index].type);
								// Close file stream flush all buffers
								fclose(fpointer);		
								exit(-1);
								
							}
							
							if(strcmp(objects[index].type, "sphere") == 0) {
								objects[index].properties.sphere.color[0] = vector[0];
								objects[index].properties.sphere.color[1] = vector[1];
								objects[index].properties.sphere.color[2] = vector[2];
								
							} else if (strcmp(objects[index].type, "plane") == 0) {
								objects[index].properties.plane.color[0] = vector[0];
								objects[index].properties.plane.color[1] = vector[1];
								objects[index].properties.plane.color[2] = vector[2];
								
							}
							
						}
						
					}
			
				}				
				
			} else if(strcmp(name, "position") == 0) {
				// Skip whitespace(s), read in the next character and advance the stream position indicator
				skip_whitespace(fpointer);
				token = get_char(fpointer);

				if(token != ':') {
					fprintf(stderr, "Error, line number %d; invalid separator '%c', expected character '%c'.\n", line_num, token, ':');
					// Close file stream flush all buffers
					fclose(fpointer);		
					exit(-1);
					
				} else {
					skip_whitespace(fpointer);
					vector = get_vector(fpointer);
					
					// Validates against object defintions without a type defined. That is all 
					// objects and object properties associated to a type value of NULL are ignored
					if(objects[index].type != NULL){
						if(strcmp(objects[index].type, "sphere") == 0) {
							objects[index].properties.sphere.position[0] = vector[0];
							objects[index].properties.sphere.position[1] = vector[1];
							objects[index].properties.sphere.position[2] = vector[2];					
							
						} else if(strcmp(objects[index].type, "plane") == 0) {
							objects[index].properties.plane.position[0] = vector[0];
							objects[index].properties.plane.position[1] = vector[1];
							objects[index].properties.plane.position[2] = vector[2];
							
						} else if(strcmp(objects[index].type, "light") == 0) {
							objects[index].properties.light.position[0] = vector[0];
							objects[index].properties.light.position[1] = vector[1];
							objects[index].properties.light.position[2] = vector[2];
							
						}
						
					}
					
				}				
				
			} else if(strcmp(name, "normal") == 0) {
				// Skip whitespace(s), read in the next character and advance the stream position indicator
				skip_whitespace(fpointer);
				token = get_char(fpointer);

				if(token != ':') {
					fprintf(stderr, "Error, line number %d; unexpected character '%c', expected character '%c'.\n", line_num, token, ':');
					// Close file stream flush all buffers
					fclose(fpointer);		
					exit(-1);
					
				} else {
					skip_whitespace(fpointer);
					vector = get_vector(fpointer);
					
					objects[index].properties.plane.normal[0] = vector[0];
					objects[index].properties.plane.normal[1] = vector[1];
					objects[index].properties.plane.normal[2] = vector[2];
					
				}	 
			   
			} else if(strcmp(name, "direction") == 0) {
				// Skip whitespace(s), read in the next character and advance the stream position indicator
				skip_whitespace(fpointer);
				token = get_char(fpointer);

				if(token != ':') {
					fprintf(stderr, "Error, line number %d; unexpected character '%c', expected character '%c'.\n", line_num, token, ':');
					// Close file stream flush all buffers
					fclose(fpointer);		
					exit(-1);
					
				} else {
					skip_whitespace(fpointer);
					vector = get_vector(fpointer);
					
					objects[index].properties.light.direction[0] = vector[0];
					objects[index].properties.light.direction[1] = vector[1];
					objects[index].properties.light.direction[2] = vector[2];
					
				}	 
			   
			} else {
				fprintf(stderr, "Error, line number %d; invalid type '%s'.\n", name);
				// Close file stream flush all buffers
				fclose(fpointer);		
				exit(-1);				
			}
			
			// Skip whitespace(s), read in the next character and advance the stream position indicator
			skip_whitespace(fpointer);
			token = get_char(fpointer);
			
			if(token == ',') {
				// Skip whitespace(s), read in the next character and advance the stream position indicator
				skip_whitespace(fpointer);
				token = get_char(fpointer);
				
			}
			
		}  // End-of-While-Loop: Name:Value pairing
		
		// Skip whitespace(s), read in the next character and advance the stream position indicator
		skip_whitespace(fpointer);
		token = get_char(fpointer);
		
		if(token == ',') {
			// Skip whitespace(s), read in the next character and advance the stream position indicator
			skip_whitespace(fpointer);
			token = get_char(fpointer);
			
		}
		
		// Increment object counter
		index = index + 1;
	
	} // End-of-While-Loop: Object defintions

	// Return the total number of objects read-in from the scene
	return index;

}