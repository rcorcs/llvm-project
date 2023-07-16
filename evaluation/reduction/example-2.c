
#include <math.h>
#define UNROLL_COUNT 8

#define ABS fabsf
#define real_t float

//#define iterations 100000
#define LEN_1D 32000
#define LEN_2D 256


void dummy(real_t *a, real_t max);

real_t s3113(int iterations, int nl, real_t *a)
{

//    reductions
//    maximum of absolute value

    real_t max;
    for (int nl = 0; nl < iterations*4; nl++) {
        max = ABS(a[0]);
        #pragma clang loop unroll_count(UNROLL_COUNT)
        for (int i = 0; i < LEN_1D; i++) {
            if ((ABS(a[i])) > max) {
                max = ABS(a[i]);
            }
        }
        dummy(a, max);
    }

    return max;
}

