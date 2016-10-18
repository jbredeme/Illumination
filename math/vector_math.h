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
 * @param vector_a - an array storing vector information
 * @returns static function, no return 
 * @description vector normalization, divide each component by its magnitude
 */
static inline void normalize(double *vector_a) {
	double len = sqrt(pow(vector_a[0], 2) + pow(vector_a[1], 2) + pow(vector_a[2], 2));
	vector_a[0] /= len;
	vector_a[1] /= len;
	vector_a[2] /= len;
	
}


/**
 * TODO
 * 
 * @param TODO
 * @param TODO
 * @param TODO
 * @param TODO
 * @param TODO
 * @returns
 */
static inline void vector_add(double *vector_a, double *vector_b, double *vector_c) {
  vector_c[0] = vector_a[0] + vector_b[0];
  vector_c[1] = vector_a[1] + vector_b[1];
  vector_c[2] = vector_a[2] + vector_b[2];
  
}


/**
 * TODO
 * 
 * @param TODO
 * @param TODO
 * @param TODO
 * @param TODO
 * @param TODO
 * @returns
 */
static inline void vector_subtract(double *vector_a, double *vector_b, double *vector_c) {
  vector_c[0] = vector_a[0] - vector_b[0];
  vector_c[1] = vector_a[1] - vector_b[1];
  vector_c[2] = vector_a[2] - vector_b[2];
  
}


/**
 * TODO
 * 
 * @param TODO
 * @param TODO
 * @param TODO
 * @param TODO
 * @param TODO
 * @returns
 */
static inline void vector_scale(double *vector_a, double scalar, double *vector_c) {
  vector_c[0] = scalar * vector_a[0];
  vector_c[1] = scalar * vector_a[1];
  vector_c[2] = scalar * vector_a[2];
  
}


/**
 * TODO
 * 
 * @param TODO
 * @param TODO
 * @param TODO
 * @param TODO
 * @param TODO
 * @returns
 */
static inline double vector_dot_product(double *vector_a, double *vector_b) {
  return (vector_a[0] * vector_b[0]) + (vector_a[1] * vector_b[1]) + (vector_a[2] * vector_b[2]);
  
}


/**
 * TODO
 * 
 * @param TODO
 * @param TODO
 * @param TODO
 * @param TODO
 * @param TODO
 * @returns
 */
static inline void vector_cross_product(double *vector_a, double *vector_b, double *vector_c) {
  vector_c[0] = (vector_a[1] * vector_b[2]) - (vector_a[2] * vector_b[1]);
  vector_c[1] = (vector_a[2] * vector_b[0]) - (vector_a[0] * vector_b[2]);
  vector_c[2] = (vector_a[0] * vector_b[1]) - (vector_a[1] * vector_b[0]);
  
}


/**
 * TODO
 * 
 * @param TODO
 * @param TODO
 * @param TODO
 * @param TODO
 * @param TODO
 * @returns
 */
static inline void vector_reflection(double *vector_a, double *vector_b, double *vector_c) {
	double scalar;
	double vector_o[3] = {0, 0, 0};

	scalar = vector_dot_product(vector_a, vector_b);
	vector_scale(vector_b, (2.0 * scalar), vector_o);
	vector_subtract(vector_a, vector_o, vector_c);

}


/**
 * TODO
 * 
 * @param TODO
 * @param TODO
 * @param TODO
 * @param TODO
 * @param TODO
 * @returns
 */
static inline double vector_length(double *vector_a) {
	return sqrt(pow(vector_a[0], 2) + pow(vector_a[1], 2) + pow(vector_a[2], 2));
  
}


/**
 * TODO
 * 
 * @param TODO
 * @param TODO
 * @param TODO
 * @param TODO
 * @param TODO
 * @returns
 */
static inline void vector_clone(double *vector_a, double *vector_b) {
	vector_b[0] = vector_a[0];
	vector_b[1] = vector_a[1];
	vector_b[2] = vector_a[2];
	
}


/**
 * TODO
 * 
 * @param TODO
 * @param TODO
 * @param TODO
 * @param TODO
 * @param TODO
 * @returns
 */
static inline double vector_distance(double *vector_a, double *vector_b) {
	return sqrt(pow((vector_b[0] - vector_a[0]), 2) + pow((vector_b[1] - vector_a[1]), 2) + pow((vector_b[2] - vector_a[2]), 2));
  
}

#endif