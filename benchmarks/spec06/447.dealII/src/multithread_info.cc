//----------------------------  multithread_info.cc  ----------------
//    $Id: multithread_info.cc,v 1.3 2006/01/23 23:49:36 wolf Exp $
//    Version: $Name:  $
//
//    Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003, 2004 by the deal authors
//
//    This file is subject to QPL and may not be  distributed
//    without copyright and license information. Please refer
//    to the file deal.II/doc/license.html for the  text  and
//    further information on this license.
//
//----------------------------  multithread_info.cc  ----------------


#include <base/multithread_info.h>

unsigned int MultithreadInfo::get_n_cpus()
{
  return 1;
}


MultithreadInfo::MultithreadInfo ()
                :
                n_cpus (get_n_cpus()),
                n_default_threads (n_cpus)
{}



unsigned int
MultithreadInfo::memory_consumption ()
{
				   // only simple data elements, so
				   // use sizeof operator
  return sizeof (MultithreadInfo);
}



// definition of the variable which is declared `extern' in the .h file
MultithreadInfo multithread_info;
