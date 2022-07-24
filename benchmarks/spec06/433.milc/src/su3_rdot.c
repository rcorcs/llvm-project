/*****************  su3_rdot.c  (in su3.a) ******************************
*									*
* double su3_rdot( su3_vector *a, su3_vector *b )			*
* return real part of dot product of two su3_vectors			*
*/
#include "config.h"
#include "complex.h"
#include "su3.h"

double su3_rdot( su3_vector *a, su3_vector *b ){

#ifndef NATIVEDOUBLE
register double temp1,temp2;
    temp2 = a->c[0].real * b->c[0].real;
    temp1 = a->c[0].imag * b->c[0].imag; temp2 += temp1;
    temp1 = a->c[1].real * b->c[1].real; temp2 += temp1;
    temp1 = a->c[1].imag * b->c[1].imag; temp2 += temp1;
    temp1 = a->c[2].real * b->c[2].real; temp2 += temp1;
    temp1 = a->c[2].imag * b->c[2].imag; temp2 += temp1;
    return(temp2);

#else /* RS6000 version */

  register double ar,ai,br,bi,ss;

  ar=a->c[0].real;  ai=a->c[0].imag;
  br=b->c[0].real;  bi=b->c[0].imag;
  ss = ar*br + ai*bi;

  ar=a->c[1].real;  ai=a->c[1].imag;
  br=b->c[1].real;  bi=b->c[1].imag;
  ss += ar*br + ai*bi;

  ar=a->c[2].real;  ai=a->c[2].imag;
  br=b->c[2].real;  bi=b->c[2].imag;
  ss += ar*br + ai*bi;

  return(ss);

#endif
}
