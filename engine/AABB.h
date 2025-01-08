#ifndef AABB_H
#define AABB_H

#include "system.h"
#include "vector.h"

typedef struct {
  vec2 min;
  vec2 max;
  void *obj;
} AABB;

typedef struct {
  void *a;
  void *b;
} tuple;

typedef AABB *gAABBArray;

void sortAndSweepAABBArray(gAABBArray arr, size_t len, void (*callback)(void *, void *, void*), void *extra_params);

#endif // !AABB_H
