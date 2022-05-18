#include <stdio.h>

void foo(int threadIdx, int THREADS, int *shared, int sb) {

  for (int k = 2; k <= sb; k *= 2) {
    // Bitonic merge:
    for (int j = k / 2; j > 0; j /= 2) {
      for (int tid = threadIdx; tid < sb; tid += THREADS) {
        unsigned int ixj = tid ^ j;
        int a = shared[tid];
        if (ixj > tid) {
          if ((tid & k) == 0) {
            if (shared[tid] > shared[ixj]) {
              int temp = shared[tid];
              shared[tid] = shared[ixj];
              shared[ixj] = temp;
              a--;
            }
          } else {
            if (shared[tid] < shared[ixj]) {
              int temp = shared[tid];
              shared[tid] = shared[ixj];
              shared[ixj] = temp;
              a++;
            }
          }
          printf("%d", a);
        }
      }
    }
  }
}
