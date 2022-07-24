/********************  addvec.c  (in su3.a) *****************************
*									*
*  Add two SU3 vectors							*
*/
#include "config.h"
#include "complex.h"
#include "su3.h"

void add_su3_vector( su3_vector *a, su3_vector *b, su3_vector *c ){
register int i;
    for(i=0;i<3;i++){
	CADD( a->c[i], b->c[i], c->c[i] );
    }
}
