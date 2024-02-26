#ifndef NBODY_H
#define NBODY_H

#include "vector.h"
#include <stddef.h>

typedef struct {
  double mass;
  double radius;
  vector pos;
  vector vel;
  vector acc;
} particle;

typedef struct {
  particle *particles;
  size_t size;
  double grav_const;
} particle_system;

enum Method { // TODO: should this be typedef'd?
  PP,
  TC,
};

particle_system createSystem(size_t particle_count, double grav_const);
particle_system openSystem(char *filename);
void saveSystem(particle_system system, char *filename);
void initSystem(particle_system system, int radius);
void updateSystem(particle_system system, int timestep, enum Method method);
void pp(particle_system system,
        int timestep);           // particle-particle method
void tc(particle_system system); // tree code method
void remove_intersections(particle_system *system);

#endif
