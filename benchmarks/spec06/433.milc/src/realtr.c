/******************  realtr.c  (in su3.a) *******************************
*									*
* double realtrace_su3( su3_matrix *a,*b)				*
* return Re( Tr( A_adjoint*B )  					*
*/
#include "config.h"
#include "complex.h"
#include "su3.h"

double realtrace_su3(  su3_matrix *a, su3_matrix *b ){
register int i,j;
register double sum;
    for(sum=0.0,i=0;i<3;i++)for(j=0;j<3;j++)
	sum+= a->e[i][j].real*b->e[i][j].real + a->e[i][j].imag*b->e[i][j].imag;
    return(sum);
}
