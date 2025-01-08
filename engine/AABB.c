#include "AABB.h"
#include <stdlib.h>

int gSortAxis = 0; // Specifies axis (0/1) to sort on (here arbitrarily initialized)

// Comparison function for qsort. Given two arguments A and B must return a
// value of less than zero if A < B, zero if A = B, and greater than zero if A > B
int cmpAABBs(const void *a, const void *b) {
  // Sort on minimum value along either x or y (specified in gSortAxis)
  float minA = ((AABB *)a)->min.xy[gSortAxis];
  float minB = ((AABB *)b)->min.xy[gSortAxis];
  if (minA < minB) return -1;
  if (minA > minB) return 1;
  return 0;
}

void sortAndSweepAABBArray(gAABBArray arr, size_t len, void (*callback)(void *, void *, void *), void *extra_params) {
  qsort(arr, len, sizeof(AABB), cmpAABBs);

  for (int i=0; i<len; i++) {
    for (int j=i+1; j<len; j++) {
      if (arr[j].min.xy[gSortAxis] > arr[i].max.xy[gSortAxis]) {
        break;
      } else {
        (*callback)(arr[i].obj, arr[j].obj, extra_params);
      }
    }
  }
}
