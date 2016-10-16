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
 * @param v - an array storing vector information
 * @returns static function, no return 
 * @description vector normalization, divide each component by its magnitude
 */
static inline void normalize(double *v) {
	double len = sqrt(pow(v[0], 2) + pow(v[1], 2) + pow(v[2], 2));
	v[0] /= len;
	v[1] /= len;
	v[2] /= len;
	
}


/**
 * @param
 * @param
 * @param
 * @returns
 * @description
 */
static inline void vector_add(double *vector_a, double *vector_b, double *vector_c) {
  vector_c[0] = vector_a[0] + vector_b[0];
  vector_c[1] = vector_a[1] + vector_b[1];
  vector_c[2] = vector_a[2] + vector_b[2];
  
}


/**
 * @param
 * @param
 * @param
 * @returns
 * @description
 */
static inline void vector_subtract(double *vector_a, double *vector_b, double *vector_c) {
  vector_c[0] = vector_a[0] - vector_b[0];
  vector_c[1] = vector_a[1] - vector_b[1];
  vector_c[2] = vector_a[2] - vector_b[2];
  
}


/**
 * @param
 * @param
 * @param
 * @returns
 * @description
 */
static inline void vector_scale(double *vector_a, double scalar, double *vector_c) {
  vector_c[0] = scalar * vector_a[0];
  vector_c[1] = scalar * vector_a[1];
  vector_c[2] = scalar * vector_a[2];
  
}


/**
 * @param
 * @param
 * @param
 * @returns
 * @description
 */
static inline double vector_dot_product(double *vector_a, double *vector_b) {
  return (vector_a[0] * vector_b[0]) + (vector_a[1] * vector_b[1]) + (vector_a[2] * vector_b[2]);
  
}


/**
 * @param
 * @param
 * @param
 * @returns
 * @description
 */
static inline void vector_cross_product(double *vector_a, double *vector_b, double *vector_c) {
  vector_c[0] = (vector_a[1] * vector_b[2]) - (vector_a[2] * vector_b[1]);
  vector_c[1] = (vector_a[2] * vector_b[0]) - (vector_a[0] * vector_b[2]);
  vector_c[2] = (vector_a[0] * vector_b[1]) - (vector_a[1] * vector_b[0]);
  
}


/**
 * @param
 * @param
 * @param
 * @returns
 * @description
 */
static inline void vector_copy(double *vector_a, double *vector_b) {
  vector_b[0] = vector_a[1];
  vector_b[1] = vector_a[2];
  vector_b[2] = vector_a[0];
  
}

/**
 * @param
 * @param
 * @param
 * @returns
 * @description
 */
static inline double vector_length(double *vector_a) {
	return sqrt(pow(vector_a[0], 2) + pow(vector_a[1], 2) + pow(vector_a[2], 2));
  
}


/**
 * @param
 * @param
 * @param
 * @returns
 * @description
 */
static inline double vector_distance_3d(double *vector_a, double *vector_b) {
	return sqrt(pow((vector_b[0] - vector_a[0]), 2) + pow((vector_b[1] - vector_a[1]), 2) + pow((vector_b[2] - vector_a[2]), 2));
  
}

#endif