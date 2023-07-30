
#include <math.h>
#define UNROLL_COUNT 8

#define ABS fabsf
#define real_t float

//#define iterations 100000
#define LEN_1D 32000
#define LEN_2D 256


void dummy(real_t *a, real_t *b);

void s122(int iterations, int nl, int n1, int n3,real_t *a, real_t *b)
{

    int j, k;
    for (int nl = 0; nl < iterations; nl++) {
        j = 1;
        k = 0;
        #pragma clang loop unroll_count(UNROLL_COUNT)
        for (int i = n1-1; i < LEN_1D; i += n3) {
            k += j;
            a[i] += b[LEN_1D - k];
        }
        dummy(a,b);
    }
}

