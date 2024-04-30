
void setup(int *, int);
void bar(int *A, int *B, int *C, int n, int fact);

void foo(int *A, int *B, int *C, int n) {
  setup(A,n);
  setup(B,n);
  setup(C,n);
  for (int i = 0; i<n; i++) {
    A[i] = B[i] * C[i];
  }
}

