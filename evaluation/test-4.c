
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

int dummy2(int a, int b, int c) {
  return a*b*c;
}

int a;
int b;
int x;

int foo() {
  dummy(a,b,x);
  return divBy(addSqred(a,a) + b, 2) +
  divBy(addSqred(x,x) + 4, 2) + dummy2(1,1,1);
}

