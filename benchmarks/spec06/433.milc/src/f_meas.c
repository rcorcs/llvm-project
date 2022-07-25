/**************** f_meas.c ***************************************/
/* MIMD version 6 */
/* CD 7/14/01 allow for multiple stochastic estimators NPBP_REPS */
/* DT 12/97 */
/* Kogut-Susskind fermions  -- this version for "fat plus Naik"
   or general "even plus odd" quark actions.  Assumes "dslash" has
   been defined to be the appropriate "dslash_fn" or "dslash_eo"
*/

/* Measure fermionic observables:
    psi-bar-psi (separately on even and odd sites)
    fermion action

    This routine uses g_rand, phi, xxx, and other vectors used by
    the matrix inversion.
*/

#include "generic_ks_includes.h"	/* definitions files and prototypes */


void f_meas_imp( field_offset phi_off, field_offset xxx_off, double mass ){
    double r_psi_bar_psi_even, r_psi_bar_psi_odd;
    double r_ferm_action;
    /* local variables for accumulators */
    register int i;
    register site *st;
    double rpbp_e,rpbp_o,rfaction;
    complex cc;

    /* If this feature is used more commonly, we should make npbp_reps
       a user-supplied parameter, instead of a macro and define it
       globally */

#ifdef NPBP_REPS
    int npbp_reps = NPBP_REPS;  /* Number of repetitions of stochastic
                                   estimate */
#else
    int npbp_reps = 1;
#endif
    int jpbp_reps;

#ifdef FN
    if(!(valid_longlinks==1)) load_longlinks();
    if(!(valid_fatlinks==1)) load_fatlinks();
#endif

    for(jpbp_reps = 0; jpbp_reps < npbp_reps; jpbp_reps++){
      rpbp_e = rpbp_o = rfaction = 0.0;
      
      /* Make random source, and do inversion */
      grsource_imp( phi_off, mass, EVENANDODD );
      mat_invert_uml( F_OFFSET(g_rand), xxx_off, phi_off, mass );
      
      /* fermion action = phi.xxx */
      /* psi-bar-psi on even sites = g_rand.xxx */
      FOREVENSITES(i,st){
        cc = su3_dot( (su3_vector *)F_PT(st,phi_off),
		      (su3_vector *)F_PT(st,xxx_off) );
	rfaction += cc.real;
        cc = su3_dot( &(st->g_rand), (su3_vector *)F_PT(st,xxx_off) );
	rpbp_e += cc.real;
      }
      /* psi-bar-psi on odd sites */
      FORODDSITES(i,st){
	cc = su3_dot( &(st->g_rand), (su3_vector *)F_PT(st,xxx_off) );
	rpbp_o += cc.real;
      }
      
      g_doublesum( &rpbp_o );
      g_doublesum( &rpbp_e );
      g_doublesum( &rfaction );
      
      r_psi_bar_psi_odd =  rpbp_o*(2.0/(double)volume) ;
      r_psi_bar_psi_even =  rpbp_e*(2.0/(double)volume) ;
      r_ferm_action =  rfaction*(1.0/(double)volume) ;
      node0_printf("PBP: mass %e     %e  %e ( %d of %d )\n",mass,
		   r_psi_bar_psi_even,r_psi_bar_psi_odd,jpbp_reps+1,npbp_reps);
      node0_printf("FACTION: mass = %e,  %e ( %d of %d )\n",mass,
		   r_ferm_action,jpbp_reps+1,npbp_reps);
    }
}
