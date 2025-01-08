#ifndef NBODY_H
#define NBODY_H

#include "vector.h"
#include <stddef.h>

typedef struct particle {
  float mass;
  float radius;
  vec2 pos;
  vec2 vel;
  vec2 acc;
} particle;

typedef struct {
  particle *particles;
  size_t size;
} particle_system;

particle_system open_system(char *filename);
void save_system(particle_system s, char *filename);
particle_system create_system(size_t particle_count, float grav_const);
void init_system(particle_system s, int radius);
void remove_intersections(particle_system *system);
void remove_particle(particle_system *system, int particle_index);

#endif
