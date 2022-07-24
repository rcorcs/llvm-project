//----------------------------  tria.all_dimensions.cc  ---------------------------
//    $Id: tria.all_dimensions.cc,v 1.2 2006/01/24 01:06:25 wolf Exp $
//    Version: $Name:  $
//
//    Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003, 2004 by the deal.II authors
//
//    This file is subject to QPL and may not be  distributed
//    without copyright and license information. Please refer
//    to the file deal.II/doc/license.html for the  text  and
//    further information on this license.
//
//----------------------------  tria.all_dimensions.cc  ---------------------------


/*
 * Single out some functions which are needed by all dimensions, but
 * which are not template. They thus have the same name and when we
 * try to link with the libraries for different dimensions at the same
 * time, we get linker errors for functions defined more than once. By
 * putting these functions in a single file, the linker is allowed to
 * use it only once and throw away all other versions of this file in
 * the other libraries.
 */


#include <base/memory_consumption.h>
#include <grid/tria.h>
#include <grid/tria_levels.h>
#include <grid/tria_boundary.h>

#include <algorithm>
#include <functional>
#include <numeric>



template <>
void
CellData<1>::rotate (const unsigned int)
{
  Assert (false, ExcNotPossible());
}



template <>
void
CellData<2>::rotate (const unsigned int times)
{
  Assert (times < 4, ExcInvalidRotation(times));
  
  for (unsigned int i=0; i<times; ++i)
    {
      const unsigned int x = vertices[0];
      vertices[0] = vertices[1];
      vertices[1] = vertices[2];
      vertices[2] = vertices[3];
      vertices[3] = x;
    };
}



template <>
void
CellData<3>::rotate (const unsigned int times)
{
  Assert (times < 24, ExcInvalidRotation(times));
  
				   // list the 24 ways to rotate a
				   // cell, by the ways by which the
				   // vertices are then permuted
  static const unsigned int rotations[24][GeometryInfo<3>::vertices_per_cell]
    = {   {0,1,2,3,4,5,6,7},
	  {1,5,6,2,0,4,7,3},
	  {5,4,7,6,1,0,3,2},
	  {4,0,3,7,5,1,2,6},
	  
	  {2,3,0,1,6,7,4,5},
	  {3,7,4,0,2,6,5,1},
	  {7,6,5,4,3,2,1,0},
	  {6,2,1,5,7,3,0,4},
	  
	  {0,3,7,4,1,2,6,5},
	  {3,2,6,7,0,1,5,4},
	  {2,1,5,6,3,0,4,7},
	  {1,0,4,5,2,3,7,6},
	  
	  {1,2,3,0,5,6,7,4},
	  {2,6,7,3,1,5,4,0},
	  {6,5,4,7,2,1,0,3},
	  {5,1,0,4,6,2,3,7},
	  
	  {5,6,2,1,4,7,3,0},
	  {6,7,3,2,5,4,0,1},
	  {7,4,0,3,6,5,1,2},
	  {4,5,1,0,7,6,2,3},
	  
	  {3,0,1,2,7,4,5,6},
	  {0,4,5,1,3,7,6,2},
	  {4,7,6,5,0,3,2,1},
	  {7,3,2,6,4,0,1,5}  };

				   // first copy over the old vertex
				   // numbers
  const unsigned int old_vertices[GeometryInfo<3>::vertices_per_cell]
    = { vertices[0], vertices[1], vertices[2], vertices[3],
	vertices[4], vertices[5], vertices[6], vertices[7]  };

				   // then copy them back in the new
				   // order
  for (unsigned int i=0; i<GeometryInfo<3>::vertices_per_cell; ++i)
    vertices[i] = old_vertices[rotations[times][i]];
}


bool
SubCellData::check_consistency (const unsigned int dim) const
{
  switch (dim) 
    {
      case 1:
	    return ((boundary_lines.size() == 0) &&
		    (boundary_quads.size() == 0));
      case 2:
	    return (boundary_quads.size() == 0);
    };
  return true;
}



