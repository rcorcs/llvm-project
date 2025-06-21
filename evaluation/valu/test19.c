#include <stdlib.h>

#define __escape__(PTR) { asm volatile("" : : "g"(PTR) : "memory");  }
#define __escape_mem__() { asm volatile("" : : : "memory");  }

#define N 5000

//#define SIZE_I 200
//#define SIZE_J 200
//#define SIZE ((SIZE_I)*(SIZE_J))
#define SIZE 262144

int main()
{
    int NSIZE = SIZE;
    __escape__(&NSIZE);
    float *fy = (float*)malloc(sizeof(float)*NSIZE);
    float *fx = (float*)malloc(sizeof(float)*NSIZE);
    float *fv = (float*)malloc(sizeof(float)*NSIZE);
    float *fw = (float*)malloc(sizeof(float)*NSIZE);
    float *fz = (float*)malloc(sizeof(float)*NSIZE);
    double *dy = (double*)malloc(sizeof(double)*NSIZE);
    double *dx = (double*)malloc(sizeof(double)*NSIZE);
    double *dv = (double*)malloc(sizeof(double)*NSIZE);
    double *dw = (double*)malloc(sizeof(double)*NSIZE);
    double *dz = (double*)malloc(sizeof(double)*NSIZE);
    int *iy = (int*)malloc(sizeof(int)*NSIZE);
    int *ix = (int*)malloc(sizeof(int)*NSIZE);
    int *iv = (int*)malloc(sizeof(int)*NSIZE);
    int *iw = (int*)malloc(sizeof(int)*NSIZE);
    int *iz = (int*)malloc(sizeof(int)*NSIZE);

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
      __escape__(ix);
      __escape__(iv);
      __escape__(iw);
      __escape__(iz);

      for (int i = 0; i < NSIZE; ++i) {
          iy[i] = ix[i]*iw[i] + iv[i]*iz[i];
          fy[i] = fx[i]*fw[i] + fv[i]*fz[i];
          dy[i] = dx[i]*dw[i] + dv[i]*dz[i];
      }
      __escape__(iy);
      __escape__(fy);
      __escape__(dy);
    }

    free(fy);
    free(fx);
    free(fv);
    free(fw);
    free(fz);
    free(dy);
    free(dx);
    free(dv);
    free(dw);
    free(dz);
    free(iy);
    free(ix);
    free(iv);
    free(iw);
    free(iz);

    return 0;
}
