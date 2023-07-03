

void example(float *A, float *B, float factor, float a0, float a1, float a2, float a3) {
  if (B[0] > (float)0.)
    A[0] = a0+B[0]*factor;
  if (B[1] > (float)0.)
    A[1] = a1+B[1]*factor;
  if (B[2] > (float)0.)
    A[2] = a2+B[2]*factor;
  if (B[3] > (float)0.)
    A[3] = a3+B[3]*factor;
}
