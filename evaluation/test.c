

int addSqred(int a, int b) {
  return a*a + b*b;
}

int divBy(int a, int b) {
  return a/b;
}

int foo(int a, int b, int x) {
  return divBy(addSqred(a,a) + b, 2) +
  divBy(addSqred(x,x) + 4, 2);
}

