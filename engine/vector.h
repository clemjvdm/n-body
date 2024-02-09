#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
  double x, y;
} vector;

vector newVector(double x, double y);
vector add(vector a, vector b);
vector sub(vector a, vector b);
vector scale(double x, vector a);
double mod(vector a);

#endif
