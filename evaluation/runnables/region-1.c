
#include <stdio.h>

#define type int

void example(type *A, type *B, type factor) {
  if (B[0] > (type)0)
    A[0] = B[0]+factor;
  if (B[1] > (type)0)
    A[1] = B[1]+factor;
  if (B[2] > (type)0)
    A[2] = B[2]+factor;
  if (B[3] > (type)0)
    A[3] = B[3]+factor;
}

int main() {
  unsigned n = 4*32;
  type A[n];
  type B[n];
  for (unsigned i = 0; i<n; i++) B[i] = (type)i;
  for (unsigned i = 0; i<n; i++) A[i] = 0;
  for (unsigned i = 0; i<n; i+=4) {
    printf("%d, %d, %d\n",i, A[i],B[i]);
    example(&A[i], &B[i], 2);
  }
  type s = 0;
  for (unsigned i = 0; i<n; i++) s += A[i];
  printf("%d\n", s);
  return 0;
}
