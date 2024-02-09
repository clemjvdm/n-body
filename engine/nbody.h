#ifndef NBODY_H
#define NBODY_H

#include "vector.h"
#include <stddef.h>

typedef struct {
  double mass;
  vector pos;
  vector vel;
  vector acc;
} particle;

typedef particle *System;

enum Method { // TODO: should this be typedef'd?
  PP,
  TC,
};

System createSystem(size_t particle_count);
System openSystem(char *filename);
void initSystem(System system, size_t size, int x_bound, int y_bound);
void updateSystem(System system, size_t size, int timestep, enum Method method);
void pp(System system, size_t size, int timestep); // particle-particle method
void tc(System system);                            // tree code method

#endif
