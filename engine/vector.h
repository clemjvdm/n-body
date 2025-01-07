#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
  double x, y;
} vector;

vector new_vector(double x, double y);
vector add(vector a, vector b);
vector sub(vector a, vector b);
vector scale(double x, vector a);
int equal(vector a, vector b);
double mod(vector a);

#endif
