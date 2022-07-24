//----------------------------  multigrid.all_dimensions.cc  ---------------------------
//    $Id: multigrid.all_dimensions.cc,v 1.4 2006/01/23 23:49:36 wolf Exp $
//    Version: $Name:  $
//
//    Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003, 2004 by the deal.II authors
//
//    This file is subject to QPL and may not be  distributed
//    without copyright and license information. Please refer
//    to the file deal.II/doc/license.html for the  text  and
//    further information on this license.
//
//----------------------------  multigrid.all_dimensions.cc  ---------------------------



#include <lac/vector.h>
#include <lac/sparse_matrix.h>
#include <lac/block_sparse_matrix.h>
#include <multigrid/mg_transfer.h>
#include <multigrid/multigrid.templates.h>
#include <grid/tria.h>
#include <dofs/dof_constraints.h>
#include <multigrid/mg_dof_handler.h>
#include <multigrid/mg_dof_accessor.h>
#include <grid/tria_iterator.h>
#include <fe/fe.h>
#include <multigrid/mg_smoother.h>
#include <multigrid/mg_smoother.templates.h>
#include <lac/vector.h>
#include <lac/block_vector.h>
#include <lac/sparse_matrix.h>
#include <lac/block_sparse_matrix.h>

#include <algorithm>



template <class VECTOR>
void
MGSmootherContinuous<VECTOR>::set_zero_interior_boundary (const unsigned int level,
						  VECTOR&            u) const
{
  if (level==0)
    return;
  else
    for (std::vector<unsigned int>::const_iterator p=interior_boundary_dofs[level-1].begin();
	 p!=interior_boundary_dofs[level-1].end(); ++p)
      u(*p) = 0;
}


#ifdef MG_DEBUG
template <>
void
Multigrid<Vector<double> >::print_vector (const unsigned int level,
					  const Vector<double> &v,
					  const char *name) const
{
  abort ();
}



template <class VECTOR>
void
Multigrid<VECTOR>::print_vector (const unsigned int level,
				 const VECTOR &v,
				 const char *name) const
{
  Assert(false, ExcNotImplemented());
}
#endif




template <class VECTOR>
MGTransferPrebuilt<VECTOR>::~MGTransferPrebuilt () 
{}


template <class VECTOR>
void MGTransferPrebuilt<VECTOR>::prolongate (
  const unsigned int to_level,
  VECTOR&            dst,
  const VECTOR&      src) const 
{
  Assert ((to_level >= 1) && (to_level<=prolongation_matrices.size()),
	  ExcIndexRange (to_level, 1, prolongation_matrices.size()+1));

  prolongation_matrices[to_level-1]->vmult (dst, src);
}


template <class VECTOR>
void MGTransferPrebuilt<VECTOR>::restrict_and_add (
  const unsigned int   from_level,
  VECTOR       &dst,
  const VECTOR &src) const 
{
  Assert ((from_level >= 1) && (from_level<=prolongation_matrices.size()),
	  ExcIndexRange (from_level, 1, prolongation_matrices.size()+1));

  prolongation_matrices[from_level-1]->Tvmult_add (dst, src);
}





template <typename number>
MGTransferBlock<number>::~MGTransferBlock () 
{}


template <typename number>
void MGTransferBlock<number>::prolongate (
  const unsigned int   to_level,
  BlockVector<number>       &dst,
  const BlockVector<number> &src) const 
{
  Assert ((to_level >= 1) && (to_level<=prolongation_matrices.size()),
	  ExcIndexRange (to_level, 1, prolongation_matrices.size()+1));

  unsigned int k=0;
  for (unsigned int b=0; b<src.n_blocks();++b)
    {
      if (!selected[k])
	++k;
      prolongation_matrices[to_level-1]->block(k,k).vmult (dst.block(b), src.block(b));
      ++k;
    }
}


template <typename number>
void MGTransferBlock<number>::restrict_and_add (
  const unsigned int   from_level,
  BlockVector<number>       &dst,
  const BlockVector<number> &src) const 
{
  Assert ((from_level >= 1) && (from_level<=prolongation_matrices.size()),
	  ExcIndexRange (from_level, 1, prolongation_matrices.size()+1));

  unsigned int k=0;
  for (unsigned int b=0; b<src.n_blocks();++b)
    {
      if (!selected[k])
	++k;
      prolongation_matrices[from_level-1]->block(k,k).Tvmult_add (dst.block(b), src.block(b));
      ++k;
    }
}




template <typename number>
MGTransferSelect<number>::~MGTransferSelect () 
{}


template <typename number>
void MGTransferSelect<number>::prolongate (
  const unsigned int   to_level,
  Vector<number>       &dst,
  const Vector<number> &src) const 
{
  Assert ((to_level >= 1) && (to_level<=prolongation_matrices.size()),
	  ExcIndexRange (to_level, 1, prolongation_matrices.size()+1));

      prolongation_matrices[to_level-1]->block(mg_selected_component,
					       mg_selected_component)
	.vmult (dst, src);
}


template <typename number>
void MGTransferSelect<number>::restrict_and_add (
  const unsigned int   from_level,
  Vector<number>       &dst,
  const Vector<number> &src) const
{
  Assert ((from_level >= 1) && (from_level<=prolongation_matrices.size()),
	  ExcIndexRange (from_level, 1, prolongation_matrices.size()+1));

  prolongation_matrices[from_level-1]->block(mg_selected_component,
					     mg_selected_component)
    .Tvmult_add (dst, src);
}


// Explicit instantiations

template class Multigrid<Vector<float> >;
template class Multigrid<Vector<double> >;
template class Multigrid<BlockVector<float> >;
template class Multigrid<BlockVector<double> >;

template class MGTransferPrebuilt<Vector<float> >;
template class MGTransferPrebuilt<BlockVector<float> >;
template class MGTransferPrebuilt<Vector<double> >;
template class MGTransferPrebuilt<BlockVector<double> >;
template class MGTransferBlock<float>;
template class MGTransferBlock<double>;
template class MGTransferSelect<float>;
template class MGTransferSelect<double>;

template
void MGSmootherContinuous<Vector<double> >::set_zero_interior_boundary (
  const unsigned int,
  Vector<double>&) const;
template
void MGSmootherContinuous<Vector<float> >::set_zero_interior_boundary (
  const unsigned int,
  Vector<float>&) const;
template
void MGSmootherContinuous<BlockVector<double> >::set_zero_interior_boundary (
  const unsigned int,
  BlockVector<double>&) const;
template
void MGSmootherContinuous<BlockVector<float> >::set_zero_interior_boundary (
  const unsigned int,
  BlockVector<float>&) const;
