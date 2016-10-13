/**
 * Author: Jarid Bredemeier
 * Email: jpb64@nau.edu
 * Date: Thursday, October 6, 2016
 * File: raycaster.c
 * Copyright © 2016 All rights reserved 
 */
 
#ifndef vector_math_h
#define vector_math_h

/**
 * vector_add
 *
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
 * vector_subtract
 *
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
 * vector_scale
 *
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
 * vector_dot_product
 *
 * @param
 * @param
 * @param
 * @returns
 * @description
 */
static inline double vector_dot_product(double *vector_a, double *vector_b) {
  return vector_a[0]*vector_b[0] + vector_a[1]*vector_b[1] + vector_a[2]*vector_b[2];
}


/**
 * vector_cross_product
 *
 * @param
 * @param
 * @param
 * @returns
 * @description
 */
static inline void vector_cross_product(double *vector_a, double *vector_b, double *vector_c) {
  vector_c[0] = vector_a[1] * vector_b[2] - vector_a[2] * vector_b[1];
  vector_c[1] = vector_a[2] * vector_b[0] - vector_a[0] * vector_b[2];
  vector_c[2] = vector_a[0] * vector_b[1] - vector_a[1] * vector_b[0];
}

#endif