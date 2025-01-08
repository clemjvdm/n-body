#ifndef VECTOR_H
#define VECTOR_H


/*
 * Let's a vector be acces both as a struct (v.x, v.y) and as an array
 * (v.xy[0], v.xy[1]).
 *
 * Because only floats are stored in struct, c won't add padding and
 * accessing through array is safe.
 */
typedef union vec2 {
        struct {
            float x, y;
        };
        float xy[2];
} vec2;

// double precision vector
typedef union vec2d {
        struct {
            double x, y;
        };
        double xy[2];
} vec2d;

vec2 new_vector(float x, float y);
vec2 add(vec2 a, vec2 b);
vec2 sub(vec2 a, vec2 b);
vec2 scale(float x, vec2 a);
vec2 scaled(double x, vec2 a);
int equal(vec2 a, vec2 b);
float mod(vec2 a);
vec2d vec2ToVec2d(vec2 v);

#endif
