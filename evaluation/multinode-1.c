#include <stdio.h>

__attribute__((noinline)) 
int addSqred(int a, int b) {
  return a*a + b*b;
}

__attribute__((noinline)) 
int divBy(int a, int b) {
  return a/b;
}

__attribute__((noinline)) 
int foo(int a, int b, int x) {
  return divBy(b + addSqred(a,a), 2) +
  divBy(addSqred(x,x) + 4, 2);
}

int main() {
  printf("%d\n",foo(1,2,3));
  return 0;
}
