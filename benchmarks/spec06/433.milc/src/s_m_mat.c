/******************  s_m_mat.c  (in su3.a) ******************************
*									*
* void scalar_mult_su3_matrix( su3_matrix *a, double s, su3_matrix *b)	*
* B <- s*A								*
*/
#include "config.h"
#include "complex.h"
#include "su3.h"

/* b <- s*a, matrices */
void scalar_mult_su3_matrix( su3_matrix *a, double s, su3_matrix *b ){

#ifndef FAST
register int i,j;
    for(i=0;i<3;i++)for(j=0;j<3;j++){
	b->e[i][j].real = s*a->e[i][j].real;
	b->e[i][j].imag = s*a->e[i][j].imag;
    }

#else
#ifdef NATIVEDOUBLE
  register double ss;
#else
  register double ss;
#endif

  ss = s;

  b->e[0][0].real = ss*a->e[0][0].real;
  b->e[0][0].imag = ss*a->e[0][0].imag;
  b->e[0][1].real = ss*a->e[0][1].real;
  b->e[0][1].imag = ss*a->e[0][1].imag;
  b->e[0][2].real = ss*a->e[0][2].real;
  b->e[0][2].imag = ss*a->e[0][2].imag;

  b->e[1][0].real = ss*a->e[1][0].real;
  b->e[1][0].imag = ss*a->e[1][0].imag;
  b->e[1][1].real = ss*a->e[1][1].real;
  b->e[1][1].imag = ss*a->e[1][1].imag;
  b->e[1][2].real = ss*a->e[1][2].real;
  b->e[1][2].imag = ss*a->e[1][2].imag;

  b->e[2][0].real = ss*a->e[2][0].real;
  b->e[2][0].imag = ss*a->e[2][0].imag;
  b->e[2][1].real = ss*a->e[2][1].real;
  b->e[2][1].imag = ss*a->e[2][1].imag;
  b->e[2][2].real = ss*a->e[2][2].real;
  b->e[2][2].imag = ss*a->e[2][2].imag;

#endif
}
