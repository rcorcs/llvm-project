//----------------------------  block_block_sparse_matrix.cc  ---------------------------
//    $Id: block_sparse_matrix.cc,v 1.1 2004/09/14 00:51:21 wolf Exp $
//    Version: $Name:  $
//
//    Copyright (C) 2000, 2001, 2002 by the deal authors
//
//    This file is subject to QPL and may not be  distributed
//    without copyright and license information. Please refer
//    to the file deal.II/doc/license.html for the  text  and
//    further information on this license.
//
//----------------------------  block_block_sparse_matrix.cc  ---------------------------

#include <lac/block_sparse_matrix.templates.h>

// explicit instantiations
template class BlockSparseMatrix<double>;
template class BlockSparseMatrix<float>;
