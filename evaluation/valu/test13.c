#include <stdio.h>

#define __escape__(PTR) { asm volatile("" : : "g"(PTR) : "memory");  }
#define __escape_mem__() { asm volatile("" : : : "memory");  }

#define N 1000000
#define SIZE 1024*4

int main()
{
    float a[SIZE], b[SIZE], c[SIZE];
    
    #pragma nounroll
    #pragma clang loop vectorize(disable)
    for (int i = 0; i<SIZE; i++) {
      b[i] = i;
      c[i] = SIZE-i;
    }

    int external = 1;
    int sum = 1;
    
    #pragma nounroll
    #pragma clang loop vectorize(disable)
    for(int repeat = 0; repeat<N; repeat++) {
      __escape__(b);
      __escape__(c);

      __escape__(&external);
      __escape__(&sum);

       
      for (int i = 0; i < SIZE; ++i) {
        sum += external;
        a[i] = b[i] + c[i] * sum;
      }

      __escape__(a);
      __escape__(&sum);
    }

    #pragma nounroll
    #pragma clang loop vectorize(disable)
    for (int i = 0; i<SIZE; i++) {
      printf("%f ", a[i]);
    }
    printf("\n");
    return 0;
}
