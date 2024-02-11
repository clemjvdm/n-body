#include "nbody.h"
#include "vector.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

particle_system createSystem(size_t particle_count, double grav_const) {
  particle_system system;
  system.size = particle_count;
  system.grav_const = grav_const;
  system.particles = malloc(particle_count * sizeof(particle));
  return system;
};

particle_system openSystem(char *filename) {
  FILE *fptr = fopen(filename, "r");
  particle_system system;
  fscanf(fptr, "%lf%zd", &system.grav_const, &system.size);
  system.particles = malloc(system.size * sizeof(particle));
  for (int i = 0; i < system.size; i++) {
    particle *p = &system.particles[i];
    fscanf(fptr, "%lf%lf", &(p->mass), &(p->radius));
    fscanf(fptr, "%lf%lf", &(p->pos.x), &(p->pos.y));
    fscanf(fptr, "%lf%lf", &(p->vel.x), &(p->vel.y));
    fscanf(fptr, "%lf%lf", &(p->acc.x), &(p->acc.y));
  }
  fclose(fptr);
  return system;
}

void saveSystem(particle_system system, char *filename) {
  FILE *fptr = fopen(filename, "w");
  fprintf(fptr, "%lf %zd\n\n", system.grav_const, system.size);
  for (int i = 0; i < system.size; i++) {
    particle p = system.particles[i];
    fprintf(fptr, "%lf %lf\n", p.mass, p.radius);
    fprintf(fptr, "%lf %lf\n", p.pos.x, p.pos.y);
    fprintf(fptr, "%lf %lf\n", p.vel.x, p.vel.y);
    fprintf(fptr, "%lf %lf\n\n", p.acc.x, p.acc.y);
  }
  fclose(fptr);
}

double dist(particle a, particle b) { return mod(sub(a.pos, b.pos)); }

void computeAcc(particle_system system) {
  vector zero = {0, 0};
  particle *particles = system.particles;

  particles[0].acc = zero;
  for (int i = 1; i < system.size; i++) {
    particles[i].acc = zero;

    for (int j = i - 1; j >= 0; j--) {
      particle *a = &particles[i]; // access by reference to change acc
      particle *b = &particles[j];

      vector force =
          scale((system.grav_const * a->mass * b->mass) /
                    pow(dist(*a, *b), 3), // TODO: can pow(...) == 0? if so fix
                sub(a->pos, b->pos)); // this can be sped up by not using mod

      a->acc = add(a->acc, scale(-1, force)); // TODO: figure out why i have to
                                              // scale a with -1 and not b
      b->acc = add(b->acc, force);
    }
  }
}

void resolveCollision(particle_system system) { // TODO: improve collisions
  for (int i = 0; i < system.size - 1; i++) {
    for (int j = i + 1; j < system.size; j++) {
      particle *a = &system.particles[i];
      particle *b = &system.particles[j];
      if (dist(*a, *b) <= a->radius + b->radius) {
        vector temp = a->vel;
        a->vel = b->vel;
        b->vel = scale(1, temp);
      }
    }
  }
}

void computeVel(particle_system system, int delta_t) {
  particle *particles = system.particles;
  for (int i = 0; i < system.size; i++)
    particles[i].vel = add(particles[i].vel, scale(delta_t, particles[i].acc));
}

void computePos(particle_system system, int delta_t) {
  particle *particles = system.particles;
  for (int i = 0; i < system.size; i++) {
    if (particles[i].pos.x !=
        add(particles[i].pos, scale(delta_t, particles[i].vel)).x) {
    }
    particles[i].pos = add(particles[i].pos, scale(delta_t, particles[i].vel));
  }
}

void pp(particle_system system, int delta_t) {
  computeAcc(system);
  computeVel(system, delta_t);
  computePos(system, delta_t);
  resolveCollision(system);
}

void updateSystem(particle_system system, int delta_t, enum Method method) {
  switch (method) {
  case PP:
    pp(system, delta_t);
    break;
  case TC:
    break;
  }
}
