#include <stdio.h>
void foo(int *a1, int *a2, int *a3, int *a4, int *a5, int *a6) {
  int a = *a1;
  int b = *a2;
  int c = *a3;
  int d = *a4;
  int e = *a5;
  int f = *a6;

  if (c == 0) {
    while (a > 0) {
      if (b < 0)
        b++;
      a--;
    }
  } else {
    while (d > 0) {
      if (e < 0)
        f++;
      d--;
    }
  }

  printf("a=%d\n"
         "b=%d\n"
         "c=%d\n"
         "d=%d\n"
         "e=%d\n"
         "f=%d\n",
         a, b, c, d, e, f);
}

int main() {
  int a, b, c, d, e, f;
  scanf("%d", &a);
  scanf("%d", &b);
  scanf("%d", &c);
  scanf("%d", &d);
  scanf("%d", &e);
  scanf("%d", &f);

  foo(&a, &b, &c, &d, &e, &f);
}
