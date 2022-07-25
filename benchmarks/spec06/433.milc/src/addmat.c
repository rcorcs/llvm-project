/********************  addmat.c (in su3.a)  *****************************
*									*
*  Add two SU3 matrices 						*
*/
#include "config.h"
#include "complex.h"
#include "su3.h"

void add_su3_matrix( su3_matrix *a, su3_matrix *b, su3_matrix *c ) {
register int i,j;
    for(i=0;i<3;i++)for(j=0;j<3;j++){
	CADD( a->e[i][j], b->e[i][j], c->e[i][j] );
    }
}
