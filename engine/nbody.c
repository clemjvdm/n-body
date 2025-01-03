#include "nbody.h"
#include "vector.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define PI 3.14159265358979323846

/**
 * Create an empty particle system.
 *
 * @param particle_count  Maximum amount of particles in the system
 * @param grav_const  The gravitational constant of the system
 */
particle_system createSystem(size_t particle_count, double grav_const) {
  particle_system system;
  system.size = particle_count;
  system.grav_const = grav_const;
  system.particles = malloc(particle_count * sizeof(particle));
  return system;
};

/**
 * Open a saved particle system from a file generated by `saveSystem`
 *
 * @param filename  The file to open
 */
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

/**
 * Save a particle system to a file
 *
 * @param filename  A file to save to
 *
 * @note Will delete all previous contents of the file
 * @note Creates the file if it doesn't exist
 */
void saveSystem(particle_system system, char *filename) {
  FILE *fptr = fopen(filename, "w");
  for (int i = 0; i < system.size; i++) {
    particle p = system.particles[i];
    fprintf(fptr, "%lf %lf\n", p.mass, p.radius);
    fprintf(fptr, "%lf %lf\n", p.pos.x, p.pos.y);
    fprintf(fptr, "%lf %lf\n", p.vel.x, p.vel.y);
    fprintf(fptr, "%lf %lf\n\n", p.acc.x, p.acc.y);
  }
  fclose(fptr);
}

/**
 * Generates a random float number between 0 and a given maximum
 *
 * @param max  The highest acceptable random number
 */
float randFloat(int max) { return (float)rand() / (float)(RAND_MAX / max); }

/**
 * Initializes a particle system by populating it with randomly generated particles.
 * 
 * @param system The particle system to initialize
 * @param radius The radius within which generated particles will lie
 *
 * NOTE: currently some things are not randomized, like radius and mass.
 * NOTE: the current method for initialization is not very interesting, and will probably be changed at some point
 */
void initSystem(particle_system system, int radius) {
  for (int i = 0; i < system.size; i++) {
    vector zero = {0, 0};
    float distance = randFloat(radius);
    float angle = randFloat(1) * 2 * PI;
    system.particles[i].pos.x = cos(angle) * distance;
    system.particles[i].pos.y = sin(angle) * distance;
    system.particles[i].vel = zero;
    system.particles[i].acc = zero;
    system.particles[i].radius = 10;
    system.particles[i].mass = 5;
  }
}

/**
 * Calculates the distance between two particles
 *
 * @param a The first particle
 * @param b The second particle
 */
double dist(particle a, particle b) { return mod(sub(a.pos, b.pos)); }

/**
 * Updates the acceleration of all particles within a system, taking into account
 * their current acceleration. This is accomplished by calculating the force
 * between the two particles.
 *
 * @param system  The particle system to update
 *
 * NOTE: This function could be formally proven
 */
void computeAcc(particle_system system) {
  vector zero = {0, 0};
  particle *particles = system.particles;

  particles[0].acc = zero;
  for (int i = 1; i < system.size; i++) {
    particles[i].acc = zero;

    for (int j = i - 1; j >= 0; j--) {
      particle *a = &particles[i]; // access by reference to change acc
      particle *b = &particles[j];

      vector s = sub(a->pos, b->pos);
      double temp = s.x * s.x + s.y * s.y;

      vector force =
          scale((system.grav_const * a->mass * b->mass) / pow(dist(*a, *b), 3),
                sub(a->pos, b->pos)); // this can be sped up by not using mod

      force = (isnan(force.x) ? zero
                              : force); // if a.pos == b.pos, no force applied

      a->acc = add(a->acc, scale(-1, force));
      b->acc = add(b->acc, force);
    }
  }
}

/**
 * This function resolves collisions by checking for overalapped particles
 * and modifying their velocities appropriately.
 *
 * @param system  The particle system to update
 *
 * NOTE: This function could be formally proven
 */
void resolveCollision(particle_system system) {
  for (int i = 0; i < system.size - 1;
       i++) { // TODO: faster way to check for collisions?
    for (int j = i + 1; j < system.size; j++) {
      particle *a = &system.particles[i];
      particle *b = &system.particles[j];
      if (dist(*a, *b) <= a->radius + b->radius) {
        vector temp = a->vel;
        a->vel = scale(1, b->vel); // TODO: momentum conservation?
        b->vel = scale(1, temp);
      }
    }
  }
}

