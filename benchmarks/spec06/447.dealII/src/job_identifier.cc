//----------------------------  job_identifier.cc  ---------------------------
//    $Id: job_identifier.cc,v 1.3 2004/09/19 22:21:42 wolf Exp $
//    Version: $Name:  $
//
//    Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003, 2004 by the deal authors
//
//    This file is subject to QPL and may not be  distributed
//    without copyright and license information. Please refer
//    to the file deal.II/doc/license.html for the  text  and
//    further information on this license.
//
//----------------------------  job_identifier.cc  ---------------------------


#include <base/job_identifier.h>

JobIdentifier dealjobid;


JobIdentifier::JobIdentifier()
{}


const std::string
JobIdentifier::operator ()() const
{
  return id;
}
