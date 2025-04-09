
//__attribute__((noinline)) 
int addSqred(int a, int b) {
  return a*a + b*b;
}

//__attribute__((noinline)) 
int divBy(int a, int b) {
  return a/b;
}

void dummy(int a, int b, int c) {
  print("%d,%d,%d\n",a,b,c);
}

int foo(int a, int b, int x) {
  dummy(a,b,x);
  return divBy(addSqred(a,a) + b, 2) +
  divBy(addSqred(x,x) + 4, 2);
}

