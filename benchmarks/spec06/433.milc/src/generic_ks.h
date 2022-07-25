#ifndef _GENERIC_KS_H
#define _GENERIC_KS_H
/************************ generic_ks.h **********************************
*									*
*  Macros and declarations for generic_ks routines                      *
*  This header is for codes that call generic_ks routines               *
*  MIMD version 6 							*
*									*
*/

#include "complex.h"
#include "su3.h"
#include "generic_quark_types.h"
#include "comdefs.h"

int congrad( int niter, double rsqmin, int parity, double *rsq );
void copy_latvec(field_offset src, field_offset dest, int parity);
void dslash( field_offset src, field_offset dest, int parity );
void dslash_special( field_offset src, field_offset dest,
    int parity, msg_tag **tag, int start );
void clear_latvec(field_offset v,int parity);

void scalar_mult_latvec(field_offset src, double scalar,
			field_offset dest, int parity);
void scalar_mult_add_latvec(field_offset src1, field_offset src2,
			    double scalar, field_offset dest, int parity);
void scalar2_mult_add_su3_vector(su3_vector *a, double s1, su3_vector *b, 
				 double s2, su3_vector *c);

void scalar2_mult_add_latvec(field_offset src1,double scalar1,
			     field_offset src2,double scalar2,
			     field_offset dest,int parity);
void checkmul();
void phaseset();
void rephase( int flag );

int ks_congrad( field_offset src, field_offset dest, double mass,
     int niter, double rsqmin, int parity, double *rsq );

void cleanup_gathers(msg_tag *tags1[], msg_tag *tags2[]);
void cleanup_dslash_temps();
void dslash_fn( field_offset src, field_offset dest, int parity );
void dslash_fn_alltemp_special(su3_vector *src, su3_vector *dest,
			       int parity, msg_tag **tag, int start );
void dslash_fn_special( field_offset src, field_offset dest,
    int parity, msg_tag **tag, int start );
void dslash_fn_on_temp( su3_vector *src, su3_vector *dest, int parity );
void dslash_fn_on_temp_special(su3_vector *src, su3_vector *dest,
			       int parity, msg_tag **tag, int start );

void dslash_eo( field_offset src, field_offset dest, int parity );
void dslash_eo_special( field_offset src, field_offset dest,
    int parity, msg_tag **tag, int start );

int congrad_ks(            /* Return value is number of iterations taken */
     field_offset src,       /* type su3_vector* (preloaded source) */
     field_offset dest,      /* type su3_vector*  (answer and initial guess) */
     quark_invert_control *qic, /* inverter control */
     void *dmp               /* parameters defining the Dirac matrix */
     );

int ks_invert( /* Return value is number of iterations taken */
    field_offset src,   /* type su3_vector or multi_su3_vector 
			   (preloaded source) */
    field_offset dest,  /* type su3_vector or multi_su3_vector 
			   (answer and initial guess) */
    int (*invert_func)(field_offset src, field_offset dest,
			quark_invert_control *qic,
			void *dmp),
    quark_invert_control *qic, /* inverter control */
    void *dmp                 /* Passthrough Dirac matrix parameters */
    );

int ks_multicg(	/* Return value is number of iterations taken */
    field_offset src,	/* source vector (type su3_vector) */
    su3_vector **psim,	/* solution vectors */
    double *masses,	/* the masses */
    int num_masses,	/* number of masses */
    int niter,		/* maximal number of CG interations */
    double rsqmin,	/* desired residue squared */
    int parity,		/* parity to be worked on */
    double *final_rsq_ptr	/* final residue squared */
    );

/* f_meas.c */
void f_meas_imp( field_offset phi_off, field_offset xxx_off, double mass );

/* flavor_ops.c */
void sym_shift(int dir, field_offset src,field_offset dest) ;
void zeta_shift(int n, int *d, field_offset src, field_offset dest ) ;
void eta_shift(int n, int *d, field_offset src, field_offset dest ) ;


void mult_flavor_vector(int mu, field_offset src, field_offset dest ) ;
void mult_flavor_tensor(int mu, int nu, field_offset src, field_offset dest ) ;
void mult_flavor_pseudovector(int mu, field_offset src, field_offset dest ) ;
void mult_flavor_pseudoscalar(field_offset src, field_offset dest ) ;

void mult_spin_vector(int mu, field_offset src, field_offset dest ) ;
void mult_spin_tensor(int mu, int nu, field_offset src, field_offset dest ) ;
void mult_spin_pseudovector(int mu, field_offset src, field_offset dest ) ;
void mult_spin_pseudoscalar(field_offset src, field_offset dest ) ;

/* grsource.c */
void grsource(int parity);

/* grsource_imp.c */
void grsource_imp( field_offset dest, double mass, int parity);

/* mat_invert.c */
int mat_invert_cg( field_offset src, field_offset dest, field_offset temp,
		   double mass );
int mat_invert_uml(field_offset src, field_offset dest, field_offset temp,
		   double mass );
void check_invert( field_offset src, field_offset dest, double mass,
		   double tol);
/* nl_spectrum.c */
int nl_spectrum( double vmass, field_offset tempvec1, field_offset tempvec2,
		 field_offset tempmat1, field_offset tempmat2);

/* quark_stuff.c */
void make_path_table();
void eo_fermion_force( double eps, int nflavors, field_offset x_off );
void eo_fermion_force_3f( double eps, int nflav1, field_offset x1_off,
	int nflav2, field_offset x2_off  );
void load_longlinks();
void load_fatlinks();

/* spectrum.c */
int spectrum();

/* spectrum2.c */
int spectrum2( double vmass, field_offset temp1, field_offset temp2 );

/* spectrum_mom.c */
int spectrum_mom( double qmass, double amass, field_offset temp, double tol);

/* spectrum_multimom.c */
int spectrum_multimom( double dyn_mass, double low_mass, double mass_inc, int nmasses, double tol);

/* spectrum_nd.c */
int spectrum_nd( double mass1, double mass2, double tol );

/* spectrum_nlpi2.c */
int spectrum_nlpi2( double qmass, double amass, field_offset temp, double tol);

#endif /* _GENERIC_KS_H */
