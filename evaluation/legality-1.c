#include <stdio.h>

__attribute__((noinline)) 
int addSqred(int a, int b) {
  printf("computing addSqred...\n");
  return a*a + b*b;
}

__attribute__((noinline)) 
int divBy(int a, int b) {
  return a/b;
}

__attribute__((noinline)) 
void dummy(int a, int b, int c) {
  printf("%d,%d,%d\n",a,b,c);
}

__attribute__((noinline)) 
int foo(int a, int b, int x) {
  int v = addSqred(a,a);
  int tmp = v + b;
  dummy(a,b,x);
  return divBy(tmp, 2);
}

__attribute__((noinline)) 
int bar(int a, int b, int x) {
  int v = addSqred(x,x);
  int tmp = v + 4;
  dummy(a,b,x);
  return divBy(tmp, 2);
}

int main() {
  printf("%d\n", foo(1,2,3));
  printf("%d\n", bar(1,2,3));
  return 0;
}