/**
 * This function finds the time of collision between two particles.
 * It returns -1 if their will not collide given current system parameters.
 * This is useful for a priori collision detection.
 * TODO: explain how this works in readme.
 *
 * @param f First particle
 * @param g Second particle
 */
double find_toc(particle f, particle g) {
    vector p1 = f.pos;
    vector p2 = g.pos;
    vector v1 = f.vel;
    vector v2 = g.vel;
    double r1 = f.radius;
    double r2 = g.radius;

    double a = (p1.x * p1.x) + (p2.x * p2.x) - 2 * p1.x * p2.x;
    double b = (p1.y * p1.y) + (p2.y * p2.y) - 2 * p1.y * p2.y;
    double c = (p1.x - p2.x) * (v1.x - v2.x) + (p1.y - p2.y) * (v1.y - v2.y);
    double d = (v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y);
    double e = a + b - r1 * r1 - r2 * r2 - 2 * r1 * r2;
    double delta = 4 * (c * c) - 4 * d * e;

    if (delta == 0) {
        return -1 * (c/d);
    }

    if (delta > 0) {
        return (-2 * c - sqrt(4 * (c * c) - 4 * d * e)) / (2 * d);
    }

    return -1;
}

/**
 * Update the positions of particles within a system based on their
 * velocity.
 *
 * @param system  The particle system to update
 * @param delta_t The time step
 */
void update_positions(particle_system system, int delta_t) {
  if (delta_t == 0) { return; }
  particle *particles = system.particles;
  for (int i = 0; i < system.size; i++) {
    particles[i].pos = add(particles[i].pos, scale(delta_t, particles[i].vel));
  }
}


/**
 * Remove a particle from a system by index.
 *
 * @param system  The system from which to remove a particle
 * @param particle_index  The index of the particle to remove
 */
void remove_particle(particle_system *system, int particle_index) {
    for (int j = particle_index; j < system->size-1; j++) {
        system->particles[j] = system->particles[j+1];
    } 
    system->size--;
}

/**
 * Removes intersecting particles from a system. This is used so that
 * an a priori collision detection algorithm can safely be used afterwards.
 *
 * @param system  The system for which to remove all intersecting particles
 */
void remove_intersections(particle_system *system) {
    for (int i=0; i<system->size - 1; i++) {
        for (int j=i+1; j<system->size; j++) {
            particle a = system->particles[i];
            particle b = system->particles[j];
            if (sqrt(((a.pos.x - b.pos.x)*(a.pos.x - b.pos.x)) + 
                     ((a.pos.y - b.pos.y)*(a.pos.y - b.pos.y))) <= a.radius + b.radius) {
                printf("particle removed\n");
                remove_particle(system, j);
            }
        }
    }
}


/**
 * Update the positions of particles within a system, taking collisions
 * into account. 
 *
 * @param system  The particle system to update
 * @param delta_t The time step
 *
 * @note This is a recursive func
 */
// TODO: better naming?
void update_positions_collisions(particle_system system, int delta_t) {
    double min_p = -1;
    int min_i = 0;
    int min_j = 0;

    // find the first collision which will take place in the system.
    // -1 if none.
    for (int i=0; i<system.size - 1; i++) {
        for (int j=i+1; j<system.size; j++) {
            double p = find_toc(system.particles[i],system.particles[j]);
            if (min_p == -1 && p >= 0) {   // TODO: is this the right approach? Ensures we pass a -1, if no collision which is more correct, but requires this if statement at every iter
                min_p = p;
                min_i = i;
                min_j = j;
            } else if (p < min_p && 0 < p) {
                min_p = p;
                min_i = i;
                min_j = j;
            } 
        }
    }

    if (0 <= min_p && min_p <= delta_t) {
        update_positions(system, min_p); // update positions until the first collision
        delta_t = delta_t - min_p; // compute the new delta_t (amount of time left in the jump)

        // update velocities for colliding particles
        vector temp = system.particles[min_i].vel;
        system.particles[min_i].vel = system.particles[min_j].vel;
        system.particles[min_j].vel = temp;
        //
        update_positions_collisions(system, delta_t); // recusively call this function with new detla_t
    } else {
        update_positions(system, delta_t);
    }
}

/**
 * Resolves the collisions within a system using an a priori method.
 * Not certain why this exists and I don't think it's being referenced anywhere else in the code.
 */
