#define __escape__(PTR) { asm volatile("" : : "g"(PTR) : "memory");  }
#define __escape_mem__() { asm volatile("" : : : "memory");  }

#define N 10000

#define SIZE_I 256
#define SIZE_J 256
#define SIZE ((SIZE_I)*(SIZE_J))

int main()
{
    int NSIZE = SIZE;
    __escape__(&NSIZE);
    float fv[NSIZE], fx[NSIZE], fy[NSIZE], fw[NSIZE], fz[NSIZE];
    double dv[NSIZE], dx[NSIZE], dy[NSIZE], dw[NSIZE], dz[NSIZE];
    int a[NSIZE], r[NSIZE], g[NSIZE], b[NSIZE];

    #pragma nounroll
    #pragma clang loop vectorize(disable)
    for(int repeat = 0; repeat<N; repeat++) {
      __escape__(fx);
      __escape__(fv);
      __escape__(fw);
      __escape__(fz);
      __escape__(dx);
      __escape__(dv);
      __escape__(dw);
      __escape__(dz);
      __escape__(r);
      __escape__(g);
      __escape__(b);

      for (int i = 0; i < NSIZE; ++i) {
          fy[i] = fx[i]*fw[i] + fv[i]*fz[i];
          dy[i] = dx[i]*dw[i] + dv[i]*dz[i];
          a[i] = (r[i] + g[i] + b[i] )/3;
      }
      __escape__(a);
      __escape__(fy);
      __escape__(dy);
    }
    return 0;
}
