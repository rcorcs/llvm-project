#include <stdio.h>
#include <stdlib.h>
void foo(int *a, int *n) {
  int v = 0;
  if (*n > 0) {
    a[0]++;
    a[9]--;
  } else {
    a[0]--;
  }
  for (int i = 0; i < 10; i++)
    printf("%d\n", a[i]);
}

int main() {
  int *a = (int *)malloc(sizeof(int) * 10);
  int n;
  for (int i = 0; i < 10; i++)
    a[i] = i;

  scanf("%d", &n);
  foo(a, &n);
  free(a);
}
