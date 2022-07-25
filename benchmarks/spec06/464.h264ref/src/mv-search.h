
/*!
 ************************************************************************
 * \file mv-search.h
 *
 * \brief
 *   array definition for motion search
 *
 * \author
 *    Inge Lille-Langoy               <inge.lille-langoy@telenor.com>   \n
 *    Copyright (C) 1999  Telenor Satellite Services, Norway
 *
 ************************************************************************
 */

#ifndef _MV_SEARCH_H_
#define _MV_SEARCH_H_

//! convert from H.263 QP to H.264 quant given by: quant=pow(2,QP/6)
const int QP2QUANT[40]=
{
   1, 1, 1, 1, 2, 2, 2, 2,
   3, 3, 3, 4, 4, 4, 5, 6,
   6, 7, 8, 9,10,11,13,14,
  16,18,20,23,25,29,32,36,
  40,45,51,57,64,72,81,91
};

#endif

