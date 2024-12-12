

float foo(float);
void dummy(float);

void example(float *A, float *B) {
  float v = A[0];
  if (B[0] > (float)0.)
    v = foo(B[0]);
  dummy(v);

  v = A[1];
  if (B[1] > (float)0.)
    v = foo(B[1]);
  dummy(v);

  v = A[2];
  if (B[2] > (float)0.)
    v = foo(B[2]);
  dummy(v);
}
