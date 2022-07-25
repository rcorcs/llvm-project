//----------------------------  log.cc  ---------------------------
//    $Id: log.cc,v 1.4 2004/09/19 21:57:30 wolf Exp $
//    Version: $Name:  $
//
//    Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003, 2004 by the deal.II authors
//
//    This file is subject to QPL and may not be  distributed
//    without copyright and license information. Please refer
//    to the file deal.II/doc/license.html for the  text  and
//    further information on this license.
//
//----------------------------  log.cc  ---------------------------


#include <base/logstream.h>
#include <base/job_identifier.h>
#include <base/memory_consumption.h>

#include <iostream>
#include <iomanip>
#include <fstream>

#ifdef HAVE_STD_STRINGSTREAM
#  include <sstream>
#else
#  include <strstream>
#endif


LogStream deallog;


LogStream::LogStream()
		:
		std_out(&std::cerr), file(0), was_endl(true),
		std_depth(10000), file_depth(10000),
		print_utime(false), diff_utime(false),
		last_time (0.), old_cerr(0)
{
  prefixes.push("DEAL:");
  std_out->setf(std::ios::showpoint | std::ios::left);
}


LogStream::~LogStream()
{
  if (old_cerr)
    std::cerr.rdbuf(old_cerr);
}


void
LogStream::attach(std::ostream& o)
{
  file = &o;
  o.setf(std::ios::showpoint | std::ios::left);
  o << dealjobid();
}


void LogStream::detach ()
{
  file = 0;
}


void LogStream::log_cerr ()
{
  if (old_cerr == 0)
    {
      old_cerr = std::cerr.rdbuf(file->rdbuf());
    } else {
      std::cerr.rdbuf(old_cerr);
      old_cerr = 0;
    }
}


std::ostream&
LogStream::get_console()
{
  return *std_out;
}


std::ostream&
LogStream::get_file_stream()
{
  Assert(file, ExcNoFileStreamGiven());
  return *file;
}


const std::string&
LogStream::get_prefix() const
{
  return prefixes.top();
}


void
LogStream::push (const std::string& text)
{
  std::string pre=prefixes.top();
  pre += text;
  pre += std::string(":");
  prefixes.push(pre);
}


void LogStream::pop ()
{
  if (prefixes.size() > 1)
    prefixes.pop();
}


unsigned int
LogStream::depth_console (const unsigned n)
{
  const unsigned int h = std_depth;
  std_depth = n;
  return h;
}


unsigned int
LogStream::depth_file (const unsigned n)
{
  const unsigned int h = file_depth;
  file_depth = n;
  return h;
}


bool
LogStream::log_execution_time (const bool flag)
{
  const bool h = print_utime;
  print_utime = flag;
  return h;
}


bool
LogStream::log_time_differences (const bool flag)
{
  const bool h = diff_utime;
  diff_utime = flag;
  return h;
}


void
LogStream::print_line_head()
{
}


unsigned int
LogStream::memory_consumption () const
{
  unsigned int mem = sizeof(*this);
				   // to determine size of stack
				   // elements, we have to copy the
				   // stack since we can't access
				   // elements from further below
  std::stack<std::string> tmp;
  while (tmp.size() > 0)
    {
      mem += MemoryConsumption::memory_consumption (tmp.top());
      tmp.pop ();
    };
  
  return mem;
}
