

void dummy();

void example(float *A, float *B, float factor) {
  dummy();
  if (B[0] > (float)0.)
    A[0] = B[0]*factor;
  if (B[1] > (float)0.)
    A[1] = B[1]*factor;
  if (B[2] > (float)0.)
    A[2] = B[2]*factor;
  if (B[3] > (float)0.)
    A[3] = B[3]*factor;
}
