#include "system.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define PI 3.14159265358979323846

/**
 * Create an empty particle system.
 *
 * @param particle_count  Maximum amount of particles in the system
 * @param grav_const  The gravitational constant of the system
 */
particle_system create_system(size_t particle_count, float grav_const) {
  particle_system system;
  system.size = particle_count;
  system.particles = malloc(particle_count * sizeof(particle));
  return system;
};

/**
 * Count the amount of particles defined within a system file generated by save_system. Returns
 * -1 if the system file is corrupted.
 *
 * @param filename The file in which to count
 * @return The amount of particles defined within ths system file, or -1 for a broken system file
 */
int particle_count(char *filename) {
  FILE *fptr = fopen(filename, "r");
  if (!fptr) { return 0; }

  int count = 0;
  float _;
  while (fscanf(fptr, "%f%f", &_, &_) == 2) { // mass and radius
    if (fscanf(fptr, "%f%f", &_, &_) != 2) return -1;  // position
    if (fscanf(fptr, "%f%f", &_, &_) != 2) return -1;  // velocity
    if (fscanf(fptr, "%f%f", &_, &_) != 2) return -1;  // acceleration
    count++;
  }
  fclose(fptr);
  return count;
}

/**
 * Open a saved particle system from a file generated by `save_system`
 *
 * @param filename  The file to open
 */
particle_system open_system(char *filename) {
  FILE *fptr = fopen(filename, "r");

  particle_system system;
  system.size = particle_count(filename);
  if (system.size == -1) { return (particle_system){0}; } // TODO: Error handling
                                                          //
  system.particles = malloc(system.size * sizeof(particle));
  for (int i = 0; i < system.size; i++) {
    particle *p = &system.particles[i];
    fscanf(fptr, "%f%f", &(p->mass), &(p->radius));
    fscanf(fptr, "%f%f", &(p->pos.x), &(p->pos.y));
    fscanf(fptr, "%f%f", &(p->vel.x), &(p->vel.y));
    fscanf(fptr, "%f%f", &(p->acc.x), &(p->acc.y));
  }
  fclose(fptr);
  return system;
}

/**
 * Generates a random float number between 0 and a given maximum
 *
 * @param max  The highest acceptable random number
 */
float rand_float(float min, float max) { return rand() / ((float)RAND_MAX / (max)) + min; }

/**
 * Initializes a particle system by populating it with randomly generated particles.
 * 
 * @param system The particle system to initialize
 * @param radius The radius within which generated particles will lie
 *
 * NOTE: currently some things are not randomized, like radius and mass.
 * NOTE: the current method for initialization is not very interesting, and will probably be changed at some point
 */
void init_system(particle_system system, int radius) {
  for (int i = 0; i < system.size; i++) {
    vec2 zero = {0, 0};
    float distance = rand_float(0, radius);
    float angle = rand_float(0, 1) * 2 * PI;
    system.particles[i].pos.x = cos(angle) * distance;
    system.particles[i].pos.y = sin(angle) * distance;
    system.particles[i].vel = zero;
    system.particles[i].acc = zero;
    system.particles[i].radius = 5;
    system.particles[i].mass = system.particles[i].radius;
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
    for (int i=0; i<system->size-1; i++) {
        for (int j=i+1; j<system->size; j++) {
            particle a = system->particles[i];
            particle b = system->particles[j];
            if (sqrt(((a.pos.x - b.pos.x)*(a.pos.x - b.pos.x)) + 
                     ((a.pos.y - b.pos.y)*(a.pos.y - b.pos.y))) <= a.radius + b.radius) {
                remove_particle(system, j);
                j--; // particles will be shifted so we need to check j again
            }
        }
    }
}
