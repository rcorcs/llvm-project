#include <stdio.h>
#include <stdlib.h>
#include "specrand.h"

int main(int argc, char **argv) {
  int seed;
  int count, i;

  if (argc < 2) {
    printf("Please supply seed and count\n");
    return(1);
  }
  seed = atoi(argv[1]);
  count = atoi(argv[2]);

  printf("seed = %d\ncount = %d\n", seed, count);
  printf("%%f sequence:\n");
  spec_srand(seed);
  for(i = 0; i < count; i++) {
    printf("%f\n", spec_rand());
  }
  printf("%%d sequence (1-2048000):\n");
  for(i = 0; i < count; i++) {
    printf("%d\n", (int)((spec_rand()*2048000)+1));
  }

  return(0);
}
