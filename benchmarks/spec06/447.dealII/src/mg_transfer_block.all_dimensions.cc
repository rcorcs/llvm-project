//-----------------------------------------------------------------------
//    $Id: mg_transfer_block.all_dimensions.cc,v 1.1 2004/09/18 20:33:38 wolf Exp $
//    Version: $Name:  $
//
//    Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003, 2004 by the deal.II authors
//
//    This file is subject to QPL and may not be  distributed
//    without copyright and license information. Please refer
//    to the file deal.II/doc/license.html for the  text  and
//    further information on this license.
//
//-----------------------------------------------------------------------

#include <base/logstream.h>

#include <lac/vector.h>
#include <lac/block_vector.h>
#include <lac/sparse_matrix.h>
#include <lac/block_sparse_matrix.h>
#include <grid/tria.h>
#include <grid/tria_iterator.h>
#include <dofs/dof_tools.h>
#include <fe/fe.h>
#include <multigrid/mg_dof_handler.h>
#include <multigrid/mg_dof_accessor.h>
#include <multigrid/mg_transfer.h>
#include <multigrid/mg_transfer.templates.h>
#include <multigrid/mg_dof_tools.h>

#include <algorithm>


//TODO:[GK] Add all those little vectors.
unsigned int
MGTransferBlockBase::memory_consumption () const
{
  unsigned int result = sizeof(*this);
  result += sizeof(unsigned int) * sizes.size();
#ifdef DEAL_PREFER_MATRIX_EZ
  std::vector<boost::shared_ptr<SparseMatrixEZ<double> > >::const_iterator m;
  const std::vector<boost::shared_ptr<SparseMatrixEZ<double> > >::const_iterator end = prolongation_matrices.end();
  for (m = prolongation_matrices.begin(); m != end ; ++m)
    result += *m->memory_consumption();
#else
  for (unsigned int i=0;i<prolongation_matrices.size();++i)
    result += prolongation_matrices[i]->memory_consumption()
	      + prolongation_sparsities[i]->memory_consumption();
#endif
  return result;
}


