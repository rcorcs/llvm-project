#define __escape__(PTR) { asm volatile("" : : "g"(PTR) : "memory");  }
#define __escape_mem__() { asm volatile("" : : : "memory");  }

#define N 10000

#define SIZE_I 512
#define SIZE_J 512
#define SIZE ((SIZE_I)*(SIZE_J))

int main()
{
    int NSIZE = SIZE;
    __escape__(&NSIZE);
    float x[NSIZE], w[NSIZE], z[NSIZE];
    char a[NSIZE], r[NSIZE], g[NSIZE], b[NSIZE];

    #pragma nounroll
    #pragma clang loop vectorize(disable)
    for(int repeat = 0; repeat<N; repeat++) {
      __escape__(w);
      __escape__(z);
      __escape__(r);
      __escape__(g);
      __escape__(b);

      for (int i = 0; i < NSIZE; ++i) {
          x[i] = w[i] + z[i];
          a[i] = (r[i] + g[i] + b[i] )/3;
      }
      __escape__(a);
      __escape__(x);
    }
    return 0;
}
