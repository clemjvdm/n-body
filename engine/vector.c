#include "vector.h"
#include <math.h>

vector newVector(double x, double y) {
  vector v = {x, y};
  return v;
}

vector add(vector a, vector b) {
  vector v = {a.x + b.x, a.y + b.y};
  return v;
}

vector sub(vector a, vector b) {
  vector v = {a.x - b.x, a.y - b.y};
  return v;
}

vector scale(double x, vector a) {
  vector v = {x * a.x, x * a.y};
  return v;
}

double mod(vector a) { return sqrt(a.x * a.x + a.y * a.y); }
