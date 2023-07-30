
#include <math.h>
#define UNROLL_COUNT 8

#define ABS fabsf
#define real_t float

//#define iterations 100000
#define LEN_1D 32000
#define LEN_2D 256


void dummy(real_t *a, real_t *b, real_t *c, real_t *d, real_t *e);

void s124(int iterations, int nl, int n1, int n3,real_t *a, real_t *b, real_t *c, real_t *d, real_t *e)
{

    int j;
    for (int nl = 0; nl < iterations; nl++) {
        j = -1;
        #pragma clang loop unroll_count(UNROLL_COUNT)
        for (int i = 0; i < LEN_1D; i++) {
            if (b[i] > (real_t)0.) {
                j++;
                a[j] = b[i] + d[i] * e[i];
            } else {
                j++;
                a[j] = c[i] + d[i] * e[i];
            }
        }
        dummy(a, b, c, d, e);
    }
}

