#ifndef _PARAMS_H
#define _PARAMS_H

#include "macros.h"  /* For MAXFILENAME */

/* structure for passing simulation parameters to each node */
typedef struct {
	int stopflag;   /* 1 if it is time to stop */
    /* INITIALIZATION PARAMETERS */
	int nx,ny,nz,nt;  /* lattice dimensions */
	int iseed;	/* for random numbers */
	int nflavors;	/* the number of flavors */
    /*  REPEATING BLOCK */
	int warms;	/* the number of warmup trajectories */
	int trajecs;	/* the number of real trajectories */
	int steps;	/* number of steps for updating */
	int propinterval;     /* number of trajectories between measurements */
	double beta,mass; /* gauge coupling, quark mass */
	double u0; /* tadpole parameter */
	int niter; 	/* maximum number of c.g. iterations */
	double rsqmin,rsqprop;  /* for deciding on convergence */
	double epsilon;	/* time step */
        int source_start, source_inc, n_sources; /* source time and increment */
	int startflag;  /* what to do for beginning lattice */
	int saveflag;   /* what to do with lattice at end */
	char startfile[MAXFILENAME],savefile[MAXFILENAME];
}  params;

#endif /* _PARAMS_H */
