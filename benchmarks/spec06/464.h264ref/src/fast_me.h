
/*!
 ************************************************************************
 *
 * \file fast_me.h
 *
 * \brief
 *   Macro definitions and global variables for fast integer pel motion 
 *   estimation and fractional pel motion estimation
 *
 * \author
 *   Main contributors: (see contributors.h for copyright, address and affiliation details)
 *    - Zhibo Chen         <chenzhibo@tsinghua.org.cn>
 *    - JianFeng Xu        <fenax@video.mdc.tsinghua.edu.cn>  
 *
 * \date
 *   Apr. 2003
 ************************************************************************
 */

#ifndef _FAST_ME_H_
#define _FAST_ME_H_

#include "mbuffer.h"

#define EARLY_TERMINATION  if(ref>0)  \
  {                                                                    \
  if ((min_mcost-pred_SAD_ref)<pred_SAD_ref*betaThird)             \
  goto third_step;                                             \
  else if((min_mcost-pred_SAD_ref)<pred_SAD_ref*betaSec)           \
  goto sec_step;                                               \
  }                                                                    \
  else if(blocktype>1)                                                 \
  {                                                                    \
  if ((min_mcost-pred_SAD_uplayer)<pred_SAD_uplayer*betaThird)     \
    {                                                                \
    goto third_step;                                             \
    }                                                                \
    else if((min_mcost-pred_SAD_uplayer)<pred_SAD_uplayer*betaSec)   \
    goto sec_step;                                               \
  }                                                                    \
  else                                                                 \
  {                                                                    \
  if ((min_mcost-pred_SAD_space)<pred_SAD_space*betaThird)         \
    {                                                                \
    goto third_step;                                             \
    }                                                                \
    else if((min_mcost-pred_SAD_space)<pred_SAD_space*betaSec)       \
    goto sec_step;                                               \
  }


#define SEARCH_ONE_PIXEL  if(abs(cand_x - center_x) <=search_range && abs(cand_y - center_y)<= search_range) \
    { \
    if(!McostState[cand_y-center_y+search_range][cand_x-center_x+search_range]) \
    { \
    mcost = MV_COST (lambda_factor, mvshift, cand_x, cand_y, pred_x, pred_y); \
    mcost = PartCalMad(ref_pic, orig_pic, get_ref_line,blocksize_y,blocksize_x,blocksize_x4,mcost,min_mcost,cand_x,cand_y); \
    McostState[cand_y-center_y+search_range][cand_x-center_x+search_range] = mcost; \
    if (mcost < min_mcost) \
    { \
    best_x = cand_x; \
    best_y = cand_y; \
    min_mcost = mcost; \
    } \
    } \
    }
#define SEARCH_ONE_PIXEL1(value_iAbort) if(abs(cand_x - center_x) <=search_range && abs(cand_y - center_y)<= search_range) \
      { \
      if(!McostState[cand_y-center_y+search_range][cand_x-center_x+search_range]) \
        { \
        mcost = MV_COST (lambda_factor, mvshift, cand_x, cand_y, pred_x, pred_y); \
        mcost = PartCalMad(ref_pic, orig_pic, get_ref_line,blocksize_y,blocksize_x,blocksize_x4,mcost,min_mcost,cand_x,cand_y); \
        McostState[cand_y-center_y+search_range][cand_x-center_x+search_range] = mcost; \
        if (mcost < min_mcost) \
          { \
          best_x = cand_x; \
          best_y = cand_y; \
          min_mcost = mcost; \
          iAbort = value_iAbort; \
          } \
        } \
      }


#if defined(SPEC_CPU)
extern int **McostState; //state for integer pel search

extern int *****all_mincost;//store the MV and SAD information needed;
extern int *****all_bwmincost;//store for backward prediction
extern int pred_SAD_space,pred_SAD_time,pred_SAD_ref,pred_SAD_uplayer;//SAD prediction
extern int FME_blocktype;  //blocktype for FME SetMotionVectorPredictor
extern int pred_MV_time[2],pred_MV_ref[2],pred_MV_uplayer[2];//pred motion vector by space or temporal correlation,Median is provided

//for early termination
extern float Quantize_step;
extern float  Bsize[8];
extern int Thresh4x4;
extern float AlphaSec[8];
extern float AlphaThird[8];
extern int  *flag_intra;
extern int  flag_intra_SAD;
extern byte **SearchState; //state for fractional pel search

#else /* SPEC_CPU */

int **McostState; //state for integer pel search

int *****all_mincost;//store the MV and SAD information needed;
int *****all_bwmincost;//store for backward prediction
int pred_SAD_space,pred_SAD_time,pred_SAD_ref,pred_SAD_uplayer;//SAD prediction
int FME_blocktype;  //blocktype for FME SetMotionVectorPredictor
int pred_MV_time[2],pred_MV_ref[2],pred_MV_uplayer[2];//pred motion vector by space or temporal correlation,Median is provided

//for early termination
float Quantize_step;
float  Bsize[8];
int Thresh4x4;
float AlphaSec[8];
float AlphaThird[8];
int  *flag_intra;
int  flag_intra_SAD;

byte **SearchState; //state for fractional pel search
#endif /* SPEC_CPU */

