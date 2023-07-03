

#define type float

void dummy(type *A, unsigned i);

void example(float *A, float *B, float factor) {
  dummy(A, 0);
  if (B[0] > (float)0.)
    A[0] = B[0]*factor;
  dummy(A, 1);
  if (B[1] > (float)0.)
    A[1] = B[1]*factor;
  dummy(A, 2);
  if (B[2] > (float)0.)
    A[2] = B[2]*factor;
  dummy(A, 3);
  if (B[3] > (float)0.)
    A[3] = B[3]*factor;
}
