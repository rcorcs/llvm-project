

int foo(int);
void uses(int,int,int);

void dummy(int);

void example(int *A, int *B) {
  int v0 = A[0];
  int u0 = v0;
  if (B[0] > 0)
    v0 = foo(B[0]);
  dummy(v0);

  int v1 = A[1];
  int u1 = v1;
  if (B[1] > 0)
    v1 = foo(B[1]);
  dummy(v1);

  int v2 = A[2];
  int u2 = v2;
  if (B[2] > 0)
    v2 = foo(B[2]);
  dummy(v2);

  uses(u0,u1,u2);
}
