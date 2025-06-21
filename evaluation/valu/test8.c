#define __escape__(PTR) { asm volatile("" : : "g"(PTR) : "memory");  }
#define __escape_mem__() { asm volatile("" : : : "memory");  }

#define N 1000000

#define SIZE_I 128
#define SIZE_J 8
#define SIZE ((SIZE_I)*(SIZE_J))

int main()
{
    float a[SIZE], b[SIZE], c[SIZE];

    #pragma nounroll
    #pragma clang loop vectorize(disable)
    for(int repeat = 0; repeat<N; repeat++) {
      __escape__(b);
      __escape__(c);
  
      for (int i = 0; i < SIZE_I; ++i)
        for (int j = 0; j < SIZE_J; ++j)
          a[i*SIZE_J + j] = b[i*SIZE_J + j] * c[i*SIZE_J + j];
  
      __escape__(a);
    }
    return 0;
}
