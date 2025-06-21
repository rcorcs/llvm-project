#define __escape__(PTR) { asm volatile("" : : "g"(PTR) : "memory");  }
#define __escape_mem__() { asm volatile("" : : : "memory");  }

#define N 10000

#define SIZE_I 256
#define SIZE_J 256
#define SIZE ((SIZE_I)*(SIZE_J))

struct MyType {
  char cV;
  float fV;
  int iV;
  long lV;
  double dV;
};

int main()
{
    int NSIZE = SIZE;
    __escape__(&NSIZE);
    struct MyType Output[NSIZE];
    float fv[NSIZE], fx[NSIZE], fy[NSIZE], fw[NSIZE], fz[NSIZE];
    double dv[NSIZE], dx[NSIZE], dy[NSIZE], dw[NSIZE], dz[NSIZE];

    #pragma nounroll
    for(int repeat = 0; repeat<N; repeat++) {
      __escape__(fx);
      __escape__(fv);
      __escape__(fw);
      __escape__(fz);
      __escape__(dx);
      __escape__(dv);
      __escape__(dw);
      __escape__(dz);
      __escape__(fy);
      __escape__(dy);

      #pragma unroll(2)
      for (int i = 0; i < NSIZE; ++i) {
          Output[i].fV = fy[i] + fx[i]*fw[i] + fv[i]*fz[i];
          Output[i].dV = dy[i] + dx[i]*dw[i] + dv[i]*dz[i];
      }

      __escape__(Output);
    }
    return 0;
}
