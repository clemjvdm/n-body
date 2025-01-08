#include "system.h"
#include "simulation.h"
#include "vector.h"
#include "AABB.h"
#include <float.h>
#include <math.h>
#include <stdio.h>

#define PI 3.14159265358979323846
#define EPSILON 1e-9

/**
 * Calculates the distance between two particles
 *
 * @param a The first particle
 * @param b The second particle
 */
double dist(particle a, particle b) { return mod(sub(a.pos, b.pos)); }

/**
 * Updates the acceleration of all particles within a system. This is accomplished by calculating
 * gravitational forces between them.
 *
 * @param s The particle system to update
 * @param g The gravitational constant
 *
 * @note O(n^2)
 */
void update_acc(particle_system s, float g) {
  vec2 zero = {0, 0};
  particle *particles = s.particles;

  particles[0].acc = zero;
  for (int i = 1; i < s.size; i++) {
    particles[i].acc = zero;
    for (int j = i - 1; j >= 0; j--) {
      particle *a = &particles[i];
      particle *b = &particles[j];

      vec2 force = scale((g * a->mass * b->mass) / pow(dist(*a, *b), 3), sub(a->pos, b->pos));

      force = (isnan(force.x) ? zero : force); // if a.pos == b.pos no force applied, this prevents bugs

      a->acc = add(a->acc, scale(a->mass,scale(-1, force)));
      b->acc = add(b->acc, scale(b->mass,force));
    }
  }
}

/**
 * This function finds the first time of collision between two particles. Returns a negative number 
 * if they will not collide given the time step. More in docs/collision_detection.
 *
 * @param f First particle
 * @param g Second particle
 *
 * @note Only predictable results when particles are not overlapping
 */
double toc(particle x, particle y) {
    vec2d p1 = vec2ToVec2d(x.pos);
    vec2d p2 = vec2ToVec2d(y.pos);
    vec2d v1 = vec2ToVec2d(x.vel);
    vec2d v2 = vec2ToVec2d(y.vel);
    double r1 = x.radius;
    double r2 = y.radius;

    double a = (p1.x * p1.x) + (p2.x * p2.x) - 2 * p1.x * p2.x;
    double b = (p1.y * p1.y) + (p2.y * p2.y) - 2 * p1.y * p2.y;
    double c = (p1.x - p2.x) * (v1.x - v2.x) + (p1.y - p2.y) * (v1.y - v2.y);
    double d = (v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y);
    double e = a + b - r1 * r1 - r2 * r2 - 2 * r1 * r2;
    double delta = 4 * (c * c) - 4 * d * e;

    if (delta < -EPSILON || fabs(delta) < EPSILON) { // no real solutions || particles have the same velocity
      return -1;
    }

    if (fabs(delta) < EPSILON) {  // one real solution
      return -1 * (c/d);
    }

    // two real solutions, are both positive or both negative
    return (-2 * c - sqrt(delta)) / (2 * d); // always take smallest solution
}

/**
 * Tests to see whether two particles collide, and sets it
 * as the first_collision if the toc is smaller than the current
 * first_collision. This function is used to as a callback to
 * sortAndSweepAABBArray.
 *
 * @param a First particle
 * @param b Second particle
 * @param extra_params Pointer to first collision
 */
void test_collision(void *a, void *b, void *extra_params) {
  particle *x = (particle *)a;
  particle *y = (particle *)b;
  collision *first_collision = (collision *)extra_params;
  double t = toc(*x, *y);
  if (-EPSILON <= t && t < first_collision->t) {
    *first_collision = (collision){t, x, y};
  }
}

/**
 * Update the velocity of particles within a system based on their acceleration and
 * a time step.
 *
 * @param system  The system to update
 * @param delta_t The time step
 *
 * @note O(n)
 */
void update_vel(particle_system s, float t) {
  particle *particles = s.particles;
  for (int i = 0; i < s.size; i++)
    particles[i].vel = add(particles[i].vel, scale(t, particles[i].acc));
}


/**
 * Moves each particle within a system over a time delta_t based on its
 * velocity.
 *
 * @param system  The particle system to update
 * @param delta_t The time step
 *
 * @note O(n)
 */
void move(particle_system system, double delta_t) {
  if (delta_t == 0) { return; }
  particle *particles = system.particles;
  for (int i = 0; i < system.size; i++) {
    particles[i].pos = add(particles[i].pos, scaled(delta_t, particles[i].vel));
  }
}

/**
 * Applies collision response to two colliding particles. Does so by updating
 * both their velocities.
 *
 * @param a The first particle
 * @param b The second particle
 * @param cor Coefficient of restitution // TODO:
 *
 * @note The particles must be at their TOC.
 */
void collision_response(particle *a, particle *b) {
  float d = sqrt((a->pos.x - b->pos.x)*(a->pos.x - b->pos.x)+(a->pos.y - b->pos.y)*(a->pos.y - b->pos.y));
  float nx = (b->pos.x-a->pos.x)/d;
  float ny = (b->pos.y-a->pos.y)/d;
  float rv = (a->vel.x - b->vel.x)*nx + (a->vel.y - b->vel.y)*ny;
  float p = 2*rv/(a->mass+b->mass);
  vec2 v1 = new_vector(a->vel.x - p*a->mass*nx, a->vel.y - p*a->mass*ny);
  vec2 v2 = new_vector(b->vel.x + p*b->mass*nx, b->vel.y + p*b->mass*ny);

  a->vel = v1;
  b->vel = v2;

  return;
}

/**
 * Updates the positions of all particles within a system over a certain
 * time step.
 *
 * @param s Particle system to update
 * @param delta_t The time step
 *
 * @note worst case O(n^2)
 * @note average case O(n*log(n))
 */
void update_pos(particle_system s, float t) {
  AABB a[s.size];
  for (int i=0; i<s.size; i++) {
    particle p = s.particles[i];
    float r = p.radius;
    vec2 next_pos = add(p.pos, scale(t, p.vel));
    float minx = (p.pos.x-r < next_pos.x-r ? p.pos.x-r : next_pos.x-r);
    float maxx = (p.pos.x+r > next_pos.x+r ? p.pos.x+r : next_pos.x+r);
    float miny = (p.pos.y-r < next_pos.y-r ? p.pos.y-r : next_pos.y-r);
    float maxy = (p.pos.y+r > next_pos.y+r ? p.pos.y+r : next_pos.y+r);
    a[i] = (AABB){new_vector(minx, miny), new_vector(maxx, maxy), &s.particles[i]};
  }
  collision first_collision = (collision){DBL_MAX, NULL, NULL};
  sortAndSweepAABBArray(a, s.size, test_collision, &first_collision);

  if (first_collision.t < t) { // first collision is always >= 0
    move(s, first_collision.t);
    collision_response(first_collision.a, first_collision.b);
    update_pos(s, (t-first_collision.t));
  } else {
    move(s, t);
  }
}


/**
 */
void step_system(particle_system s, sim_config c) {
  update_acc(s, c.grav_const);
  update_vel(s, c.timestep);
  update_pos(s, c.timestep);
}
