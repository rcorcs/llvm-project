/********************** cmplx.c (in complex.a) **********************/
/* MIMD version 6 */
/* Subroutines for operations on complex numbers */
/* make a complex number from two real numbers */
#include "config.h"
#include "complex.h"

complex cmplx( double x, double y )  {
    complex c;
    c.real = x; c.imag = y;
    return(c);
}
