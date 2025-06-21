#include <stdio.h>

#define __escape__(PTR) { asm volatile("" : : "g"(PTR) : "memory");  }
#define __escape_mem__() { asm volatile("" : : : "memory");  }


#define N 10000000
#define SIZE 1512

int main()
{
    int n = 4096;
    __escape__(&n);
    double a[n], b[n], c[n];
//    int n = -17;
    
    #pragma nounroll
    #pragma clang loop vectorize(disable)
    for (int i = 0; i<n; i++) {
      b[i] = i;
      c[i] = n-i;
      a[i] = 0;
    }
    double num = 3.1415;
    __escape__(&num);

    #pragma nounroll
    for(int repeat = 0; repeat<N; repeat++) {
      __escape__(b);
      __escape__(c);
      for (int i = 0; i < n; ++i)
        a[i] = b[i];
      __escape__(a);
    }

    #pragma nounroll
    #pragma clang loop vectorize(disable)
    for (int i = 0; i<n; i++) {
      printf("%f ", a[i]);
    }
    printf("\n");
    return 0;
}

