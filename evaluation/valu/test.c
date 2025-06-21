#include <stdio.h>

#define __escape__(PTR) { asm volatile("" : : "g"(PTR) : "memory");  }
#define __escape_mem__() { asm volatile("" : : : "memory");  }


#define N 2
//#define SIZE 4101//4096+5
#define SIZE 4096//4096+3

int main()
{
    float a[SIZE], b[SIZE], c[SIZE];
    #pragma nounroll
    for (size_t i = 0; i < SIZE; ++i) {
      b[i] = i;
      c[i] = SIZE-i;
    }

    #pragma nounroll
    for(int repeat = 0; repeat<N; repeat++) {
      __escape__(a);
      __escape__(b);
      __escape__(c);

      float s = 0.f;
      for (size_t i = 0; i < SIZE; ++i) {
        s += b[i] * c[i];
      }
      
      printf("%f\n", s);

      __escape__(a);
      __escape__(&s);
    }

    return 0;
}

