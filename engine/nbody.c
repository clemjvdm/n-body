#include "nbody.h"
#include "vector.h"
#include <math.h>
#include <stdlib.h>

#define GRAV_CONST 1

System createSystem(size_t particle_count) {
  return malloc(particle_count * sizeof(particle));
};

double dist(particle a, particle b) { return mod(sub(a.pos, b.pos)); }

void computeAcc(System system, size_t size) {
  vector zero = {0, 0};
  system[0].acc = zero;
  for (int i = 1; i < size; i++) {
    system[i].acc = zero;
    for (int j = i - 1; j >= 0; j--) {
      particle a = system[i];
      particle b = system[j];

      vector acc =
          scale((GRAV_CONST * a.mass * b.mass) /
                    pow(dist(a, b), 3), // this can be sped up by not using mod
                sub(a.pos, b.pos));
      a.acc = add(a.acc, acc);
      b.acc = add(a.acc, acc);
    }
  }
}

void computeVel(System system, size_t size, int delta_t) {
  for (int i = 0; i < size; i++)
    system[i].vel = add(system[i].vel, scale(delta_t, system[i].acc));
}

void computePos(System system, size_t size, int delta_t) {
  for (int i = 0; i < size; i++)
    system[i].pos = add(system[i].pos, scale(delta_t, system[i].vel));
}

void pp(System system, size_t size, int delta_t) {
  computeAcc(system, size);
  computeVel(system, size, delta_t);
  computePos(system, size, delta_t);
}

void updateSystem(System system, size_t size, int delta_t, enum Method method) {
  switch (method) {
  case PP:
    pp(system, size, delta_t);
    break;
  case TC:
    break;
  }
}
