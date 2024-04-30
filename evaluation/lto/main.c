
#include <stdio.h>
#include <stdlib.h>

void foo(int *A, int *B, int *C, int n);
void bar(int *A, int *B, int *C, int n, int fact);

void setup(int *A, int n) {
  for (int i = 0; i<n; i++) A[i] = i;
}

int main() {
  int n = 1024*64;
  int *A = malloc(n*sizeof(int));
  int *B = malloc(n*sizeof(int));
  int *C = malloc(n*sizeof(int));
  foo(A,B,C,n);
  bar(A,B,C,n,2);
  for (int i = 0; i<n; i++) {
    printf("%d ", A[i]);
  }
  printf("\n");
  free(A);
  free(B);
  free(C);
  return 0;
}
