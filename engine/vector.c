#include "vector.h"
#include <math.h>


/**
 * Creates a new 2D vector with the given x and y components.
 * 
 * @param x The x-component of the vector
 * @param y The y-component of the vector
 * @return A new vector with the specified components
 */
vec2 new_vector(float x, float y) {
  vec2 v = {x, y};
  return v;
}

/**
 * Adds two vectors component-wise.
 * 
 * @param a The first vector
 * @param b The second vector
 * @return A new vector representing the sum of a and b
 */
vec2 add(vec2 a, vec2 b) {
  vec2 v = {a.x + b.x, a.y + b.y};
  return v;
}

/**
 * Checks if two vectors are equal by comparing their components.
 * 
 * @param a The first vector
 * @param b The second vector
 * @return 1 if the vectors are equal, 0 otherwise
 *
 * NOTE: This function could be updated to use a VECTOR_EPSILON instead of `==`
 */
int equal(vec2 a, vec2 b) { 
  return (a.x == b.x && a.y == b.y); 
}

/**
 * Subtracts vector b from vector a component-wise.
 * 
 * @param a The vector to subtract from
 * @param b The vector to subtract
 * @return A new vector representing a - b
 */
vec2 sub(vec2 a, vec2 b) {
  vec2 v = {a.x - b.x, a.y - b.y};
  return v;
}

/**
 * Multiplies a vector by a scalar value.
 * 
 * @param x The scalar value to multiply by
 * @param a The vector to scale
 * @return A new vector representing x * a
 */
vec2 scale(float x, vec2 a) {
  vec2 v = {x * a.x, x * a.y};
  return v;
}

/**
 * Multiplies a vector by a double precision scalar value.
 * 
 * @param x The scalar value to multiply by
 * @param a The vector to scale
 * @return A new vector representing x * a
 */
vec2 scaled(double x, vec2 a) {
  vec2 v = {(double)(x * a.x), (double)(x * a.y)};
  return v;
}

/**
 * Calculates the magnitude (length) of a vector.
 * 
 * @param a The vector whose magnitude to calculate
 * @return The magnitude of the vector
 */
float mod(vec2 a) { 
  return sqrt(a.x * a.x + a.y * a.y); 
}

/**
 * Turns a float vector to a double vector.
 *
 * @param v the vector to convert
 */
vec2d vec2ToVec2d(vec2 v) {
  return (vec2d){(double)v.x, (double)v.y};
}