void
TriangulationLevel<0>::reserve_space (const unsigned int total_cells,
				      const unsigned int dimension)
{
				   // we need space for total_cells
				   // cells. Maybe we have more already
				   // with those cells which are unused,
				   // so only allocate new space if needed.
				   //
				   // note that all arrays should have equal
				   // sizes (checked by @p{monitor_memory}
  if (total_cells > refine_flags.size()) 
    {
      refine_flags.reserve (total_cells);
      refine_flags.insert (refine_flags.end(),
			   total_cells - refine_flags.size(),
			   false);
      
      coarsen_flags.reserve (total_cells);
      coarsen_flags.insert (coarsen_flags.end(),
			    total_cells - coarsen_flags.size(),
			    false);
      
      subdomain_ids.reserve (total_cells);
      subdomain_ids.insert (subdomain_ids.end(),
			    total_cells - subdomain_ids.size(),
			    0);
      
      neighbors.reserve (total_cells*(2*dimension));
      neighbors.insert (neighbors.end(),
			total_cells*(2*dimension) - neighbors.size(),
			std::make_pair(-1,-1));
    };
}



void
TriangulationLevel<0>::monitor_memory (const unsigned int true_dimension) const
{
}



unsigned int
TriangulationLevel<0>::memory_consumption () const
{
  return (MemoryConsumption::memory_consumption (refine_flags) +
	  MemoryConsumption::memory_consumption (coarsen_flags) +
	  MemoryConsumption::memory_consumption (neighbors));
}



void
TriangulationLevel<1>::reserve_space (const unsigned int new_lines)
{
  const unsigned int new_size = new_lines +
				std::count_if (lines.used.begin(),
					       lines.used.end(),
					       std::bind2nd (std::equal_to<bool>(), true));

				   // same as in @p{reserve_space<0>}: only
				   // allocate space if necessary
  if (new_size>lines.lines.size()) 
    {
      lines.lines.reserve (new_size);
      lines.lines.insert (lines.lines.end(), new_size-lines.lines.size(), Line());
  
      lines.used.reserve (new_size);
      lines.used.insert (lines.used.end(), new_size-lines.used.size(), false);
  
      lines.user_flags.reserve (new_size);
      lines.user_flags.insert (lines.user_flags.end(),
			       new_size-lines.user_flags.size(), false);
      
      lines.children.reserve (new_size);
      lines.children.insert (lines.children.end(), new_size-lines.children.size(),
			     -1);

      lines.material_id.reserve (new_size);
      lines.material_id.insert (lines.material_id.end(),
				new_size-lines.material_id.size(),
				255);

      lines.user_pointers.reserve (new_size);
      lines.user_pointers.insert (lines.user_pointers.end(),
				  new_size-lines.user_pointers.size(), 0);
    };
}



void
TriangulationLevel<1>::monitor_memory (const unsigned int true_dimension) const
{
  TriangulationLevel<0>::monitor_memory (true_dimension);
}



unsigned int
TriangulationLevel<1>::memory_consumption () const
{
  return (TriangulationLevel<0>::memory_consumption() +
	  MemoryConsumption::memory_consumption (lines.lines) +
	  MemoryConsumption::memory_consumption (lines.children) +
	  MemoryConsumption::memory_consumption (lines.used) +
	  MemoryConsumption::memory_consumption (lines.user_flags) +
	  MemoryConsumption::memory_consumption (lines.material_id) +
	  MemoryConsumption::memory_consumption (lines.user_pointers));
}
  


void
TriangulationLevel<2>::reserve_space (const unsigned int new_quads)
{
  const unsigned int new_size = new_quads +
				std::count_if (quads.used.begin(),
					       quads.used.end(),
					       std::bind2nd (std::equal_to<bool>(), true));

				   // see above...
  if (new_size>quads.quads.size())
    {
      quads.quads.reserve (new_size);
      quads.quads.insert (quads.quads.end(), new_size-quads.quads.size(), Quad());
      
      quads.used.reserve (new_size);
      quads.used.insert (quads.used.end(), new_size-quads.used.size(), false);
  
      quads.user_flags.reserve (new_size);
      quads.user_flags.insert (quads.user_flags.end(),
			       new_size-quads.user_flags.size(), false);
  
      quads.children.reserve (new_size);
      quads.children.insert (quads.children.end(), new_size-quads.children.size(),
			     -1);

      quads.material_id.reserve (new_size);
      quads.material_id.insert (quads.material_id.end(),
				new_size-quads.material_id.size(),
				255);

      quads.user_pointers.reserve (new_size);
      quads.user_pointers.insert (quads.user_pointers.end(),
				  new_size-quads.user_pointers.size(), 0);
    };
}



void
TriangulationLevel<2>::monitor_memory (const unsigned int true_dimension) const
{
  TriangulationLevel<1>::monitor_memory (true_dimension);
}



