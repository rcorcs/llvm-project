
#include <math.h>

#define ABS fabsf


float example(float *A, float max) {
  if ((ABS(A[0])) > max)
    max = ABS(A[0]);
  if ((ABS(A[1])) > max)
    max = ABS(A[1]);
  if ((ABS(A[2])) > max)
    max = ABS(A[2]);
  if ((ABS(A[3])) > max)
    max = ABS(A[3]);
  return max;
}
