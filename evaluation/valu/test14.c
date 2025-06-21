#include <stdio.h>

#define __escape__(PTR) { asm volatile("" : : "g"(PTR) : "memory");  }
#define __escape_mem__() { asm volatile("" : : : "memory");  }

#define N 1000000
#define SIZE 1024*4

int main()
{
    float a[SIZE], b[SIZE], c[SIZE], d[SIZE];
    
    #pragma nounroll
    #pragma clang loop vectorize(disable)
    for (int i = 0; i<SIZE; i++) {
      b[i] = i;
      c[i] = SIZE-i;
      d[i] = SIZE-i;
    }

    int external = 1;
    int k = 1;
    
    #pragma nounroll
    #pragma clang loop vectorize(disable)
    for(int repeat = 0; repeat<N; repeat++) {
      __escape__(b);
      __escape__(c);
      __escape__(d);

      __escape__(&external);
      __escape__(&k);

       
      for (int i = 0; i < SIZE; ++i) {
        a[i] = b[i] + c[i] * d[external*i - k];
      }

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
