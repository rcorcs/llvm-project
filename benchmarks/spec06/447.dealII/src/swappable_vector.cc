//----------------------------  swappable_vector.cc  ---------------------------
//    $Id: swappable_vector.cc,v 1.1 2004/09/14 00:51:25 wolf Exp $
//    Version: $Name:  $
//
//    Copyright (C) 1999, 2000, 2001, 2002 by the deal authors
//
//    This file is subject to QPL and may not be  distributed
//    without copyright and license information. Please refer
//    to the file deal.II/doc/license.html for the  text  and
//    further information on this license.
//
//----------------------------  swappable_vector.cc  ---------------------------


#include <lac/swappable_vector.templates.h>

// explicit instantiations
template class SwappableVector<double>;
template class SwappableVector<float>;