void DefineThreshold();
void DefineThresholdMB();
int get_mem_mincost (int****** mv);
int get_mem_bwmincost (int****** mv);
int get_mem_FME();
void free_mem_mincost (int***** mv);
void free_mem_bwmincost (int***** mv);
void free_mem_FME();
void   decide_intrabk_SAD();
void skip_intrabk_SAD(int best_mode, int ref_max);

int                                     //  ==> minimum motion cost after search
FastIntegerPelBlockMotionSearch  (pel_t**   orig_pic,     // <--  not used
                                  short     ref,          // <--  reference frame (0... or -1 (backward))
                                  int       list,
                                  int       pic_pix_x,    // <--  absolute x-coordinate of regarded AxB block
                                  int       pic_pix_y,    // <--  absolute y-coordinate of regarded AxB block
                                  int       blocktype,    // <--  block type (1-16x16 ... 7-4x4)
                                  short     pred_mv_x,    // <--  motion vector predictor (x) in sub-pel units
                                  short     pred_mv_y,    // <--  motion vector predictor (y) in sub-pel units
                                  short*    mv_x,         //  --> motion vector (x) - in pel units
                                  short*    mv_y,         //  --> motion vector (y) - in pel units
                                  int       search_range, // <--  1-d search range in pel units                         
                                  int       min_mcost,    // <--  minimum motion cost (cost for center or huge value)
                                  double    lambda) ;     // <--  lagrangian parameter for determining motion cost

int AddUpSADQuarter(int pic_pix_x,int pic_pix_y,int blocksize_x,int blocksize_y,
                    int cand_mv_x,int cand_mv_y, StorablePicture *ref_picture, pel_t**   orig_pic, 
                    int Mvmcost, int min_mcost,int useABT);

int                                                   //  ==> minimum motion cost after search
FastSubPelBlockMotionSearch (pel_t**   orig_pic,      // <--  original pixel values for the AxB block
                             short       ref,           // <--  reference frame (0... or -1 (backward))
                             int       list,
                             int       pic_pix_x,     // <--  absolute x-coordinate of regarded AxB block
                             int       pic_pix_y,     // <--  absolute y-coordinate of regarded AxB block
                             int       blocktype,     // <--  block type (1-16x16 ... 7-4x4)
                             short     pred_mv_x,     // <--  motion vector predictor (x) in sub-pel units
                             short     pred_mv_y,     // <--  motion vector predictor (y) in sub-pel units
                             short*    mv_x,          // <--> in: search center (x) / out: motion vector (x) - in pel units
                             short*    mv_y,          // <--> in: search center (y) / out: motion vector (y) - in pel units
                             int       search_pos2,   // <--  search positions for    half-pel search  (default: 9)
                             int       search_pos4,   // <--  search positions for quarter-pel search  (default: 9)
                             int       min_mcost,     // <--  minimum motion cost (cost for center or huge value)
                             double    lambda,        // <--  lagrangian parameter for determining motion cost
                             int  useABT);

int                                               //  ==> minimum motion cost after search
SubPelBlockMotionSearch (pel_t**   orig_pic,      // <--  original pixel values for the AxB block
                         short     ref,           // <--  reference frame (0... or -1 (backward))
                         int       list,
                         int       pic_pix_x,     // <--  absolute x-coordinate of regarded AxB block
                         int       pic_pix_y,     // <--  absolute y-coordinate of regarded AxB block
                         int       blocktype,     // <--  block type (1-16x16 ... 7-4x4)
                         short     pred_mv_x,     // <--  motion vector predictor (x) in sub-pel units
                         short     pred_mv_y,     // <--  motion vector predictor (y) in sub-pel units
                         short*    mv_x,          // <--> in: search center (x) / out: motion vector (x) - in pel units
                         short*    mv_y,          // <--> in: search center (y) / out: motion vector (y) - in pel units
                         int       search_pos2,   // <--  search positions for    half-pel search  (default: 9)
                         int       search_pos4,   // <--  search positions for quarter-pel search  (default: 9)
                         int       min_mcost,     // <--  minimum motion cost (cost for center or huge value)
                         double    lambda         // <--  lagrangian parameter for determining motion cost
                         );

int                                         //  ==> minimum motion cost after search
FME_BlockMotionSearch (short     ref,           // <--  reference frame (0... or -1 (backward))
                       int       list,
                       int       pic_pix_x,     // <--  absolute x-coordinate of regarded AxB block
                       int       pic_pix_y,     // <--  absolute y-coordinate of regarded AxB block
                       int       blocktype,     // <--  block type (1-16x16 ... 7-4x4)
                       int       search_range,  // <--  1-d search range for integer-position search
                       double    lambda         // <--  lagrangian parameter for determining motion cost
                       );

int                                              //!< minimum motion cost after search
noFME_BlockMotionSearch (short     ref,          //!< reference idx
                         int       list,         //!< reference pciture list
                         int       mb_x,         //!< x-coordinate inside macroblock
                         int       mb_y,         //!< y-coordinate inside macroblock
                         int       blocktype,    //!< block type (1-16x16 ... 7-4x4)
                         int       search_range, //!< 1-d search range for integer-position search
                         double    lambda        //!< lagrangian parameter for determining motion cost
                         );
#endif
