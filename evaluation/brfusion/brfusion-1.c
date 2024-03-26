


void setup(int *, int);
int dummy(int);
int diff(int);
void print(int*,int);

void foo(int c, int *A, int *B, int *C, int n) {
  if (c>0) {
    
    setup(A,n);
    setup(B,n);
    setup(C,n);
    for (int i = 0; i<n; i++)
      A[i] = dummy(B[i])*dummy(c);
    print(A,n);
  } else {
    c = diff(c);
    setup(A,n);
    setup(B,n);
    setup(C,n);
    for (int i = 0; i<n; i++)
      A[i] = dummy(C[i])*dummy(c);
    print(A,n);
  }
}
