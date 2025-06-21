#include <stdio.h>

#define __escape__(PTR) { asm volatile("" : : "g"(PTR) : "memory");  }
#define __escape_mem__() { asm volatile("" : : : "memory");  }

#define N 2000000
#define SIZE 1512

int main()
{
    float a[SIZE], b[SIZE], c[SIZE];
    int start = 3;
    int n = 1193;
//    int n = -17;
    
    #pragma nounroll
    #pragma clang loop vectorize(disable)
    for (int i = 0; i<SIZE; i++) {
      b[i] = i;
      c[i] = SIZE-i;
      a[i] = 0;
    }
    
    #pragma nounroll
    #pragma clang loop vectorize(disable)
    for(int repeat = 0; repeat<N; repeat++) {
      __escape__(b);
      __escape__(c);
      __escape__(&n);
      __escape__(&start);

      for (int i = start; i < n; ++i)
        a[i] = b[i]*c[i];

      __escape__(a);
    }

    #pragma nounroll
    #pragma clang loop vectorize(disable)
    for (int i = 0; i<SIZE; i++) {
      printf("%f ", a[i]);
    }
    printf("\n");
    return 0;
}

