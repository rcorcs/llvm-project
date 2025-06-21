#include <stdio.h>

#define __escape__(PTR) { asm volatile("" : : "g"(PTR) : "memory");  }
#define __escape_mem__() { asm volatile("" : : : "memory");  }


#define N 10000000
//#define SIZE 4101//4096+5
#define SIZE 4099//4096+3

int main()
{
    int a[SIZE], b[SIZE], c[SIZE];

    #pragma nounroll
    for(int repeat = 0; repeat<N; repeat++) {
      __escape__(b);
      __escape__(c);

      for (size_t i = 0; i < SIZE; ++i)
        a[i] = b[i] * c[i] + 10;

      __escape__(a);
    }

    return 0;
}

