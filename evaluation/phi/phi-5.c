

int foo(int);
void dummy(int);

void example(int *A, int *B) {
  int v = 101;
  if (B[0] > 0)
    v = foo(B[0]);
  dummy(v);

  v = 7;
  if (B[1] > 0)
    v = foo(B[1]);
  dummy(v);

  v = 317;
  if (B[2] > 0)
    v = foo(B[2]);
  dummy(v);
}
