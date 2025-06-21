#define __escape__(PTR) { asm volatile("" : : "g"(PTR) : "memory");  }
#define __escape_mem__() { asm volatile("" : : : "memory");  }

#define N 10000

#define SIZE_I 512
#define SIZE_J 512
#define SIZE ((SIZE_I)*(SIZE_J))


int main()
{
    float a[SIZE], b[SIZE], c[SIZE], d[SIZE];


    #pragma nounroll
    for(int repeat = 0; repeat<N; repeat++) {
      __escape__(b);
      __escape__(c);
      __escape__(d);
  
      for (int i = 0; i < SIZE_I; ++i) {
        int k = 1;
        for (int j = 0; j < SIZE_J; ++j) {
          d[i*SIZE_J + (k-1)] += d[i*SIZE_J + (k%SIZE_J)];
          a[i*SIZE_J + j] = b[i*SIZE_J + j] * c[i*SIZE_J + j];
          k++;
        }
      }
  
      __escape__(a);
      __escape__(d);
    }
    return 0;
}
