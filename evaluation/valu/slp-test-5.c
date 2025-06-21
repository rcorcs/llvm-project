#include <stdio.h>

#define __escape__(PTR) { asm volatile("" : : "g"(PTR) : "memory");  }
#define __escape_mem__() { asm volatile("" : : : "memory");  }


#define N 10000000
//#define SIZE 4101//4096+5
#define SIZE 4096//4096+3

int main()
{
    float a[SIZE], b[SIZE], c[SIZE];

    #pragma nounroll
    for(int repeat = 0; repeat<N; repeat++) {
      __escape__(b);
      __escape__(c);

      float s = 0.f;
      for (size_t i = 0; i < SIZE; i+=8) {
        s += b[i] * c[i];
        s += b[i+1] * c[i+1];
        s += b[i+2] * c[i+2];
        s += b[i+3] * c[i+3];
        s += b[i+4] * c[i+4];
        s += b[i+5] * c[i+5];
        s += b[i+6] * c[i+6];
        s += b[i+7] * c[i+7];
      }
      
      __escape__(&s);
    }

    return 0;
}

