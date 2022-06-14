#include <stdio.h>
void foo() {

  int a = scanf("%d", &a);
  int b = scanf("%d", &b);
  int c = scanf("%d", &c);
  int d = scanf("%d", &d);
  int e = scanf("%d", &e);
  int f = scanf("%d", &f);

  if (a > 0) {
    if (b == 2) {
      b *= 5;
      b++;
    }

    a++;
    b--;
    c *= 10;
  } else {
    if (e == 1)
      e *= 10;
    e++;
    f--;
    c *= 10;
  }

  printf("a=%d\n"
          "b=%d\n"
          "c=%d\n"
          "d=%d\n"
          "e=%d\n"
          "f=%d\n",
          a,b,c,d,e,f);
}

int main() {
    foo();
}
