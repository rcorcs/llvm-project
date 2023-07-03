

#define type float

void dummy();
void addI(type *A, unsigned i);


void example(type *A, type *B, type factor) {
  dummy();
  A[0] += (type)0;
  if (B[0] > (type)0.)
    A[0] = B[0]*factor;
  A[1] += (type)1;
  if (B[1] > (type)0.)
    A[1] = B[1]*factor;
  A[2] += (type)2;
  if (B[2] > (type)0.)
    A[2] = B[2]*factor;
  A[3] += (type)3;
  if (B[3] > (type)0.)
    A[3] = B[3]*factor;
}
