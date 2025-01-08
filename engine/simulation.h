#ifndef SIMULATION_H
#define SIMULATION_H

#include "system.h"
#include <stddef.h>

typedef struct {
  float timestep;
  float grav_const;
  float cof;
} sim_config;

typedef struct {
  double t;
  particle *a, *b;
} collision;

void step_system(particle_system s, sim_config c);
void update_acc(particle_system s, float grav_const);
void update_vel(particle_system s, float delta_t);
void update_pos(particle_system s, float delta_t);
double toc(particle x, particle y);

#endif
