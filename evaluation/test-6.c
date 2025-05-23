#include <stdio.h>

//__attribute__((noinline)) 
int addSqred(int a, int b) {
  return a*a + b*b;
}

//__attribute__((noinline)) 
int divBy(int a, int b) {
  return a/b;
}

int a;
int b;
int x;

int foo() {
  return divBy(addSqred(a,a) + b, 2) +
  divBy(addSqred(x,x) + 4, 2) + divBy(addSqred(x+1,2), 1);
}

int main() {
  printf("%d\n",foo());
  return 0;
}