/*void resolveCollisionPriori(particle_system system) {*/
/*  for (int i = 0; i < system.size - 1; i++) {*/
/*    for (int j = i + 1; j < system.size; j++) {*/
/*      vector p1 = system.particles[i].pos;*/
/*      vector p2 = system.particles[j].pos;*/
/*      vector v1 = system.particles[i].vel;*/
/*      vector v2 = system.particles[j].vel;*/
/*      double r1 = system.particles[i].radius;*/
/*      double r2 = system.particles[j].radius;*/
/**/
/*      double a = (p1.x * p1.x) + (p2.x * p2.x) - 2 * p1.x * p2.x;*/
/*      double b = (p1.y * p1.y) + (p2.y * p2.y) - 2 * p1.y * p2.y;*/
/*      double c = (p1.x - p2.x) * (v1.x - v2.x) + (p1.y - p2.y) * (v1.y - v2.y);*/
/*      double d = (v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y);*/
/*      double e = a + b - r1 * r1 - r2 * r2 - 2 * r1 * r2;*/
/*      double delta = 4 * (c * c) - 4 * d * e;*/
/**/
/*      if (delta == 0) {*/
/*        double p = -1 * (c / d);*/
/*        if (p >= 0 && p <= 1) {*/
/*          scale(p, system.particles[i].vel);*/
/*          scale(p, system.particles[j].vel);*/
/*        }*/
/*      }*/
/*      if (delta > 0) {*/
/*        double p = (-2 * c - sqrt(4 * (c * c) - 4 * d * e)) / (2 * d);*/
/*        if (p >= 0 && p <= 1) {*/
/*          vector temp = system.particles[i].vel;*/
/*          system.particles[i].pos =*/
/*              add(system.particles[i].pos,*/
/*                  scale(p - 0.01, system.particles[i].vel));*/
/*          system.particles[j].pos =*/
/*              add(system.particles[j].pos,*/
/*                  scale(p - 0.01, system.particles[j].vel));*/
/*          system.particles[i].vel = scale(1, system.particles[j].vel);*/
/*          system.particles[j].vel = scale(1, temp);*/
/**/
/*          /*system.particles[i].vel = scale(*/
/*              0,*/
/*              system.particles[j]*/
/*                  .vel); // for these to be accurate, we know velocity at t1 =*/
/*                         // v1, and we know the current distance between the two*/
/*                         // particles. If we immediately handle the collision,*/
/*                         // this particle will fast forward in time, as they*/
/*                         // will have gone through the current v1, and the new*/
/*                         // velocity we created for the collision. Thats like 2*/
/*                         // timesteps. Instead we remove |v1| from |v2|*/
/*          system.particles[j].vel = scale(0, temp);*/
/*        } else {*/
/*          p = (-2 * c + sqrt(4 * (c * c) - 4 * d * e)) / (2 * d);*/
/*          if (p >= 0 && p <= 1) {*/
/*            scale(p, system.particles[i].vel);*/
/*            scale(p, system.particles[j].vel);*/
/*          }*/
/*        }*/
/*      }*/
/*    }*/
/*  }*/
/*}*/


/**
 * Update the velocity of particles within a system, based on their acceleration and
 * a time step.
 *
 * @param system  The system to update
 * @param delta_t The time step
 */
void computeVel(particle_system system, int delta_t) {
  particle *particles = system.particles;
  for (int i = 0; i < system.size; i++)
    particles[i].vel = add(particles[i].vel, scale(delta_t, particles[i].acc));
}

/*void computePos(particle_system system, int delta_t) {*/
/*  particle *particles = system.particles;*/
/*  for (int i = 0; i < system.size; i++) {*/
/*    if (particles[i].pos.x !=*/
/*        add(particles[i].pos, scale(delta_t, particles[i].vel)).x) {*/
/*    }*/
/*    particles[i].pos = add(particles[i].pos, scale(delta_t, particles[i].vel));*/
/*  }*/
/*}*/

/**
 * NOTE: I don't remember exactly why I named this `pp` and will likely refactor it in the future
 *
 * Updates a particle system to simulate n-bodies interacting through gravity, over
 * a certain time step. 
 *
 * @param system  The system to update
 * @param delta_t The time step
 */
void pp(particle_system system, int delta_t) {
  update_positions_collisions(system, delta_t);
  // computePos(system, delta_t);
  computeAcc(system);
  computeVel(system, delta_t);
  // resolveCollisionPriori(system);
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
