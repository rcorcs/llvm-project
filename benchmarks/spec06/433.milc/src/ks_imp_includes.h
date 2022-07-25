/****************** ks_imp_includes.h ******************************/
/*
*  Include files for Kogut-Susskind dynamical improved action application
*/

/* Include files */
#include "config.h"  /* Keep this first */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "complex.h"
#include "su3.h"
#include "lattice.h"
#include "macros.h"
#include "comdefs.h"
#include "io_lat.h"
#include "generic_ks.h"
#include "generic.h"

#ifdef FN
#define dslash dslash_fn
#endif
#ifdef EO
#define dslash dslash_eo
#endif

/* prototypes for functions in high level code */
int setup();
int readin(int prompt);
int update();
void update_h( double eps );
void update_u( double eps );
double hmom_action( );
double fermion_action( );


void g_measure( void );
void gauge_field_copy(field_offset src,field_offset dest);
