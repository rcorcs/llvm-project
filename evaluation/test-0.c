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
int foo(int b) {
  return divBy(addSqred(0,1) + b, 2) +
  divBy(addSqred(0,1) + 4, 2);
}


int main() {
  printf("%d\n",foo(2));
  return 0;
}