unsigned int
TriangulationLevel<2>::memory_consumption () const
{
  return (TriangulationLevel<1>::memory_consumption() +
	  MemoryConsumption::memory_consumption (quads.quads) +
	  MemoryConsumption::memory_consumption (quads.children) +
	  MemoryConsumption::memory_consumption (quads.used) +
	  MemoryConsumption::memory_consumption (quads.user_flags) +
	  MemoryConsumption::memory_consumption (quads.material_id) +
	  MemoryConsumption::memory_consumption (quads.user_pointers));
}



void
TriangulationLevel<3>::reserve_space (const unsigned int new_hexes)
{
  const unsigned int new_size = new_hexes +
				std::count_if (hexes.used.begin(),
					       hexes.used.end(),
					       std::bind2nd (std::equal_to<bool>(), true));

				   // see above...
  if (new_size>hexes.hexes.size())
    {
      hexes.hexes.reserve (new_size);
      hexes.hexes.insert (hexes.hexes.end(), new_size-hexes.hexes.size(), Hexahedron());
      
      hexes.used.reserve (new_size);
      hexes.used.insert (hexes.used.end(), new_size-hexes.used.size(), false);
  
      hexes.user_flags.reserve (new_size);
      hexes.user_flags.insert (hexes.user_flags.end(),
			       new_size-hexes.user_flags.size(), false);
  
      hexes.children.reserve (new_size);
      hexes.children.insert (hexes.children.end(), new_size-hexes.children.size(),
			     -1);

      hexes.material_id.reserve (new_size);
      hexes.material_id.insert (hexes.material_id.end(),
				new_size-hexes.material_id.size(),
				255);

      hexes.user_pointers.reserve (new_size);
      hexes.user_pointers.insert (hexes.user_pointers.end(),
				  new_size-hexes.user_pointers.size(), 0);

      hexes.face_orientations.reserve (new_size * GeometryInfo<3>::faces_per_cell);
      hexes.face_orientations.insert (hexes.face_orientations.end(),
                                      new_size * GeometryInfo<3>::faces_per_cell
                                      - hexes.face_orientations.size(),
                                      true);
    };
}



void
TriangulationLevel<3>::monitor_memory (const unsigned int true_dimension) const
{
  TriangulationLevel<2>::monitor_memory (true_dimension);
}



unsigned int
TriangulationLevel<3>::memory_consumption () const
{
  return (TriangulationLevel<2>::memory_consumption() +
	  MemoryConsumption::memory_consumption (hexes.hexes) +
	  MemoryConsumption::memory_consumption (hexes.children) +
	  MemoryConsumption::memory_consumption (hexes.used) +
	  MemoryConsumption::memory_consumption (hexes.user_flags) +
	  MemoryConsumption::memory_consumption (hexes.material_id) +
	  MemoryConsumption::memory_consumption (hexes.user_pointers) +
	  MemoryConsumption::memory_consumption (hexes.face_orientations));
}



TriaNumberCache<1>::TriaNumberCache () :
		n_lines (0),
		n_active_lines (0) 
				   // all other fields are
				   // default constructed
{}



unsigned int
TriaNumberCache<1>::memory_consumption () const
{
  return (MemoryConsumption::memory_consumption (n_lines) +
	  MemoryConsumption::memory_consumption (n_lines_level) +
	  MemoryConsumption::memory_consumption (n_active_lines) +
	  MemoryConsumption::memory_consumption (n_active_lines_level));
}


TriaNumberCache<2>::TriaNumberCache () :
		n_quads (0),
		n_active_quads (0) 
				   // all other fields are
				   // default constructed
{}



unsigned int
TriaNumberCache<2>::memory_consumption () const
{
  return (TriaNumberCache<1>::memory_consumption () +
	  MemoryConsumption::memory_consumption (n_quads) +
	  MemoryConsumption::memory_consumption (n_quads_level) +
	  MemoryConsumption::memory_consumption (n_active_quads) +
	  MemoryConsumption::memory_consumption (n_active_quads_level));
}



TriaNumberCache<3>::TriaNumberCache () :
		n_hexes (0),
		n_active_hexes (0) 
				   // all other fields are
				   // default constructed
{}



unsigned int
TriaNumberCache<3>::memory_consumption () const
{
  return (TriaNumberCache<2>::memory_consumption () +
	  MemoryConsumption::memory_consumption (n_hexes) +
	  MemoryConsumption::memory_consumption (n_hexes_level) +
	  MemoryConsumption::memory_consumption (n_active_hexes) +
	  MemoryConsumption::memory_consumption (n_active_hexes_level));
}
