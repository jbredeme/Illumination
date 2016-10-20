/**
 * Author: Jarid Bredemeier
 * Email: jpb64@nau.edu
 * Date: Thursday, October 6, 2016
 * File: vector_math.h
 * Copyright © 2016 All rights reserved 
 */
 
#ifndef vector_math_h
#define vector_math_h

/**
 * Takes a vector of any length while maintaining its direction change its length to 
 * 1, turning it into a unit vector.
 *
 * @param vector_a - single dimensional array of three double precision numbers
 */
static inline void normalize(double *vector_a) {
	double len = sqrt(pow(vector_a[0], 2) + pow(vector_a[1], 2) + pow(vector_a[2], 2));
	vector_a[0] /= len;
	vector_a[1] /= len;
	vector_a[2] /= len;
	
}


/**
 * Adds two three dimensional vectors to one another and stores the result in the third
 * vector. Vector addition results in the tail of the second vector being projected onto
 * the head of the first vector, and the resulting vector connects the head of the second
 * vector with the tail of the first. Triangle Method
 * 
 * @param vector_a - an array representing a vector in Euclidean space
 * @param vector_b - an array representing a vector in Euclidean space
 * @param vector_c - vector used to store the computational result
 */
static inline void vector_add(double *vector_a, double *vector_b, double *vector_c) {
  vector_c[0] = vector_a[0] + vector_b[0];
  vector_c[1] = vector_a[1] + vector_b[1];
  vector_c[2] = vector_a[2] + vector_b[2];
  
}


/**
 * Preforms subtraction on two three dimensional vectors and stores the result in the third
 * vector. Vector subtraction is just vector addition with changing the direction of one of
 * the vectors.
 * 
 * @param vector_a - an array representing a vector in Euclidean space
 * @param vector_b - an array representing a vector in Euclidean space
 * @param vector_c - vector used to store the computational result
 */
static inline void vector_subtract(double *vector_a, double *vector_b, double *vector_c) {
  vector_c[0] = vector_a[0] - vector_b[0];
  vector_c[1] = vector_a[1] - vector_b[1];
  vector_c[2] = vector_a[2] - vector_b[2];
  
}


/**
 * Preforms scaling on two three dimensional vectors and stores the result in the third
 * vector. Vector scaling results in changing the vectors magnitude.
 * 
 * @param vector_a - an array containing three double precision numbers
 * @param scalar - value used to scale vector 'a' with
 * @param vector_c - vector used to store the computational result
 */
static inline void vector_scale(double *vector_a, double scalar, double *vector_c) {
  vector_c[0] = scalar * vector_a[0];
  vector_c[1] = scalar * vector_a[1];
  vector_c[2] = scalar * vector_a[2];
  
}


/**
 * Operation that takes two equal-length sequences of numbers (usually coordinate vectors)
 * and returns a single number. In three-dimensional space, the dot product contrasts with 
 * the cross product of two vectors, which produces a pseudovector as the result. The dot 
 * product is directly related to the cosine of the angle between two vectors in Euclidean 
 * space of any number of dimensions.
 * 
 * @param vector_a - an array containing three double precision numbers
 * @param vector_b - an array containing three double precision numbers
 * @param vector_c - vector used to store the computational result
 * @returns double used typically as a scalar value
 */
static inline double vector_dot_product(double *vector_a, double *vector_b) {
  return (vector_a[0] * vector_b[0]) + (vector_a[1] * vector_b[1]) + (vector_a[2] * vector_b[2]);
  
}


/**
 * Given two linearly independent vectors a and b, the cross product, a × b, is a vector that 
 * is perpendicular to both a and b and therefore normal to the plane containing them.
 * 
 * @param vector_a - an array containing three double precision numbers
 * @param vector_b - an array containing three double precision numbers
 * @param vector_c - vector used to store the computational results
 */
static inline void vector_cross_product(double *vector_a, double *vector_b, double *vector_c) {
  vector_c[0] = (vector_a[1] * vector_b[2]) - (vector_a[2] * vector_b[1]);
  vector_c[1] = (vector_a[2] * vector_b[0]) - (vector_a[0] * vector_b[2]);
  vector_c[2] = (vector_a[0] * vector_b[1]) - (vector_a[1] * vector_b[0]);
  
}


/**
 * The operation of exchanging all points of a mathematical object with their mirror images 
 * (i.e., reflections in a mirror).
 * 
 * @param vector_a - an array containing three double precision numbers
 * @param vector_b - an array containing three double precision numbers
 * @param vector_c - vector used to store the computational result
 */
static inline void vector_reflection(double *vector_a, double *vector_b, double *vector_c) {
	double scalar;
	double vector_o[3] = {0, 0, 0};

	scalar = vector_dot_product(vector_a, vector_b);
	vector_scale(vector_b, (2.0 * scalar), vector_o);
	vector_subtract(vector_a, vector_o, vector_c);

}


/**
 * Pythagorean Formula.
 * 
 * @param vector_a - an array containing three double precision numbers
 * @returns scalar that reprents a vectors magnitude (length)
 */
static inline double vector_length(double *vector_a) {
	return sqrt(pow(vector_a[0], 2) + pow(vector_a[1], 2) + pow(vector_a[2], 2));
  
}


/**
 * Transposes or copies one vector values to another throught assignment.
 * 
 * @param vector_a - an array containing three double precision numbers
 * @param vector_c - an array containing the contents of vector_a
 */
static inline void vector_copy(double *vector_a, double *vector_b) {
	vector_b[0] = vector_a[0];
	vector_b[1] = vector_a[1];
	vector_b[2] = vector_a[2];
	
}

#endif