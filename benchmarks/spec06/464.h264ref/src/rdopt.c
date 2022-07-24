
/*!
 ***************************************************************************
 * \file rdopt.c
 *
 * \brief
 *    Rate-Distortion optimized mode decision
 *
 * \author
 *    - Heiko Schwarz              <hschwarz@hhi.de>
 *    - Valeri George              <george@hhi.de>
 *    - Lowell Winger              <lwinger@lsil.com>
 *    - Alexis Michael Tourapis    <alexismt@ieee.org>
 * \date
 *    12. April 2001
 **************************************************************************
 */

#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <limits.h>

#include "global.h"

#include "rdopt_coding_state.h"
#include "memalloc.h"
#include "mb_access.h"
#include "elements.h"
#include "intrarefresh.h"
#include "image.h"
#include "transform8x8.h"
#include "cabac.h"            // head file for rate control
#include "vlc.h"
#include "fast_me.h"
#include "ratectl.h"            // head file for rate control

#define KS_MV


//Rate control

int DELTA_QP,DELTA_QP2;
#if !defined(SPEC_CPU)
int QP,QP2;
int diffy[16][16];
#endif
static int pred[16][16];

extern       int  QP2QUANT  [40];

//==== MODULE PARAMETERS ====
int   best_mode;
imgpel   rec_mbY[16][16], rec_mbU[16][16], rec_mbV[16][16], rec_mbY8x8[16][16];    // reconstruction values
imgpel   mpr8x8[16][16];
int   ****cofAC=NULL, ****cofAC8x8=NULL;        // [8x8block][4x4block][level/run][scan_pos]
int   ***cofDC=NULL;                       // [yuv][level/run][scan_pos]
int   **cofAC4x4=NULL, ****cofAC4x4intern=NULL; // [level/run][scan_pos]
int   cbp, cbp8x8, cnt_nonz_8x8;
int64 cbp_blk;
int   cbp_blk8x8;
short   frefframe[4][4], brefframe[4][4], b8mode[4], b8pdir[4];
short   best8x8mode [4];                // [block]
short   best8x8pdir  [MAXMODE][4];       // [mode][block]
short   best8x8fwref [MAXMODE][4];       // [mode][block]
short   best8x8bwref [MAXMODE][4];       // [mode][block]
#if BI_PREDICTION
int    bi_pred_me;
#endif

CSptr cs_mb=NULL, cs_b8=NULL, cs_cm=NULL, cs_imb=NULL, cs_ib8=NULL, cs_ib4=NULL, cs_pc=NULL;
int   best_c_imode;
int   best_i16offset;

imgpel temp_imgY[16][16]; // to temp store the Y data for 8x8 transform
imgpel temp_imgU[16][16];
imgpel temp_imgV[16][16];
int   best_transform_size[4];

//mixed transform sizes definitions
int   luma_transform_size_8x8_flag;
int   NoMbPartLessThan8x8Flag;

short part8x8pdir[4];
short part8x8fwref[4];
short part8x8bwref[4];

short mv8x8[2][4][4][2];              //[LIST][block_x][block_y][MVx/MVy]
short ref8x8[2][4][4];                //[LIST][block_x][block_y]

short all_mv8x8[2][2][4][4][2];       //[8x8_data/temp_data][LIST][block_x][block_y][MVx/MVy]
short pred_mv8x8[2][2][4][4][2];

//ADD-VG-28062004
int   ****cofAC_8x8ts = NULL;        // [8x8block][4x4block][level/run][scan_pos]
imgpel   rec_mbY_8x8ts[16][16];
imgpel   mpr_8x8ts[16][16];
int64    cbp_blk8_8x8ts;
int      cbp8_8x8ts;
int      cost8_8x8ts;
int      cnt_nonz8_8x8ts;
int      part8x8mode[4];
//ADD-VG-28062004-END


void StoreMV8x8(int dir);
void RestoreMV8x8(int dir);
// end of mixed transform sizes definitions


// Residue Color Transform
int   cofAC4x4_chroma[2][2][18];
int   rec_resG_8x8[16][16], resTrans_R_8x8[16][16], resTrans_B_8x8[16][16];
int   rec_resG_8x8ts[16][16], resTrans_R_8x8ts[16][16], resTrans_B_8x8ts[16][16];
int   mprRGB_8x8[3][16][16], mprRGB_8x8ts[3][16][16];
int   b4_ipredmode[16], b4_intra_pred_modes[16];

/*!
 ************************************************************************
 * \brief
 *    delete structure for RD-optimized mode decision
 ************************************************************************
 */
void clear_rdopt ()
{
  free_mem_DCcoeff (cofDC);
  free_mem_ACcoeff (cofAC);
  free_mem_ACcoeff (cofAC8x8);
  free_mem_ACcoeff (cofAC4x4intern);

  if (input->AllowTransform8x8)
  {
    free_mem_ACcoeff (cofAC_8x8ts);
  }
  
  // structure for saving the coding state
  delete_coding_state (cs_mb);
  delete_coding_state (cs_b8);
  delete_coding_state (cs_cm);
  delete_coding_state (cs_imb);
  delete_coding_state (cs_ib8);
  delete_coding_state (cs_ib4);
  delete_coding_state (cs_pc);
}


/*!
 ************************************************************************
 * \brief
 *    create structure for RD-optimized mode decision
 ************************************************************************
 */
void init_rdopt ()
{
  rdopt = NULL;

  get_mem_DCcoeff (&cofDC);
  get_mem_ACcoeff (&cofAC);
  get_mem_ACcoeff (&cofAC8x8);
  get_mem_ACcoeff (&cofAC4x4intern);
  cofAC4x4 = cofAC4x4intern[0][0];

  if (input->AllowTransform8x8)
  {
    get_mem_ACcoeff (&cofAC_8x8ts);
  }
  
  // structure for saving the coding state
  cs_mb  = create_coding_state ();
  cs_b8  = create_coding_state ();
  cs_cm  = create_coding_state ();
  cs_imb = create_coding_state ();
  cs_ib8 = create_coding_state ();
  cs_ib4 = create_coding_state ();
  cs_pc  = create_coding_state ();
}



/*!
 *************************************************************************************
 * \brief
 *    Updates the pixel map that shows, which reference frames are reliable for
 *    each MB-area of the picture.
 *
 * \note
 *    The new values of the pixel_map are taken from the temporary buffer refresh_map
 *
 *************************************************************************************
 */
void UpdatePixelMap()
{
  int mx,my,y,x,i,j;
  if (img->type==I_SLICE)
  {
    for (y=0; y<img->height; y++)
    for (x=0; x<img->width; x++)
    {
      pixel_map[y][x]=1;
    }
  }
  else
  {
    for (my=0; my<img->height/8; my++)
    for (mx=0; mx<img->width/8;  mx++)
    {
      j = my*8 + 8;
      i = mx*8 + 8;
      if (refresh_map[my][mx])
      {
        for (y=my*8; y<j; y++)
        for (x=mx*8; x<i; x++)  pixel_map[y][x] = 1;
      }
      else
      {
        for (y=my*8; y<j; y++)
        for (x=mx*8; x<i; x++)  pixel_map[y][x] = min(pixel_map[y][x]+1, input->num_ref_frames+1);
      }
    }
  }
}

/*!
 *************************************************************************************
 * \brief
 *    Checks if a given reference frame is reliable for the current
 *    macroblock, given the motion vectors that the motion search has
 *    returned.
 *
 * \return
 *    If the return value is 1, the reference frame is reliable. If it
 *    is 0, then it is not reliable.
 *
 * \note
 *    A specific area in each reference frame is assumed to be unreliable
 *    if the same area has been intra-refreshed in a subsequent frame.
 *    The information about intra-refreshed areas is kept in the pixel_map.
 *
 *************************************************************************************
 */
int CheckReliabilityOfRef (int block, int list_idx, int ref, int mode)
{
  int y,x, block_y, block_x, dy, dx, y_pos, x_pos, yy, xx, pres_x, pres_y;
  int maxold_x  = img->width-1;
  int maxold_y  = img->height-1;
  int ref_frame = ref+1;

  int by0 = (mode>=4?2*(block/2):mode==2?2*block:0);
  int by1 = by0 + (mode>=4||mode==2?2:4);
  int bx0 = (mode>=4?2*(block%2):mode==3?2*block:0);
  int bx1 = bx0 + (mode>=4||mode==3?2:4);

  for (block_y=by0; block_y<by1; block_y++)
    for (block_x=bx0; block_x<bx1; block_x++)
    {
      y_pos  = img->all_mv[block_x][block_y][list_idx][ref][mode][1];
      y_pos += (img->block_y+block_y) * BLOCK_SIZE * 4;
      x_pos  = img->all_mv[block_x][block_y][list_idx][ref][mode][0];
      x_pos += (img->block_x+block_x) * BLOCK_SIZE * 4;

      /* Here we specify which pixels of the reference frame influence
         the reference values and check their reliability. This is
         based on the function Get_Reference_Pixel */

      dy = y_pos & 3;
      dx = x_pos & 3;

      y_pos = (y_pos-dy)/4;
      x_pos = (x_pos-dx)/4;

      if (dy==0 && dx==0) //full-pel
      {
        for (y=0 ; y < BLOCK_SIZE ; y++)
          for (x=0 ; x < BLOCK_SIZE ; x++)
            if (pixel_map[max(0,min(maxold_y,y_pos+y))][max(0,min(maxold_x,x_pos+x))] < ref_frame)
              return 0;
      }
      else  /* other positions */
      {
        if (dy == 0)
        {
          for (y=0 ; y < BLOCK_SIZE ; y++)
            for (x=0 ; x < BLOCK_SIZE ; x++)
            {
              pres_y = max(0,min(maxold_y,y_pos+y));
              for(xx=-2;xx<4;xx++) {
                pres_x = max(0,min(maxold_x,x_pos+x+xx));
                if (pixel_map[pres_y][pres_x] < ref_frame)
                  return 0;
              }
            }
        }

        else if (dx == 0)
        {
          for (y=0 ; y < BLOCK_SIZE ; y++)
            for (x=0 ; x < BLOCK_SIZE ; x++)
            {
              pres_x = max(0,min(maxold_x,x_pos+x));
              for(yy=-2;yy<4;yy++) {
                pres_y = max(0,min(maxold_y,y_pos+yy+y));
                if (pixel_map[pres_y][pres_x] < ref_frame)
                  return 0;
              }
            }
        }
        else if (dx == 2)
        {
          for (y=0 ; y < BLOCK_SIZE ; y++)
            for (x=0 ; x < BLOCK_SIZE ; x++)
            {
              for(yy=-2;yy<4;yy++) {
                pres_y = max(0,min(maxold_y,y_pos+yy+y));
                for(xx=-2;xx<4;xx++) {
                  pres_x = max(0,min(maxold_x,x_pos+xx+x));
                  if (pixel_map[pres_y][pres_x] < ref_frame)
                    return 0;
                }
              }
            }
        }
        else if (dy == 2)
        {
          for (y=0 ; y < BLOCK_SIZE ; y++)
            for (x=0 ; x < BLOCK_SIZE ; x++)
            {
              for(xx=-2;xx<4;xx++) {
                pres_x = max(0,min(maxold_x,x_pos+xx+x));
                for(yy=-2;yy<4;yy++) {
                  pres_y = max(0,min(maxold_y,y_pos+yy+y));
                  if (pixel_map[pres_y][pres_x] < ref_frame)
                    return 0;
                }
              }
            }
        }
        else
        {
          for (y=0 ; y < BLOCK_SIZE ; y++)
            for (x=0 ; x < BLOCK_SIZE ; x++)
            {
              pres_y = dy == 1 ? y_pos+y : y_pos+y+1;
              pres_y = max(0,min(maxold_y,pres_y));

              for(xx=-2;xx<4;xx++) {
                pres_x = max(0,min(maxold_x,x_pos+xx+x));
                if (pixel_map[pres_y][pres_x] < ref_frame)
                  return 0;
              }

              pres_x = dx == 1 ? x_pos+x : x_pos+x+1;
              pres_x = max(0,min(maxold_x,pres_x));

              for(yy=-2;yy<4;yy++) {
                pres_y = max(0,min(maxold_y,y_pos+yy+y));
                if (pixel_map[pres_y][pres_x] < ref_frame)
                  return 0;
              }
            }
        }

      }
    }

  return 1;
}



/*!
 *************************************************************************************
 * \brief
 *    R-D Cost for an 4x4 Intra block
 *************************************************************************************
 */
double RDCost_for_4x4IntraBlocks (int*    nonzero,
                           int     b8,
                           int     b4,
                           int    ipmode,
                           double  lambda,
                           double  min_rdcost,
                           int mostProbableMode)
{
  double  rdcost;
  int     dummy, x, y, rate;
  int     distortion  = 0;
  int     block_x     = 8*(b8%2)+4*(b4%2);
  int     block_y     = 8*(b8/2)+4*(b4/2);
  int     pic_pix_x   = img->pix_x+block_x;
  int     pic_pix_y   = img->pix_y+block_y;
  int     pic_opix_y  = img->opix_y+block_y;
  imgpel  **imgY      = enc_picture->imgY;

  Slice          *currSlice    =  img->currentSlice;
  Macroblock     *currMB       = &img->mb_data[img->current_mb_nr];
  SyntaxElement  *currSE       = &img->MB_SyntaxElements[currMB->currSEnr];
  const int      *partMap      = assignSE2partition[input->partition_mode];
  DataPartition  *dataPart;

  //===== perform DCT, Q, IQ, IDCT, Reconstruction =====
  dummy = 0;

  *nonzero = dct_luma (block_x, block_y, &dummy, 1);

  //===== get distortion (SSD) of 4x4 block =====
  if(!img->residue_transform_flag)
  {
    for (y=0; y<4; y++)
    {
      for (x=pic_pix_x; x<pic_pix_x+4; x++)
      {
        distortion += img->quad [imgY_org[pic_opix_y+y][x] - imgY[pic_pix_y+y][x]];
      }
    }
  }

  store_coding_state (cs_cm);
  //===== RATE for INTRA PREDICTION MODE  (SYMBOL MODE MUST BE SET TO UVLC) =====
  currSE->value1 = (mostProbableMode == ipmode) ? -1 : ipmode < mostProbableMode ? ipmode : ipmode-1;

  //--- set position and type ---
  currSE->context = 4*b8 + b4;
  currSE->type    = SE_INTRAPREDMODE;

  //--- set function pointer ----
  if (input->symbol_mode != UVLC)    
  currSE->writing = writeIntraPredMode_CABAC;

  //--- choose data partition ---
  dataPart = &(currSlice->partArr[partMap[SE_INTRAPREDMODE]]);
  //--- encode and update rate ---
  if (input->symbol_mode == UVLC)    writeSyntaxElement_Intra4x4PredictionMode(currSE, dataPart);
  else                               dataPart->writeSyntaxElement (currSE, dataPart);

  rate = currSE->len;
  currSE++;
  currMB->currSEnr++;

  //===== RATE for LUMINANCE COEFFICIENTS =====
  if (input->symbol_mode == UVLC)
  {
    rate  += writeCoeff4x4_CAVLC (LUMA, b8, b4, 0);
  }
  else
  {
    rate  += writeLumaCoeff4x4_CABAC (b8, b4, 1);
  }
  reset_coding_state (cs_cm);
  rdcost = (double)distortion + lambda*(double)rate;

  if(img->residue_transform_flag)
    return (double)rate;
  else
    return rdcost;
}


// Residue Color Transform
int RDCost_for_4x4Blocks_Chroma (int     b8,
                                 int     b4,
                                 int  chroma)
{
  int     rate=0;

  Slice          *currSlice    =  img->currentSlice;
  Macroblock     *currMB       = &img->mb_data[img->current_mb_nr];
  SyntaxElement  *currSE       = &img->MB_SyntaxElements[currMB->currSEnr];
  const int      *partMap      = assignSE2partition[input->partition_mode];
  int uv;

  //===== perform DCT, Q, IQ, IDCT, Reconstruction =====
  if(b8 > 7) 
    uv = 1;
  else 
    uv = 0;

  cbp_chroma_block_temp[uv][2*((b8-4*(uv+1))%2)+(b4%2)][2*((b8-4*(uv+1))/2)+(b4/2)] = dct_chroma4x4 (chroma, b8, b4);

  store_coding_state (cs_cm);
  //===== RATE for LUMINANCE COEFFICIENTS =====
  if (input->symbol_mode == UVLC)
  {
    rate  = writeCoeff4x4_CAVLC (CHROMA_AC, b8, b4, ((2*(b8%2)+b4%2)<<4) | (2*(b8/2)+(b4/2)));
  }
  else
  {
    int * ACLevel, * ACRun;
    int level, run, k;
    DataPartition*  dataPart;
    int*            bitCount  = currMB->bitcounter;
    ACLevel = img->cofAC[b8][b4][0];
    ACRun   = img->cofAC[b8][b4][1];
    
    level=1;
    
    img->subblock_y = b4/2;
    img->subblock_x = b4%2;

    for (k=0; k < 17 && level != 0; k++)
    {
      level = currSE->value1 = ACLevel[k]; // level
      run   = currSE->value2 = ACRun  [k]; // run

      if (input->symbol_mode == UVLC)   currSE->mapping = levrun_linfo_inter;
      else                              currSE->writing = writeRunLevel_CABAC;

      currSE->context     = CHROMA_AC;
      currSE->type        = SE_CHR_AC_INTRA;

      img->is_intra_block =  IS_INTRA(currMB);
      img->is_v_block     = uv;

      // choose the appropriate data partition
      dataPart = &(currSlice->partArr[partMap[currSE->type]]); 
      dataPart->writeSyntaxElement (currSE, dataPart);
      bitCount[BITS_COEFF_UV_MB] += currSE->len;
      rate                       += currSE->len;

      // proceed to next SE
      currSE++;
      currMB->currSEnr++;
    }
  }
  reset_coding_state (cs_cm);

  return rate;
}


/*!
 *************************************************************************************
 * \brief
 *    Mode Decision for an 4x4 Intra block
 *************************************************************************************
 */
int Mode_Decision_for_4x4IntraBlocks (int  b8,  int  b4,  double  lambda,  int*  min_cost)
{
  int     ipmode, best_ipmode = 0, i, j, k, x, y, cost, dummy;
  int     c_nz, nonzero = 0, rec4x4[4][4], diff[16];
  double  rdcost;
  int     block_x     = 8*(b8%2)+4*(b4%2);
  int     block_y     = 8*(b8/2)+4*(b4/2);
  int     pic_pix_x   = img->pix_x+block_x;
  int     pic_pix_y   = img->pix_y+block_y;
  int     pic_opix_x   = img->opix_x+block_x;
  int     pic_opix_y   = img->opix_y+block_y;
  int     pic_block_x = pic_pix_x/4;
  int     pic_block_y = pic_pix_y/4;
  double  min_rdcost  = 1e30;

#if defined(SPEC_CPU)
  /* SPEC_CPU: epsilon is required for systems where double can be
     more than 64-bits (e.g. x87) */
  /* Allow 1 ulp of error @ ~2000.0f */
  /* hex value of epsilon = 0x3d50000000000000ull */
  double epsilon = 2.27373675443232059478759765626E-13f;
#endif

  int left_available, up_available, all_available;

  int     upMode;
  int     leftMode;
  int     mostProbableMode;

  PixelPos left_block;
  PixelPos top_block;

#ifdef BEST_NZ_COEFF
  int best_nz_coeff = 0;
#endif

  // Residue Color Transform
  int residue_R, residue_G, residue_B;
  int rate, distortion, temp;
  int c_ipmode = img->mb_data[img->current_mb_nr].c_ipred_mode;
  int rec4x4_c[2][4][4];
  
  getLuma4x4Neighbour(img->current_mb_nr, block_x/4, block_y/4, -1,  0, &left_block);
  getLuma4x4Neighbour(img->current_mb_nr, block_x/4, block_y/4,  0, -1, &top_block);

  // constrained intra pred
  if (input->UseConstrainedIntraPred)
  {
    left_block.available = left_block.available ? img->intra_block[left_block.mb_addr] : 0;
    top_block.available  = top_block.available  ? img->intra_block[top_block.mb_addr]  : 0;
  }

  upMode            = top_block.available ? img->ipredmode[top_block.pos_x ][top_block.pos_y ] : -1;
  leftMode          = left_block.available ? img->ipredmode[left_block.pos_x][left_block.pos_y] : -1;

  mostProbableMode  = (upMode < 0 || leftMode < 0) ? DC_PRED : upMode < leftMode ? upMode : leftMode;

  *min_cost = INT_MAX;

  //===== INTRA PREDICTION FOR 4x4 BLOCK =====
  intrapred_luma (pic_pix_x, pic_pix_y, &left_available, &up_available, &all_available);

  //===== LOOP OVER ALL 4x4 INTRA PREDICTION MODES =====
  for (ipmode=0; ipmode<NO_INTRA_PMODE; ipmode++)
  {
    int available_mode =  (ipmode==DC_PRED) ||
        ((ipmode==VERT_PRED||ipmode==VERT_LEFT_PRED||ipmode==DIAG_DOWN_LEFT_PRED) && up_available ) ||
        ((ipmode==HOR_PRED||ipmode==HOR_UP_PRED) && left_available ) ||(all_available);

    if (input->IntraDisableInterOnly==0 || img->type != I_SLICE)
    {
      if (input->Intra4x4ParDisable && (ipmode==VERT_PRED||ipmode==HOR_PRED))
        continue;
      
      if (input->Intra4x4DiagDisable && (ipmode==DIAG_DOWN_LEFT_PRED||ipmode==DIAG_DOWN_RIGHT_PRED))
        continue;
      
      if (input->Intra4x4DirDisable && ipmode>=VERT_RIGHT_PRED)
        continue;
    }
 
    if( available_mode)
    {
      if (!input->rdopt)
      {
        for (k=j=0; j<4; j++)
          for (i=0; i<4; i++, k++)
          {
            diff[k] = imgY_org[pic_opix_y+j][pic_opix_x+i] - img->mprr[ipmode][j][i];
          }
        cost  = (ipmode == mostProbableMode) ? 0 : (int)floor(4 * lambda );
        cost += SATD (diff, input->hadamard);
        if (cost < *min_cost)
        {
          best_ipmode = ipmode;
          *min_cost   = cost;
        }
      }
      else
      {
        // Residue Color Transform
        if(!img->residue_transform_flag)
        {
          // get prediction and prediction error
          for (j=0; j<4; j++)
          for (i=0; i<4; i++)
          {
            img->mpr[block_x+i][block_y+j]  = img->mprr[ipmode][j][i];
            img->m7[i][j]                   = imgY_org[pic_opix_y+j][pic_opix_x+i] - img->mprr[ipmode][j][i];
          }

          //===== store the coding state =====
          store_coding_state (cs_cm);
          // get and check rate-distortion cost
#if defined(SPEC_CPU)
          rdcost = RDCost_for_4x4IntraBlocks (&c_nz, b8, b4, ipmode, lambda, min_rdcost, mostProbableMode);
          if ((fabs(rdcost - min_rdcost) > epsilon) && (rdcost < min_rdcost))
#else
          if ((rdcost = RDCost_for_4x4IntraBlocks (&c_nz, b8, b4, ipmode, lambda, min_rdcost, mostProbableMode)) < min_rdcost)
#endif
          {
            //--- set coefficients ---
            for (j=0; j<2; j++)
            for (i=0; i<18;i++)  cofAC4x4[j][i]=img->cofAC[b8][b4][j][i];

            //--- set reconstruction ---
            for (y=0; y<4; y++)
            for (x=0; x<4; x++)  rec4x4[y][x] = enc_picture->imgY[pic_pix_y+y][pic_pix_x+x];

            //--- flag if dct-coefficients must be coded ---
            nonzero = c_nz;

            //--- set best mode update minimum cost ---
            min_rdcost    = rdcost;
            best_ipmode   = ipmode;
#ifdef BEST_NZ_COEFF
            best_nz_coeff = img->nz_coeff [img->current_mb_nr][block_x/4][block_y/4];
#endif

          }
          reset_coding_state (cs_cm);
        } 
        else 
        {
          for (j=0; j<4; j++)
            for (i=0; i<4; i++)
            {
              residue_B = imgUV_org[0][pic_opix_y+j][pic_opix_x+i] - img->mprr_c[0][c_ipmode][block_x+i][block_y+j];
              residue_G = imgY_org[pic_opix_y+j][pic_opix_x+i] - img->mprr[ipmode][j][i];
              residue_R = imgUV_org[1][pic_opix_y+j][pic_opix_x+i] - img->mprr_c[1][c_ipmode][block_x+i][block_y+j];
              
              /* Foward Residue Transform */
              resTrans_R[i][j] = residue_R-residue_B;
              temp = residue_B+(resTrans_R[i][j]>>1);
              resTrans_B[i][j] = residue_G-temp;
              resTrans_G[i][j] = temp+(resTrans_B[i][j]>>1);
            }
            
          for (j=0; j<4; j++)
            for (i=0; i<4; i++)
            {
              img->m7[i][j]  = resTrans_G[i][j];
            }

          store_coding_state (cs_cm);
          rate = (int) RDCost_for_4x4IntraBlocks (&c_nz, b8, b4, ipmode, lambda, min_rdcost, mostProbableMode);
          reset_coding_state (cs_cm);

          for (j=0; j<4; j++)
            for (i=0; i<4; i++)
            {
              rec_resG[i][j] = img->m7[i][j];
              img->m7[i][j]  = resTrans_R[i][j];
            }
          store_coding_state (cs_cm);
          rate += RDCost_for_4x4Blocks_Chroma (b8+4, b4, 0);
          for (j=0; j<4; j++)
            for (i=0; i<4; i++)
            {
              rec_resR[i][j] = img->m7[i][j];
              img->m7[i][j]  = resTrans_B[i][j];
            }
          rate += RDCost_for_4x4Blocks_Chroma (b8+8, b4, 1);
          reset_coding_state (cs_cm);
          for (j=0; j<4; j++)
            for (i=0; i<4; i++)
            {
              rec_resB[i][j] = img->m7[i][j];
            }

          for (j=0; j<4; j++)
            for (i=0; i<4; i++)
            {
              /* Inverse Residue Transform */
              temp      = rec_resG[i][j]-(rec_resB[i][j]>>1);
              residue_G = rec_resB[i][j]+temp;
              residue_B = temp - (rec_resR[i][j]>>1);
              residue_R = residue_B+rec_resR[i][j];
              enc_picture->imgUV[0][pic_pix_y+j][pic_pix_x+i] = min(img->max_imgpel_value_uv,max(0,residue_B+(int)img->mprr_c[0][c_ipmode][block_x+i][block_y+j]));
              enc_picture->imgY[pic_pix_y+j][pic_pix_x+i]     = min(img->max_imgpel_value,max(0,residue_G+(int)img->mprr[ipmode][j][i]));
              enc_picture->imgUV[1][pic_pix_y+j][pic_pix_x+i] = min(img->max_imgpel_value_uv,max(0,residue_R+(int)img->mprr_c[1][c_ipmode][block_x+i][block_y+j]));
            }

          //===== get distortion (SSD) of 4x4 block =====
          distortion = 0;
          for (y=0; y<4; y++)
            for (x=pic_pix_x; x<pic_pix_x+4; x++)
            {
              distortion += (imgY_org[pic_pix_y+y][x] - enc_picture->imgY[pic_pix_y+y][x])*(imgY_org[pic_pix_y+y][x] - enc_picture->imgY[pic_pix_y+y][x]);
              distortion += (imgUV_org[0][pic_pix_y+y][x] - enc_picture->imgUV[0][pic_pix_y+y][x])*(imgUV_org[0][pic_pix_y+y][x] - enc_picture->imgUV[0][pic_pix_y+y][x]);
              distortion += (imgUV_org[1][pic_pix_y+y][x] - enc_picture->imgUV[1][pic_pix_y+y][x])*(imgUV_org[1][pic_pix_y+y][x] - enc_picture->imgUV[1][pic_pix_y+y][x]);
            }
          rdcost = (double)distortion + lambda*(double)rate;

          if (rdcost < min_rdcost)
          {
            //--- set coefficients ---
            for (j=0; j<2; j++)
              for (i=0; i<18;i++)  cofAC4x4[j][i]=img->cofAC[b8][b4][j][i];

            for (j=0; j<2; j++)
              for (i=0; i<18;i++)  cofAC4x4_chroma[0][j][i]=img->cofAC[b8+4][b4][j][i];
            for (j=0; j<2; j++)
              for (i=0; i<18;i++)  cofAC4x4_chroma[1][j][i]=img->cofAC[b8+8][b4][j][i];

            for (i=0; i<2; i++)
            { //uv
              dc_level[i][2*(b8%2)+(b4%2)][2*(b8/2)+(b4/2)] = dc_level_temp[i][2*(b8%2)+(b4%2)][2*(b8/2)+(b4/2)];
              cbp_chroma_block[i][2*(b8%2)+(b4%2)][2*(b8/2)+(b4/2)] = cbp_chroma_block_temp[i][2*(b8%2)+(b4%2)][2*(b8/2)+(b4/2)];
              //--- set reconstruction ---
              for (y=0; y<4; y++)
                for (x=0; x<4; x++)  rec4x4_c[i][y][x] = enc_picture->imgUV[i][pic_pix_y+y][pic_pix_x+x];
            }

            //--- set reconstruction ---
            for (y=0; y<4; y++)
              for (x=0; x<4; x++)  rec4x4[y][x] = enc_picture->imgY[pic_pix_y+y][pic_pix_x+x];

            //--- flag if dct-coefficients must be coded ---
            nonzero = c_nz;

            //--- set best mode update minimum cost ---
            min_rdcost  = rdcost;
            best_ipmode = ipmode;
#ifdef BEST_NZ_COEFF
            best_nz_coeff = img->nz_coeff [img->current_mb_nr][block_x/4][block_y/4];
#endif
          }
        }
      }
    }
  }

#ifdef BEST_NZ_COEFF
  img->nz_coeff [img->current_mb_nr][block_x/4][block_y/4] = best_nz_coeff;
#endif
  //===== set intra mode prediction =====
  img->ipredmode[pic_block_x][pic_block_y] = best_ipmode;
  img->mb_data[img->current_mb_nr].intra_pred_modes[4*b8+b4] = mostProbableMode == best_ipmode ? -1 : best_ipmode < mostProbableMode ? best_ipmode : best_ipmode-1;

  if (!input->rdopt)
  {
    // Residue Color Transform
    if(!img->residue_transform_flag)
    {
      // get prediction and prediction error
      for (j=0; j<4; j++)
        for (i=0; i<4; i++)
        {
          img->mpr[block_x+i][block_y+j]  = img->mprr[best_ipmode][j][i];
          img->m7[i][j]                   = imgY_org[pic_opix_y+j][pic_opix_x+i] - img->mprr[best_ipmode][j][i];
        }
      nonzero = dct_luma (block_x, block_y, &dummy, 1);
    } else 
    {
      for (j=0; j<4; j++)
        for (i=0; i<4; i++)
        {
          residue_B = imgUV_org[0][pic_opix_y+j][pic_opix_x+i] - img->mprr_c[0][c_ipmode][block_x+i][block_y+j];
          residue_G = imgY_org[pic_opix_y+j][pic_opix_x+i] - img->mprr[best_ipmode][j][i];
          residue_R = imgUV_org[1][pic_opix_y+j][pic_opix_x+i] - img->mprr_c[1][c_ipmode][block_x+i][block_y+j];
          
          /* Forward Residue Transform */
          resTrans_R[i][j] = residue_R-residue_B;
          temp = residue_B+(resTrans_R[i][j]>>1);
          resTrans_B[i][j] = residue_G-temp;
          resTrans_G[i][j] = temp+(resTrans_B[i][j]>>1);
        }

      for (j=0; j<4; j++)
        for (i=0; i<4; i++)
        {
          img->m7[i][j]  = resTrans_G[i][j];
        }
      nonzero = dct_luma (block_x, block_y, &dummy, 1);
      for (j=0; j<4; j++)
        for (i=0; i<4; i++)
        {
          rec_resG[i][j] = img->m7[i][j];
          img->m7[i][j]  = resTrans_R[i][j];
        }
      cbp_chroma_block[0][2*(b8%2)+(b4%2)][2*(b8/2)+(b4/2)] = dct_chroma4x4 (0, b8+4, b4);
      dc_level[0][2*(b8%2)+(b4%2)][2*(b8/2)+(b4/2)] = dc_level_temp[0][2*(b8%2)+(b4%2)][2*(b8/2)+(b4/2)];
      for (j=0; j<4; j++)
        for (i=0; i<4; i++)
        {
          rec_resR[i][j] = img->m7[i][j];
          img->m7[i][j]  = resTrans_B[i][j];
        }
      cbp_chroma_block[1][2*(b8%2)+(b4%2)][2*(b8/2)+(b4/2)] = dct_chroma4x4 (1, b8+8, b4);
      dc_level[1][2*(b8%2)+(b4%2)][2*(b8/2)+(b4/2)] = dc_level_temp[1][2*(b8%2)+(b4%2)][2*(b8/2)+(b4/2)];
      for (j=0; j<4; j++)
        for (i=0; i<4; i++)
        {
          rec_resB[i][j] = img->m7[i][j];
        }
      for (j=0; j<4; j++)
        for (i=0; i<4; i++)
        {
          /* Inverse Residue Transform */
          temp      = rec_resG[i][j]-(rec_resB[i][j]>>1);
          residue_G = rec_resB[i][j]+temp;
          residue_B = temp - (rec_resR[i][j]>>1);
          residue_R = residue_B+rec_resR[i][j];
          enc_picture->imgUV[0][pic_pix_y+j][pic_pix_x+i] = min(img->max_imgpel_value_uv,max(0,residue_B+(int)img->mprr_c[0][c_ipmode][block_x+i][block_y+j]));
          enc_picture->imgY[pic_pix_y+j][pic_pix_x+i]     = min(img->max_imgpel_value,max(0,residue_G+(int)img->mprr[best_ipmode][j][i]));
          enc_picture->imgUV[1][pic_pix_y+j][pic_pix_x+i] = min(img->max_imgpel_value_uv,max(0,residue_R+(int)img->mprr_c[1][c_ipmode][block_x+i][block_y+j]));
        }
    }
  }
  else
  {
    //===== restore coefficients =====
    for (j=0; j<2; j++)
      for (i=0; i<18;i++)  img->cofAC[b8][b4][j][i]=cofAC4x4[j][i];

    // Residue Color Transform
    if(img->residue_transform_flag)
    {
      for (j=0; j<2; j++)
        for (i=0; i<18;i++)  img->cofAC[b8+4][b4][j][i]=cofAC4x4_chroma[0][j][i];
      for (j=0; j<2; j++)
        for (i=0; i<18;i++)  img->cofAC[b8+8][b4][j][i]=cofAC4x4_chroma[1][j][i];
    }

    //===== restore reconstruction and prediction (needed if single coeffs are removed) =====
    for (y=0; y<4; y++)
      for (x=0; x<4; x++)
      {
        enc_picture->imgY[pic_pix_y+y][pic_pix_x+x] = rec4x4[y][x];
        img->mpr[block_x+x][block_y+y] = img->mprr[best_ipmode][y][x];
      }

    // Residue Color Transform
    if(img->residue_transform_flag)
    {
      for (i=0; i<2; i++)
      { //uv
        //--- set reconstruction ---
        for (y=0; y<4; y++)
          for (x=0; x<4; x++) enc_picture->imgUV[i][pic_pix_y+y][pic_pix_x+x] = rec4x4_c[i][y][x] ;
      }
    }

  }

  return nonzero;
}


/*!
 *************************************************************************************
 * \brief
 *    Mode Decision for an 8x8 Intra block
 *************************************************************************************
 */
int Mode_Decision_for_8x8IntraBlocks(int b8,double lambda,int *cost)
{
  int  nonzero=0, b4;
  int  cost4x4;

  *cost = (int)floor(6.0 * lambda + 0.4999);

  for (b4=0; b4<4; b4++)
  {
    if (Mode_Decision_for_4x4IntraBlocks (b8, b4, lambda, &cost4x4))
    {
      nonzero        = 1;
    }
    *cost += cost4x4;
  }

  return nonzero;
}

/*!
 *************************************************************************************
 * \brief
 *    4x4 Intra mode decision for an macroblock
 *************************************************************************************
 */
int Mode_Decision_for_Intra4x4Macroblock (double lambda,  int* cost)

{
  int  cbp=0, b8, cost8x8;

  for (*cost=0, b8=0; b8<4; b8++)
  {
    if (Mode_Decision_for_8x8IntraBlocks (b8, lambda, &cost8x8))
    {
      cbp |= (1<<b8);
    }
    *cost += cost8x8;
  }

  return cbp;
}


/*!
 *************************************************************************************
 * \brief
 *    R-D Cost for an 8x8 Partition
 *************************************************************************************
 */
double RDCost_for_8x8blocks (int*    cnt_nonz,   // --> number of nonzero coefficients
                             int64*  cbp_blk,    // --> cbp blk
                             double  lambda,     // <-- lagrange multiplier
                             int     block,      // <-- 8x8 block number
                             int     mode,       // <-- partitioning mode
                             short   pdir,       // <-- prediction direction
                             short   ref,        // <-- reference frame
                             short   bwd_ref)    // <-- abp type
{
  int  i, j, k;
  int  rate=0;
  int64 distortion=0;
  int  dummy, mrate;
  int  fw_mode, bw_mode;
  int  cbp     = 0;
  int  pax     = 8*(block%2);
  int  pay     = 8*(block/2);
  int  i0      = pax/4;
  int  j0      = pay/4;
  int  bframe  = (img->type==B_SLICE);
  int  direct  = (bframe && mode==0);
  int  b8value = B8Mode2Value (mode, pdir);

  Macroblock    *currMB    = &img->mb_data[img->current_mb_nr];
  SyntaxElement *currSE    = &img->MB_SyntaxElements[currMB->currSEnr];
  Slice         *currSlice = img->currentSlice;
  DataPartition *dataPart;
  const int     *partMap   = assignSE2partition[input->partition_mode];

  EncodingEnvironmentPtr eep_dp;

  // Residue Color Transform
  int residue_R, residue_G, residue_B, temp, b4;
  int b4_x, b4_y;

  //=====
  //=====  GET COEFFICIENTS, RECONSTRUCTIONS, CBP
  //=====
#if BI_PREDICTION
  currMB->bi_pred_me=0;
#endif

  if (direct)
  {
    if (direct_pdir[img->block_x+i0][img->block_y+j0]<0) // mode not allowed
    {
      return (1e20);
    }
    else
    {
      *cnt_nonz = LumaResidualCoding8x8 (&cbp, cbp_blk, block, direct_pdir[img->block_x+i0][img->block_y+j0], 0, 0, (short)max(0,direct_ref_idx[LIST_0][img->block_x+i0][img->block_y+j0]), direct_ref_idx[LIST_1][img->block_x+i0][img->block_y+j0]);
    }
  }
  else
  {
    fw_mode   = (pdir==0||pdir==2 ? mode : 0);
    bw_mode   = (pdir==1||pdir==2 ? mode : 0);
    *cnt_nonz = LumaResidualCoding8x8 (&cbp, cbp_blk, block, pdir, fw_mode, bw_mode, ref, bwd_ref);
  }

  // Residue Color Transform
  if(img->residue_transform_flag)
  {
    for(b4 = 0; b4 < 4; b4++)
    {
      b4_x = pax+(b4%2)*4;
      b4_y = pay+(b4/2)*4;
      for (j=0; j<4; j++)
      for (i=0; i<4; i++)
      {
        img->m7[i][j]  = resTrans_R[i+b4_x][j+b4_y];
      }
      rate += RDCost_for_4x4Blocks_Chroma (block+4, b4, 0);
      for (j=0; j<4; j++)
      for (i=0; i<4; i++)
      {
        rec_resR[i+b4_x][j+b4_y] = img->m7[i][j];
        img->m7[i][j]  = resTrans_B[i+b4_x][j+b4_y];
      }
      rate += RDCost_for_4x4Blocks_Chroma (block+8, b4, 1);
      for (j=0; j<4; j++)
      for (i=0; i<4; i++)
      {
        rec_resB[i+b4_x][j+b4_y] = img->m7[i][j];
      }
    }

    /* Inverse Residue Transform */
    for (j=pay; j<pay+8; j++)
    for (i=pax; i<pax+8; i++)
    {
      /* YCoCg-R */
      temp      = rec_resG[i][j]-(rec_resB[i][j]>>1);
      residue_G = rec_resB[i][j]+temp;
      residue_B = temp - (rec_resR[i][j]>>1);
      residue_R = residue_B+rec_resR[i][j];

      enc_picture->imgUV[0][img->pix_y+j][img->pix_x+i] = min(img->max_imgpel_value_uv,max(0,residue_B+mprRGB[1][i][j]));
      enc_picture->imgY[img->pix_y+j][img->pix_x+i]     = min(img->max_imgpel_value,max(0,residue_G+mprRGB[0][i][j]));
      enc_picture->imgUV[1][img->pix_y+j][img->pix_x+i] = min(img->max_imgpel_value_uv,max(0,residue_R+mprRGB[2][i][j]));
    }
  }

  //===== get residue =====
  if (input->rdopt==2 && img->type!=B_SLICE)
  {
    // We need the reconstructed prediction residue for the simulated decoders.
    compute_residue_b8block (block, -1);
  }

  //=====
  //=====   GET DISTORTION
  //=====
  if (input->rdopt==2 && img->type!=B_SLICE)
  {
    for (k=0; k<input->NoOfDecoders ;k++)
    {
      decode_one_b8block (k, P8x8, block, mode, ref);
      for (j=img->opix_y+pay; j<img->opix_y+pay+8; j++)
      for (i=img->opix_x+pax; i<img->opix_x+pax+8; i++)
      {
        distortion += img->quad[imgY_org[j][i] - decs->decY[k][j][i]];
      }
    }
    distortion /= input->NoOfDecoders;
  }
  else
  {
    for (j=pay; j<pay+8; j++)
    for (i=img->pix_x+pax; i<img->pix_x+pax+8; i++)
    {
      distortion += img->quad [imgY_org[img->opix_y+j][i] - enc_picture->imgY[img->pix_y+j][i]];
      // Residue Color Transform
      if(img->residue_transform_flag)
      {
        distortion += img->quad [imgUV_org[0][img->opix_y+j][i] - enc_picture->imgUV[0][img->pix_y+j][i]];
        distortion += img->quad [imgUV_org[1][img->opix_y+j][i] - enc_picture->imgUV[1][img->pix_y+j][i]];
      }
    }
  }

  //=====
  //=====   GET RATE
  //=====
  //----- block 8x8 mode -----
  if (input->symbol_mode == UVLC)
  {
    ue_linfo (b8value, dummy, &mrate, &dummy);
    rate += mrate;
  }
  else
  {
    currSE->value1  = b8value;
    currSE->writing = writeB8_typeInfo_CABAC;
    currSE->type    = SE_MBTYPE;
    dataPart = &(currSlice->partArr[partMap[currSE->type]]);
    dataPart->writeSyntaxElement (currSE, dataPart);
    rate += currSE->len;
    currSE++;
    currMB->currSEnr++;
  }

  //----- motion information -----
  if (!direct)
  {
    if ((img->num_ref_idx_l0_active > 1 ) && (pdir==0 || pdir==2))
      rate  += writeReferenceFrame (mode, i0, j0, 1, ref);
    if(img->num_ref_idx_l1_active > 1 && img->type== B_SLICE)
    {
      if (pdir==1 || pdir==2)
      {
        rate  += writeReferenceFrame (mode, i0, j0, 0, bwd_ref);
      }
    }

    if (pdir==0 || pdir==2)
    {
      rate  += writeMotionVector8x8 (i0, j0, i0+2, j0+2, ref,LIST_0, mode);
    }
    if (pdir==1 || pdir==2)
    {
      rate  += writeMotionVector8x8 (i0, j0, i0+2, j0+2, bwd_ref, LIST_1, mode);
    }
  }

  //----- coded block pattern (for CABAC only) -----
  if (input->symbol_mode == CABAC)
  {
    dataPart = &(currSlice->partArr[partMap[SE_CBP_INTER]]);
    eep_dp   = &(dataPart->ee_cabac);
    mrate    = arienco_bits_written (eep_dp);
    writeCBP_BIT_CABAC (block, ((*cnt_nonz>0)?1:0), cbp8x8, currMB, 1, eep_dp);
    mrate    = arienco_bits_written (eep_dp) - mrate;
    rate    += mrate;
  }

  //----- luminance coefficients -----
  if (*cnt_nonz)
  {
    rate += writeLumaCoeff8x8 (block, mode, currMB->luma_transform_size_8x8_flag);
  }

  return (double)distortion + lambda * (double)rate;
}


/*!
 *************************************************************************************
 * \brief
 *    Gets mode offset for intra16x16 mode
 *************************************************************************************
 */
int I16Offset (int cbp, int i16mode)
{
  return (cbp&15?13:1) + i16mode + ((cbp&0x30)>>2);
}


/*!
 *************************************************************************************
 * \brief
 *    Sets modes and reference frames for an macroblock
 *************************************************************************************
 */
void SetModesAndRefframeForBlocks (int mode)
{
  int i,j,k,l;
  Macroblock *currMB = &img->mb_data[img->current_mb_nr];
  int  bframe  = (img->type==B_SLICE);

  int list_offset   = ((img->MbaffFrameFlag)&&(currMB->mb_field))? img->current_mb_nr%2 ? 4 : 2 : 0;

  //--- macroblock type ---
  currMB->mb_type = mode;

#if BI_PREDICTION
  if (mode == 1)
    currMB->bi_pred_me=img->bi_pred_me[mode];  
  else
    currMB->bi_pred_me = 0;
#endif

  //--- block 8x8 mode and prediction direction ---
  switch (mode)
  {
  case 0:
    for(i=0;i<4;i++)
    {
      currMB->b8mode[i] = 0;
      currMB->b8pdir[i] = (bframe?direct_pdir[img->block_x+(i%2)*2][img->block_y+(i/2)*2]:0);
    }
    break;
  case 1:
  case 2:
  case 3:
    for(i=0;i<4;i++)
    {
      currMB->b8mode[i] = mode;
      currMB->b8pdir[i] = best8x8pdir[mode][i];
    }
    break;
  case P8x8:
    for(i=0;i<4;i++)
    {
      currMB->b8mode[i]   = best8x8mode[i];
      currMB->b8pdir[i]   = best8x8pdir[mode][i];
    }
    break;
  case I4MB:
    for(i=0;i<4;i++)
    {
      currMB->b8mode[i] = IBLOCK;
      currMB->b8pdir[i] = -1;
    }
    break;
  case I16MB:
    for(i=0;i<4;i++)
    {
      currMB->b8mode[i] =  0;
      currMB->b8pdir[i] = -1;
    }
    break;
  case I8MB:
    for(i=0;i<4;i++)
    {
      currMB->b8mode[i] = I8MB;
      currMB->b8pdir[i] = -1;
    }
    //switch to 8x8 transform
    currMB->luma_transform_size_8x8_flag = 1;
    break;
  default:
    printf ("Unsupported mode in SetModesAndRefframeForBlocks!\n");
    exit (1);
  }

#define IS_FW ((best8x8pdir[mode][k]==0 || best8x8pdir[mode][k]==2) && (mode!=P8x8 || best8x8mode[k]!=0 || !bframe))
#define IS_BW ((best8x8pdir[mode][k]==1 || best8x8pdir[mode][k]==2) && (mode!=P8x8 || best8x8mode[k]!=0))
  //--- reference frame arrays ---
  if (mode==0 || mode==I4MB || mode==I16MB || mode==I8MB)
  {
    if (bframe)
    {
      for (j=0;j<4;j++)
        for (i=0;i<4;i++)
        {
          if(!mode)
          {     //direct mode
            enc_picture->ref_idx[LIST_0][img->block_x+i][img->block_y+j] = direct_ref_idx[LIST_0][img->block_x+i][img->block_y+j];
            enc_picture->ref_idx[LIST_1][img->block_x+i][img->block_y+j] = direct_ref_idx[LIST_1][img->block_x+i][img->block_y+j];
          }
          else
          {   //intra
            enc_picture->ref_idx[LIST_0][img->block_x+i][img->block_y+j] = -1;
            enc_picture->ref_idx[LIST_1][img->block_x+i][img->block_y+j] = -1;

          }
        }
    }
    else
    {
      for (j=0;j<4;j++)
        for (i=0;i<4;i++)
        {
          enc_picture->ref_idx[LIST_0][img->block_x+i][img->block_y+j] = (mode==0?0:-1);
        }
    }
  }
  else
  {
    if (bframe)
    {
      for (j=0;j<4;j++)
        for (i=0;i<4;i++)
        {
          k = 2*(j/2)+(i/2);
          l = 2*(j%2)+(i%2);

          if(mode == P8x8 && best8x8mode[k]==0)
          {
            enc_picture->ref_idx[LIST_0][img->block_x+i][img->block_y+j] = direct_ref_idx[LIST_0][img->block_x+i][img->block_y+j];
            enc_picture->ref_idx[LIST_1][img->block_x+i][img->block_y+j] = direct_ref_idx[LIST_1][img->block_x+i][img->block_y+j];
          }
          else
          {
#if BI_PREDICTION
            if (mode ==1 && currMB->bi_pred_me && IS_FW && IS_BW)
            {
              enc_picture->ref_idx[LIST_0][img->block_x+i][img->block_y+j] = 0;
              enc_picture->ref_idx[LIST_1][img->block_x+i][img->block_y+j] = 0;
            }
            else
#endif
            {
              enc_picture->ref_idx[LIST_0][img->block_x+i][img->block_y+j] = (IS_FW ? best8x8fwref[mode][k] : -1);
              enc_picture->ref_idx[LIST_1][img->block_x+i][img->block_y+j] = (IS_BW ? best8x8bwref[mode][k] : -1);
            }
          }
        }
    }
    else
    {
      for (j=0;j<4;j++)
        for (i=0;i<4;i++)
        {
          k = 2*(j/2)+(i/2);
          l = 2*(j%2)+(i%2);
          enc_picture->ref_idx[LIST_0][img->block_x+i][img->block_y+j] = (IS_FW ? best8x8fwref[mode][k] : -1);
        }
    }
  }

  for (j=0;j<4;j++)
  {
    for (i=0;i<4;i++)
    {
      enc_picture->ref_pic_id [LIST_0][img->block_x+i][img->block_y+j] =
        (enc_picture->ref_idx[LIST_0][img->block_x+i][img->block_y+j]>=0 ?
         enc_picture->ref_pic_num[LIST_0 + list_offset][enc_picture->ref_idx[LIST_0][img->block_x+i][img->block_y+j]]:
         -1);
    }
  }
  if (bframe)
  {
    for (j=0;j<4;j++)
    {
      for (i=0;i<4;i++)
      {
        enc_picture->ref_pic_id [LIST_1][img->block_x+i][img->block_y+j] =
          (enc_picture->ref_idx[LIST_1][img->block_x+i][img->block_y+j]>=0 ?
           enc_picture->ref_pic_num[LIST_1 + list_offset][enc_picture->ref_idx[LIST_1][img->block_x+i][img->block_y+j]]:
           -1);
      }

    }
  }


#undef IS_FW
#undef IS_BW
}


/*!
 *************************************************************************************
 * \brief
 *    Intra 16x16 mode decision
 *************************************************************************************
 */
void
Intra16x16_Mode_Decision (Macroblock* currMB, int* i16mode)
{
  // Residue Color Transform
  int residue_R, residue_G, residue_B;
  int c_ipmode = img->mb_data[img->current_mb_nr].c_ipred_mode;
  int i, j, temp;
  int pic_pix_x   = img->pix_x;
  int pic_pix_y   = img->pix_y;
  pel_t   **imgY_orig  = imgY_org;
  pel_t   ***imgUV_orig  = imgUV_org;
  int cr_cbp;

  intrapred_luma_16x16 ();   /* make intra pred for all 4 new modes */

  if(!img->residue_transform_flag)
    find_sad_16x16 (i16mode);   /* get best new intra mode */

  // Residue Color Transform
  if(img->residue_transform_flag)
  {
    for (j=0; j<16; j++)
      for (i=0; i<16; i++)
      {
        residue_B = imgUV_orig[0][pic_pix_y+j][pic_pix_x+i] - img->mprr_c[0][c_ipmode][i][j];
        residue_G = imgY_orig[pic_pix_y+j][pic_pix_x+i] - img->mprr_2[*i16mode][j][i];
        residue_R = imgUV_orig[1][pic_pix_y+j][pic_pix_x+i] - img->mprr_c[1][c_ipmode][i][j];
        
        /* Forward Residue Transform */
        resTrans_R[i][j] = residue_R-residue_B;
        temp = residue_B+(resTrans_R[i][j]>>1);
        resTrans_B[i][j] = residue_G-temp;
        resTrans_G[i][j] = temp+(resTrans_B[i][j]>>1);
        
        img->m7[i][j]  = resTrans_G[i][j];
      }
  }

  currMB->cbp = dct_luma_16x16 (*i16mode);

  // Residue Color Transform
  if(img->residue_transform_flag)
  {
    for (j=0; j<16; j++)
      for (i=0; i<16; i++)
      {
        rec_resG[i][j] = img->m7[i][j];
        img->m7[i][j]  = resTrans_R[i][j];
      }

    cr_cbp = dct_chroma(0, 0);

    for (j=0; j<16; j++)
      for (i=0; i<16; i++)
      {
        rec_resR[i][j] = img->m7[i][j];
        img->m7[i][j]  = resTrans_B[i][j];
      }
      
    cr_cbp = dct_chroma(1, cr_cbp);

    for (j=0; j<16; j++)
      for (i=0; i<16; i++)
      {
        rec_resB[i][j] = img->m7[i][j];
      }

    currMB->cbp += (cr_cbp<<4);

    /* Inverse Residue Transform */
    for (j=0; j<16; j++)
      for (i=0; i<16; i++)
      {
        temp      = rec_resG[i][j]-(rec_resB[i][j]>>1);
        residue_G = rec_resB[i][j]+temp;
        residue_B = temp - (rec_resR[i][j]>>1);
        residue_R = residue_B+rec_resR[i][j];
        
        enc_picture->imgUV[0][pic_pix_y+j][pic_pix_x+i] = min(img->max_imgpel_value_uv,max(0,residue_B+(int)img->mprr_c[0][c_ipmode][i][j]));
        enc_picture->imgY[pic_pix_y+j][pic_pix_x+i]     = min(img->max_imgpel_value,max(0,residue_G+(int)img->mprr_2[*i16mode][j][i]));
        enc_picture->imgUV[1][pic_pix_y+j][pic_pix_x+i] = min(img->max_imgpel_value_uv,max(0,residue_R+(int)img->mprr_c[1][c_ipmode][i][j]));
      }
  }
}



/*!
 *************************************************************************************
 * \brief
 *    Sets Coefficients and reconstruction for an 8x8 block
 *************************************************************************************
 */
void SetCoeffAndReconstruction8x8 (Macroblock* currMB)
{
  int block, k, j, i;


  //============= MIXED TRANSFORM SIZES FOR 8x8 PARTITION ==============
  //--------------------------------------------------------------------
  int l;
  int bframe = img->type==B_SLICE;
  int list_offset   = ((img->MbaffFrameFlag)&&(currMB->mb_field))? img->current_mb_nr%2 ? 4 : 2 : 0;

  if (currMB->luma_transform_size_8x8_flag)
  {
    assert(input->AllowTransform8x8);

    //============= set mode and ref. frames ==============
    for(i=0;i<4;i++)
    {
      currMB->b8mode[i]   = part8x8mode[i];
      currMB->b8pdir[i]   = part8x8pdir[i];
    }

    if (bframe)
    {
      for (j=0;j<4;j++)
        for (i=0;i<4;i++)
        {
          k = 2*(j/2)+(i/2);
          l = 2*(j%2)+(i%2);
          enc_picture->ref_idx[LIST_0][img->block_x+i][img->block_y+j] = part8x8fwref[k];
          enc_picture->ref_idx[LIST_1][img->block_x+i][img->block_y+j] = part8x8bwref[k];
        }
    }
    else
    {
      for (j=0;j<4;j++)
        for (i=0;i<4;i++)
        {
          k = 2*(j/2)+(i/2);
          l = 2*(j%2)+(i%2);
          enc_picture->ref_idx[LIST_0][img->block_x+i][img->block_y+j] = part8x8fwref[k];
        }
    }

    for (j=0;j<4;j++)
    {
      for (i=0;i<4;i++)
      {
        enc_picture->ref_pic_id [LIST_0][img->block_x+i][img->block_y+j] =
          (enc_picture->ref_idx[LIST_0][img->block_x+i][img->block_y+j]>=0 ?
          enc_picture->ref_pic_num[LIST_0 + list_offset][enc_picture->ref_idx[LIST_0][img->block_x+i][img->block_y+j]]:
        -1);
      }
    }
    if (bframe)
    {
      for (j=0;j<4;j++)
      {
        for (i=0;i<4;i++)
        {
          enc_picture->ref_pic_id [LIST_1][img->block_x+i][img->block_y+j] =
            (enc_picture->ref_idx[LIST_1][img->block_x+i][img->block_y+j]>=0 ?
            enc_picture->ref_pic_num[LIST_1 + list_offset][enc_picture->ref_idx[LIST_1][img->block_x+i][img->block_y+j]]:
          -1);
        }

      }
    }

    //====== set the mv's for 8x8 partition with transform size 8x8 ======
    //save the mv data for 4x4 transform
    StoreMV8x8(1);
    //set new mv data for 8x8 transform
    RestoreMV8x8(0);

    //============= get pre-calculated data ==============
    //restore coefficients from 8x8 transform
    for (block=0; block<4; block++)
    for (k=0; k<4; k++)
    for (j=0; j<2; j++)
    for (i=0; i<65; i++)
      img->cofAC[block][k][j][i] = cofAC_8x8ts[block][k][j][i];
    
    //restore reconstruction 
    if (cnt_nonz8_8x8ts <= _LUMA_8x8_COEFF_COST_ && 
        ((img->qp + img->bitdepth_luma_qp_scale)!=0 || img->lossless_qpprime_flag==0))
    {
      currMB->cbp     = 0;
      currMB->cbp_blk = 0;
      for (j=0; j<16; j++)
        for (i=0; i<16; i++)
          // Residue Color Transform
          if(!img->residue_transform_flag)
            enc_picture->imgY[img->pix_y+j][img->pix_x+i] = mpr_8x8ts[j][i];
          else
            rec_resG[i][j] = 0;
    }
    else
    {
      currMB->cbp     = cbp8_8x8ts;
      currMB->cbp_blk = cbp_blk8_8x8ts;
      for (j=0; j<16; j++)
        for (i=0; i<16; i++)
          // Residue Color Transform
          if(!img->residue_transform_flag)
            enc_picture->imgY[img->pix_y+j][img->pix_x+i] = rec_mbY_8x8ts[j][i];
          else
            rec_resG[i][j] =  rec_resG_8x8ts[i][j];
    }

    // Residue Color Transform
    for (j=0; j<16; j++)
    for (i=0; i<16; i++)
    {
      mprRGB[0][i][j] = mprRGB_8x8ts[0][i][j];
      mprRGB[1][i][j] = mprRGB_8x8ts[1][i][j];
      mprRGB[2][i][j] = mprRGB_8x8ts[2][i][j];
      resTrans_R[i][j] = resTrans_R_8x8ts[i][j];
      resTrans_B[i][j] = resTrans_B_8x8ts[i][j];
    }
  }
  else
  {
    //============= get pre-calculated data ==============
    //---------------------------------------------------
    //--- restore coefficients ---
    for (block=0; block<4+img->num_blk8x8_uv; block++)
    for (k=0; k<4; k++)
    for (j=0; j<2; j++)
    for (i=0; i<65; i++)
      img->cofAC[block][k][j][i] = cofAC8x8[block][k][j][i];

    if (cnt_nonz_8x8<=5 && img->type!=SP_SLICE &&
        ((img->qp + img->bitdepth_luma_qp_scale)!=0 || img->lossless_qpprime_flag==0))
    {
      currMB->cbp     = 0;
      currMB->cbp_blk = 0;
      for (j=0; j<16; j++)
      for (i=0; i<16; i++)
        // Residue Color Transform
        if(!img->residue_transform_flag)
          enc_picture->imgY[img->pix_y+j][img->pix_x+i] = mpr8x8[j][i];
        else
          rec_resG[i][j] = 0;
    }
    else
    {
      currMB->cbp     = cbp8x8;
      currMB->cbp_blk = cbp_blk8x8;
      for (j=0; j<16; j++)
      for (i=0; i<16; i++)
        // Residue Color Transform
        if(!img->residue_transform_flag)
          enc_picture->imgY[img->pix_y+j][img->pix_x+i] = rec_mbY8x8[j][i];
        else
          rec_resG[i][j] =  rec_resG_8x8[i][j];
    }

    // Residue Color Transform
    for (j=0; j<16; j++)
    for (i=0; i<16; i++)
    {
      mprRGB[0][i][j] = mprRGB_8x8[0][i][j];
      mprRGB[1][i][j] = mprRGB_8x8[1][i][j];
      mprRGB[2][i][j] = mprRGB_8x8[2][i][j];
      resTrans_R[i][j] = resTrans_R_8x8[i][j];
      resTrans_B[i][j] = resTrans_B_8x8[i][j];
    }
  }
}


/*!
 *************************************************************************************
 * \brief
 *    Sets motion vectors for an macroblock
 *************************************************************************************
 */
void SetMotionVectorsMB (Macroblock* currMB, int bframe)
{
  int i, j, k, l, m, mode8, pdir8, ref, by, bx, bxr;
  short ******all_mv  = img->all_mv;
  short ******pred_mv = img->pred_mv;
  int  bw_ref;

#ifdef PMVFAST  
  int mvblocktype = input->InterSearch8x8? 4 : 1;
#endif

  for (j=0; j<4; j++)
    for (i=0; i<4; i++)
    {
      mode8 = currMB->b8mode[k=2*(j/2)+(i/2)];
      l     = 2*(j%2)+(i%2);
      by    = img->block_y+j;
      bxr   = img->block_x+i;
      bx    = img->block_x+i+4;
      
      pdir8 = currMB->b8pdir[k];
      ref    = enc_picture->ref_idx[LIST_0][bxr][by];
      bw_ref = enc_picture->ref_idx[LIST_1][bxr][by];
      
#if BI_PREDICTION
      if (currMB->bi_pred_me && (pdir8 == 2) && currMB->mb_type==1)
      {
        all_mv  = currMB->bi_pred_me == 1 ? img->bipred_mv1 : img->bipred_mv2;
        ref = 0;
        bw_ref = 0;
      }
#endif
      
      if (!bframe)
      {
        if (pdir8>=0) //(mode8!=IBLOCK)&&(mode8!=I16MB))  // && ref != -1)
        {
          enc_picture->mv[LIST_0][bxr][by][0] = all_mv [i][j][LIST_0][ ref][mode8][0];
          enc_picture->mv[LIST_0][bxr][by][1] = all_mv [i][j][LIST_0][ ref][mode8][1];
        }
        else
        {
          enc_picture->mv[LIST_0][bxr][by][0] = 0;
          enc_picture->mv[LIST_0][bxr][by][1] = 0;
        }
      }
      else
      {
        if (pdir8==-1) // intra
        {
          enc_picture->mv[LIST_0][bxr][by][0] = 0;
          enc_picture->mv[LIST_0][bxr][by][1] = 0;
          enc_picture->mv[LIST_1][bxr][by][0] = 0;
          enc_picture->mv[LIST_1][bxr][by][1] = 0;
        }
        else if (pdir8==0) // forward
        {
          enc_picture->mv[LIST_0][bxr][by][0] = all_mv [i][j][LIST_0][ ref][mode8][0];
          enc_picture->mv[LIST_0][bxr][by][1] = all_mv [i][j][LIST_0][ ref][mode8][1];
          enc_picture->mv[LIST_1][bxr][by][0] = 0;
          enc_picture->mv[LIST_1][bxr][by][1] = 0;
        }
        else if (pdir8==1) // backward
        {
          enc_picture->mv[LIST_0][bxr][by][0] = 0;
          enc_picture->mv[LIST_0][bxr][by][1] = 0;

          enc_picture->mv[LIST_1][bxr][by][0] = all_mv[i][j][LIST_1][bw_ref][mode8][0];
          enc_picture->mv[LIST_1][bxr][by][1] = all_mv[i][j][LIST_1][bw_ref][mode8][1];
        }
        else if (pdir8==2) // bidir
        {
          enc_picture->mv[LIST_0][bxr][by][0] = all_mv [i][j][LIST_0][ ref][mode8][0];
          enc_picture->mv[LIST_0][bxr][by][1] = all_mv [i][j][LIST_0][ ref][mode8][1];


          enc_picture->mv[LIST_1][bxr][by][0] = all_mv[i][j][LIST_1][bw_ref][mode8][0];
          enc_picture->mv[LIST_1][bxr][by][1] = all_mv[i][j][LIST_1][bw_ref][mode8][1];
        }
        else
        {
          error("invalid direction mode", 255);
        }
      }
  }

  // copy all the motion vectors into rdopt structure
  // Can simplify this by copying the MV's of the best mode (TBD)
  if(img->MbaffFrameFlag)
  {
    for(i=0;i<4;i++)
    {
      for(j=0;j<4;j++)
      {
        for (k=0;k<2;k++)
        {
          for(l=0;l<img->max_num_references;l++)
          {
            for(m=0;m<9;m++)
            {
              rdopt->all_mv [i][j][k][l][m][0]  = all_mv [i][j][k][l][m][0];
              rdopt->pred_mv[i][j][k][l][m][0]  = pred_mv[i][j][k][l][m][0];

              rdopt->all_mv [i][j][k][l][m][1]  = all_mv [i][j][k][l][m][1];
              rdopt->pred_mv[i][j][k][l][m][1]  = pred_mv[i][j][k][l][m][1];
            }
          }
        }
      }
    }
  }
}



/*!
 *************************************************************************************
 * \brief
 *    R-D Cost for a macroblock
 *************************************************************************************
 */
int RDCost_for_macroblocks (double   lambda,       // <-- lagrange multiplier
                            int      mode,         // <-- modus (0-COPY/DIRECT, 1-16x16, 2-16x8, 3-8x16, 4-8x8(+), 5-Intra4x4, 6-Intra16x16)
                            double*  min_rdcost,   // <-> minimum rate-distortion cost
                            int i16mode )
{
  int         i, j, k; //, k, ****ip4;
  int         rate=0;
  int64       distortion=0;
  double      rdcost;
  Macroblock  *currMB   = &img->mb_data[img->current_mb_nr];
  Macroblock  *prevMB   = img->current_mb_nr ? &img->mb_data[img->current_mb_nr-1] : NULL;
  int         bframe    = (img->type==B_SLICE);
  int         tmp_cc;
  int         use_of_cc =  (img->type!=I_SLICE &&  input->symbol_mode!=CABAC);
  int         cc_rate, dummy;
  int         cr_cbp=0, uv;

  //=====
  //=====  SET REFERENCE FRAMES AND BLOCK MODES
  //=====
  SetModesAndRefframeForBlocks (mode);

  //=====
  //=====  GET COEFFICIENTS, RECONSTRUCTIONS, CBP
  //=====
  if (bframe && mode==0)
  {
    int block_x=img->pix_x>>2;
    int block_y=img->pix_y>>2;
    for (i=0;i<4;i++)
      for (j=0;j<4;j++)
        if (direct_pdir[block_x+i][block_y+j]<0)
          return 0;
  }

  if (mode<P8x8)
  {
    LumaResidualCoding ();
    if(mode==0 && currMB->cbp==0 && currMB->luma_transform_size_8x8_flag==1) //for B_skip, luma_transform_size_8x8_flag=0 only
      return 0;
  }
  else if (mode==P8x8)
  {
    SetCoeffAndReconstruction8x8 (currMB);
  }
  else if (mode==I4MB)
  {
    currMB->cbp = Mode_Decision_for_Intra4x4Macroblock (lambda, &dummy);

    // Residue Color Transform
    if(img->residue_transform_flag)
    {
      for(i=0; i<2; i++)
      for(j=0; j<4; j++)
      for(k=0; k<4; k++)
        if(cbp_chroma_block[i][j][k])
          cr_cbp = 2;

      for(uv=0; uv<2; uv++)
        cr_cbp = dct_chroma_DC(uv, cr_cbp);

      currMB->cbp += (cr_cbp<<4);
    }
  }
  else if (mode==I16MB)
  {
    Intra16x16_Mode_Decision  (currMB, &i16mode);
  }
  else if(mode==I8MB){
    currMB->cbp = Mode_Decision_for_new_Intra8x8Macroblock(lambda, &dummy);

    // Residue Color Transform
    if(img->residue_transform_flag)
    {
      for(i=0; i<2; i++)
      for(j=0; j<4; j++)
      for(k=0; k<4; k++)
        if(cbp_chroma_block[i][j][k])
          cr_cbp = 2;

      for(uv=0; uv<2; uv++)
        cr_cbp = dct_chroma_DC(uv, cr_cbp);

      currMB->cbp += (cr_cbp<<4);
    }
  }

  if (input->rdopt==2 && img->type!=B_SLICE)
  {
    // We need the reconstructed prediction residue for the simulated decoders.
    compute_residue_mb (mode==I16MB?i16mode:-1);
  }

  //Rate control
  if (mode == I16MB)
  {
    for(i=0; i<16; i++)
    for(j=0; j<16; j++)
      pred[j][i] = img->mprr_2[i16mode][j][i];
  }else
  {
    for(i=0; i<16; i++)
    for(j=0; j<16; j++)
      pred[j][i] = img->mpr[i][j];
  }

  img->i16offset = 0;
  dummy = 0;
  if ((!(img->residue_transform_flag && (mode==I4MB || mode==I16MB || mode==I8MB))) && img->yuv_format!=YUV400)
    ChromaResidualCoding (&dummy);
  if (mode==I16MB)     img->i16offset = I16Offset  (currMB->cbp, i16mode);

  //=====
  //=====   GET DISTORTION
  //=====
  // LUMA
  if (input->rdopt==2 && img->type!=B_SLICE)
  {
    for (k=0; k<input->NoOfDecoders ;k++)
    {
      decode_one_mb (k, currMB);
      for (j=0; j<MB_BLOCK_SIZE; j++)
      for (i=img->opix_x; i<img->opix_x+MB_BLOCK_SIZE; i++)
      {
        distortion += img->quad [imgY_org[img->opix_y+j][i] - decs->decY[k][img->opix_y+j][i]];
      }
    }
    distortion /= input->NoOfDecoders;
  }
  else
  {
    for (j=0; j<16; j++)
    for (i=img->opix_x; i<img->opix_x+16; i++)
    {
      distortion += img->quad [imgY_org[j+img->opix_y][i] - enc_picture->imgY[img->pix_y+j][i]];
    }
  }

  if (img->yuv_format != YUV400)
  {
    // CHROMA
    for (j=0; j<img->mb_cr_size_y; j++)
    for (i=img->opix_c_x; i<img->opix_c_x+img->mb_cr_size_x; i++)
    {
      distortion += img->quad [imgUV_org[0][j+img->opix_c_y][i] - enc_picture->imgUV[0][img->pix_c_y+j][i]];
      distortion += img->quad [imgUV_org[1][j+img->opix_c_y][i] - enc_picture->imgUV[1][img->pix_c_y+j][i]];
    }
  }


  //=====   S T O R E   C O D I N G   S T A T E   =====
  //---------------------------------------------------
  store_coding_state (cs_cm);

  //=====
  //=====   GET RATE
  //=====
  //----- macroblock header -----
  if (use_of_cc)
  {
    if (currMB->mb_type!=0 || (bframe && currMB->cbp!=0))
    {
      // cod counter and macroblock mode are written ==> do not consider code counter
      tmp_cc = img->cod_counter;
      rate   = writeMBLayer (1);
      ue_linfo (tmp_cc, dummy, &cc_rate, &dummy);
      rate  -= cc_rate;
      img->cod_counter = tmp_cc;
    }
    else
    {
      // cod counter is just increased  ==> get additional rate
      ue_linfo (img->cod_counter+1, dummy, &rate,    &dummy);
      ue_linfo (img->cod_counter,   dummy, &cc_rate, &dummy);
      rate -= cc_rate;
    }
  }
  else
  {
    rate = writeMBLayer (1);
  }

  //=====   R E S T O R E   C O D I N G   S T A T E   =====
  //-------------------------------------------------------
  reset_coding_state (cs_cm);

  rdcost = (double)distortion + lambda * max(0.5,(double)rate);

#if defined(SPEC_CPU)
   if (((rdcost - *min_rdcost) >= -1e-12) ||
#else
  if (rdcost >= *min_rdcost ||
#endif
      ((img->qp + img->bitdepth_luma_qp_scale)==0 && img->lossless_qpprime_flag==1 && distortion!=0))
  {
    return 0;
  }


  if ((img->MbaffFrameFlag) && (mode ? 0: ((img->type == B_SLICE) ? !currMB->cbp:1)))  // AFF and current is skip
  {
    if (img->current_mb_nr%2) //bottom
    {
      if (prevMB->mb_type ? 0:((img->type == B_SLICE) ? !prevMB->cbp:1)) //top is skip
      {
        if (!(field_flag_inference() == currMB->mb_field)) //skip only allowed when correct inference
          return 0;
      }
    }
  }

  //=====   U P D A T E   M I N I M U M   C O S T   =====
  //-----------------------------------------------------
  *min_rdcost = rdcost;
  return 1;
}





/*!
 *************************************************************************************
 * \brief
 *    Store macroblock parameters
 *************************************************************************************
 */
void store_macroblock_parameters (int mode)
{
  int  i, j, k, ****i4p, ***i3p;
  Macroblock *currMB  = &img->mb_data[img->current_mb_nr];
  int        bframe   = (img->type==B_SLICE);

  //--- store best mode ---
  best_mode = mode;
  best_c_imode = currMB->c_ipred_mode;
  best_i16offset = img->i16offset;
#if BI_PREDICTION
  // If condition is not really necessary.
  if (mode == 1)
    bi_pred_me=currMB->bi_pred_me;  
  else
    bi_pred_me=0;
#endif

  for (i=0; i<4; i++)
  {
    b8mode[i]   = currMB->b8mode[i];
    b8pdir[i]   = currMB->b8pdir[i];
  }
  // Residue Color Transform
  for (k=0, j=img->block_y; j<img->block_y+4; j++)
  for (i=img->block_x; i<img->block_x+4; i++, k++)
  {
    b4_ipredmode[k] = img->ipredmode[i][j];
    b4_intra_pred_modes[k] = currMB->intra_pred_modes[k];
  }
  for(i = k = 0; i < 4; i++)
  for(j = 0; j < 4; j++,k++)
  {
    b8_ipredmode8x8[i][j] = img->ipredmode8x8[img->block_x+i][img->block_y+j];
    //only every fourth value relevant
    b8_intra_pred_modes8x8[k] = currMB->intra_pred_modes8x8[k];
  }

  //--- reconstructed blocks ----
  for (j=0; j<16; j++)
  for (i=0; i<16; i++)
  {
    rec_mbY[j][i] = enc_picture->imgY[img->pix_y+j][img->pix_x+i];
  }

  if (img->yuv_format != YUV400)
  {
    for (j=0; j<img->mb_cr_size_y; j++)
    for (i=0; i<img->mb_cr_size_x; i++)
    {
      rec_mbU[j][i] = enc_picture->imgUV[0][img->pix_c_y+j][img->pix_c_x+i];
      rec_mbV[j][i] = enc_picture->imgUV[1][img->pix_c_y+j][img->pix_c_x+i];
    }
  }


  //--- store results of decoders ---
  if (input->rdopt==2 && img->type!=B_SLICE)
  {
    for (k=0; k<input->NoOfDecoders; k++)
    {
      for (j=img->pix_y; j<img->pix_y+16; j++)
      for (i=img->pix_x; i<img->pix_x+16; i++)
      {
        // Keep the decoded values of each MB for updating the ref frames
        decs->decY_best[k][j][i] = decs->decY[k][j][i];
      }
    }
  }

  //--- coeff, cbp, kac ---
  if (mode || bframe)
  {
    i4p=cofAC; cofAC=img->cofAC; img->cofAC=i4p;
    i3p=cofDC; cofDC=img->cofDC; img->cofDC=i3p;
    cbp     = currMB->cbp;
    cbp_blk = currMB->cbp_blk;
  }
  else
  {
    cbp_blk = cbp = 0;
  }

  //--- store transform size ---
  luma_transform_size_8x8_flag = currMB->luma_transform_size_8x8_flag;
//  NoMbPartLessThan8x8Flag  = currMB->NoMbPartLessThan8x8Flag;   //DEL-VG-29072004


  for (j=0; j<4; j++)
  for (i=0; i<4; i++)
  {
    frefframe[j][i] = enc_picture->ref_idx[LIST_0][img->block_x+i][img->block_y+j];

    if (bframe)
    {
      brefframe[j][i] = enc_picture->ref_idx[LIST_1][img->block_x+i][img->block_y+j];
    }
  }

}


/*!
 *************************************************************************************
 * \brief
 *    Set stored macroblock parameters
 *************************************************************************************
 */
void set_stored_macroblock_parameters ()
{
  int  i, j, k, ****i4p, ***i3p,l;
  Macroblock  *currMB  = &img->mb_data[img->current_mb_nr];
  int         mode     = best_mode;
  int         bframe   = (img->type==B_SLICE);
  int         **ipredmodes = img->ipredmode;

  imgpel        **imgY  = enc_picture->imgY;
  imgpel       ***imgUV = enc_picture->imgUV;

  int        list_offset   = ((img->MbaffFrameFlag)&&(currMB->mb_field))? img->current_mb_nr%2 ? 4 : 2 : 0;

  //===== reconstruction values =====
  for (j=0; j<16; j++)
  for (i=0; i<16; i++)
  {
    imgY[img->pix_y+j][img->pix_x+i] = rec_mbY[j][i];

    if(img->MbaffFrameFlag)
      rdopt->rec_mbY[j][i]       = rec_mbY[j][i];
  }

  if (img->yuv_format != YUV400)
  {
    for (j=0; j<img->mb_cr_size_y; j++)
      for (i=0; i<img->mb_cr_size_x; i++)
      {
        imgUV[0][img->pix_c_y+j][img->pix_c_x+i] = rec_mbU[j][i];
        imgUV[1][img->pix_c_y+j][img->pix_c_x+i] = rec_mbV[j][i];
        
        if(img->MbaffFrameFlag)
        {
          rdopt->rec_mbU[j][i]           = rec_mbU[j][i];
          rdopt->rec_mbV[j][i]           = rec_mbV[j][i];
        }
      }
  }

  //===== coefficients and cbp =====
  i4p=cofAC; cofAC=img->cofAC; img->cofAC=i4p;
  i3p=cofDC; cofDC=img->cofDC; img->cofDC=i3p;
  currMB->cbp      = cbp;
  currMB->cbp_blk = cbp_blk;
  //==== macroblock type ====
  currMB->mb_type = mode;

  if(img->MbaffFrameFlag)
  {
    rdopt->mode = mode;
    rdopt->i16offset = img->i16offset;
    rdopt->cbp = cbp;
    rdopt->cbp_blk = cbp_blk;
    rdopt->mb_type  = mode;

    rdopt->prev_qp=currMB->prev_qp;
    rdopt->prev_delta_qp=currMB->prev_delta_qp;
    rdopt->qp=currMB->qp;

    for(i=0;i<4+img->num_blk8x8_uv;i++)
      for(j=0;j<4;j++)
        for(k=0;k<2;k++)
          for(l=0;l<65;l++)
            rdopt->cofAC[i][j][k][l] = img->cofAC[i][j][k][l];

    for(i=0;i<3;i++)
        for(k=0;k<2;k++)
          for(l=0;l<18;l++)
            rdopt->cofDC[i][k][l] = img->cofDC[i][k][l];
  }


  for (i=0; i<4; i++)
  {
    currMB->b8mode[i]   = b8mode[i];
    currMB->b8pdir[i]   = b8pdir[i];
    if(img->MbaffFrameFlag)
    {
      rdopt->b8mode[i]  = b8mode[i];
      rdopt->b8pdir[i]  = b8pdir[i];
    }
  }

#if BI_PREDICTION
  // Again if condition seems not necessary, Just a precaution
  if (currMB->mb_type == 1)
    currMB->bi_pred_me=bi_pred_me;  
  else
    currMB->bi_pred_me=0;  
#endif

  //if P8x8 mode and transform size 4x4 choosen, restore motion vector data for this transform size 
  if (mode == P8x8 && !luma_transform_size_8x8_flag && input->AllowTransform8x8)
    RestoreMV8x8(1);
  
  //==== transform size flag ====
  if (((currMB->cbp & 15) == 0) && !(IS_OLDINTRA(currMB) || currMB->mb_type == I8MB))
    currMB->luma_transform_size_8x8_flag = 0;
  else
    currMB->luma_transform_size_8x8_flag = luma_transform_size_8x8_flag;

  rdopt->luma_transform_size_8x8_flag  = currMB->luma_transform_size_8x8_flag;


  if (input->rdopt==2 && img->type!=B_SLICE)
  {
    //! save the MB Mode of every macroblock
    decs->dec_mb_mode[img->mb_x][img->mb_y] = mode;
  }

  //==== reference frames =====
  for (j=0; j<4; j++)
  for (i=0; i<4; i++)
  {
    // backward prediction or intra
    if ((currMB->b8pdir[i/2+(j/2)*2] == 1) || IS_INTRA(currMB))
    {
      enc_picture->ref_idx[LIST_0][img->block_x+i][img->block_y+j] = -1;
      enc_picture->ref_pic_id [LIST_0][img->block_x+i][img->block_y+j] = -1;

      enc_picture->mv[LIST_0][img->block_x+i][img->block_y+j][0] =0;
      enc_picture->mv[LIST_0][img->block_x+i][img->block_y+j][1] = 0;
      if(img->MbaffFrameFlag)
        rdopt->refar[LIST_0][j][i] = -1;
    }
    else
    {
#if BI_PREDICTION
      if (currMB->bi_pred_me && (currMB->b8pdir[i/2+(j/2)*2] == 2) && currMB->mb_type==1)
      {
        short   ******bipred_mv = currMB->bi_pred_me == 1 ? img->bipred_mv1 : img->bipred_mv2;

        enc_picture->ref_idx[LIST_0][img->block_x+i][img->block_y+j] = 0; 
        enc_picture->ref_pic_id [LIST_0][img->block_x+i][img->block_y+j] = enc_picture->ref_pic_num[LIST_0 + list_offset][0];
        enc_picture->mv[LIST_0][img->block_x+i][img->block_y+j][0] = bipred_mv[i][j][LIST_0][0][currMB->b8mode[i/2+(j/2)*2]][0];
        enc_picture->mv[LIST_0][img->block_x+i][img->block_y+j][1] = bipred_mv[i][j][LIST_0][0][currMB->b8mode[i/2+(j/2)*2]][1];        
        if(img->MbaffFrameFlag)
          rdopt->refar[LIST_0][j][i] = 0;        
      }
      else
#endif
      {
        enc_picture->ref_idx[LIST_0][img->block_x+i][img->block_y+j] = frefframe[j][i];
        enc_picture->ref_pic_id [LIST_0][img->block_x+i][img->block_y+j] = enc_picture->ref_pic_num[LIST_0 + list_offset][enc_picture->ref_idx[LIST_0][img->block_x+i][img->block_y+j]];
        enc_picture->mv[LIST_0][img->block_x+i][img->block_y+j][0] = img->all_mv[i][j][LIST_0][frefframe[j][i]][currMB->b8mode[i/2+(j/2)*2]][0];
        enc_picture->mv[LIST_0][img->block_x+i][img->block_y+j][1] = img->all_mv[i][j][LIST_0][frefframe[j][i]][currMB->b8mode[i/2+(j/2)*2]][1];
        if(img->MbaffFrameFlag)
          rdopt->refar[LIST_0][j][i] = frefframe[j][i];
      }
    }

    // forward prediction or intra
    if ((currMB->b8pdir[i/2+(j/2)*2] == 0) || IS_INTRA(currMB))
    {
      enc_picture->ref_idx[LIST_1][img->block_x+i][img->block_y+j] = -1;
      enc_picture->ref_pic_id [LIST_1][img->block_x+i][img->block_y+j] = -1;
      enc_picture->mv[LIST_1][img->block_x+i][img->block_y+j][0] =0;
      enc_picture->mv[LIST_1][img->block_x+i][img->block_y+j][1] = 0;
      if(img->MbaffFrameFlag)
        rdopt->refar[LIST_1][j][i] = -1;
    }
  }

  if (bframe)
  {
    for (j=0; j<4; j++)
      for (i=0; i<4; i++)
      {

        // forward
        if (IS_INTRA(currMB)||(currMB->b8pdir[i/2+(j/2)*2] == 0))
        {
          enc_picture->ref_idx[LIST_1][img->block_x+i][img->block_y+j] = -1;
          enc_picture->ref_pic_id [LIST_1][img->block_x+i][img->block_y+j] = -1;
          enc_picture->mv[LIST_1][img->block_x+i][img->block_y+j][0] = 0;
          enc_picture->mv[LIST_1][img->block_x+i][img->block_y+j][1] = 0;
          if(img->MbaffFrameFlag)
            rdopt->refar[LIST_1][j][i] = -1;
        }
        else
        {
#if BI_PREDICTION
          if (currMB->bi_pred_me && (currMB->b8pdir[i/2+(j/2)*2] == 2) && currMB->mb_type==1)
          {
            short   ******bipred_mv = currMB->bi_pred_me == 1 ? img->bipred_mv1 : img->bipred_mv2;

            enc_picture->ref_idx[LIST_1][img->block_x+i][img->block_y+j] = 0; 
            enc_picture->ref_pic_id [LIST_1][img->block_x+i][img->block_y+j] = enc_picture->ref_pic_num[LIST_1 + list_offset][0];
            enc_picture->mv[LIST_1][img->block_x+i][img->block_y+j][0] = bipred_mv[i][j][LIST_1][0][currMB->b8mode[i/2+(j/2)*2]][0];
            enc_picture->mv[LIST_1][img->block_x+i][img->block_y+j][1] = bipred_mv[i][j][LIST_1][0][currMB->b8mode[i/2+(j/2)*2]][1];        
          if(img->MbaffFrameFlag)
            rdopt->refar[LIST_1][j][i] = 0;        
            //printf("mvl0 %d %d\n",enc_picture->mv[LIST_1][img->block_x+i][img->block_y+j][0],enc_picture->mv[LIST_1][img->block_x+i][img->block_y+j][1]);
          }
          else
#endif
          {
            enc_picture->ref_idx[LIST_1][img->block_x+i][img->block_y+j] = brefframe[j][i];
            enc_picture->ref_pic_id [LIST_1][img->block_x+i][img->block_y+j] = enc_picture->ref_pic_num[LIST_1 + list_offset][enc_picture->ref_idx[LIST_1][img->block_x+i][img->block_y+j]];
            enc_picture->mv[LIST_1][img->block_x+i][img->block_y+j][0] = img->all_mv[i][j][LIST_1][brefframe[j][i]][currMB->b8mode[i/2+(j/2)*2]][0];
            enc_picture->mv[LIST_1][img->block_x+i][img->block_y+j][1] = img->all_mv[i][j][LIST_1][brefframe[j][i]][currMB->b8mode[i/2+(j/2)*2]][1];
            if(img->MbaffFrameFlag)
              rdopt->refar[LIST_1][j][i] = brefframe[j][i];
          }
        }
      }
  }

  //==== intra prediction modes ====
  currMB->c_ipred_mode = best_c_imode;
  img->i16offset = best_i16offset;

  if(currMB->mb_type == I8MB)
  {
    for(i = k = 0; i < 4; i++)
      for(j = 0; j < 4; j++,k++)
      {
        img->ipredmode[img->block_x+i][img->block_y+j] = img->ipredmode8x8[img->block_x+i][img->block_y+j];
        //only every fourth value is relevant
        currMB->intra_pred_modes[k] = currMB->intra_pred_modes8x8[k];
      }
  }
  else if (mode!=I4MB && mode!=I8MB)
  {
    for (k=0, j=img->block_y; j<img->block_y+4; j++)
      for (   i=img->block_x; i<img->block_x+4; i++, k++)
      {
        ipredmodes           [i][j] = DC_PRED;
        currMB->intra_pred_modes[k] = DC_PRED;
      }
  }
  // Residue Color Transform
  else if (mode == I4MB)
  {
    for (k=0, j=img->block_y; j<img->block_y+4; j++)
      for (i=img->block_x; i<img->block_x+4; i++, k++)
      {
        ipredmodes[i][j] = b4_ipredmode[k];
        currMB->intra_pred_modes[k] = b4_intra_pred_modes[k];
      }
  }

  if(currMB->mb_type == I8MB)
  {
    for(i = k = 0; i < 4; i++)
      for(j = 0; j < 4; j++,k++)
      {
        //img->ipredmode[img->block_x+i][img->block_y+j] = img->ipredmode8x8[img->block_x+i][img->block_y+j];
        //only every fourth value relevant
        //currMB->intra_pred_modes[k] = currMB->intra_pred_modes8x8[k];
        img->ipredmode[img->block_x+i][img->block_y+j] = b8_ipredmode8x8[i][j];
        currMB->intra_pred_modes[k] = b8_intra_pred_modes8x8[k];
      }
  }


  if(img->MbaffFrameFlag)
  {
    for (k=0, j=img->block_y; j<img->block_y+4; j++)
      for (   i=img->block_x; i<img->block_x+4; i++, k++)
      {
        rdopt->ipredmode[i][j]     = ipredmodes[i][j];
        rdopt->intra_pred_modes[k] = currMB->intra_pred_modes[k];
      }
    rdopt->c_ipred_mode = currMB->c_ipred_mode;
    rdopt->i16offset = img->i16offset;  // DH
  }

  //==== motion vectors =====
  SetMotionVectorsMB (currMB, bframe);
}



/*!
 *************************************************************************************
 * \brief
 *    Set reference frames and motion vectors
 *************************************************************************************
 */
void SetRefAndMotionVectors (int block, int mode, int pdir, int fwref, int bwref)
{
  int     i, j=0;
  int     bslice  = (img->type==B_SLICE);
  int     pmode   = (mode==1||mode==2||mode==3?mode:4);
  int     j0      = ((block/2)<<1);
  int     i0      = ((block%2)<<1);
  int     j1      = j0 + (input->part_size[pmode][1]);
  int     i1      = i0 + (input->part_size[pmode][0]);

  int list_offset   = ((img->MbaffFrameFlag)&&(img->mb_data[img->current_mb_nr].mb_field))? img->current_mb_nr%2 ? 4 : 2 : 0;
  Macroblock  *currMB  = &img->mb_data[img->current_mb_nr];

  if (pdir<0)
  {
    for (j=j0; j<j1; j++)
    for (i=i0; i<i1; i++)
    {
      enc_picture->mv[LIST_0][img->block_x+i][img->block_y+j][0] = 0;
      enc_picture->mv[LIST_0][img->block_x+i][img->block_y+j][1] = 0;
      enc_picture->mv[LIST_1][img->block_x+i][img->block_y+j][0] = 0;
      enc_picture->mv[LIST_1][img->block_x+i][img->block_y+j][1] = 0;
      enc_picture->ref_idx[LIST_0][img->block_x+i][img->block_y+j] = -1;
      enc_picture->ref_idx[LIST_1][img->block_x+i][img->block_y+j] = -1;
      enc_picture->ref_pic_id[LIST_0][img->block_x+i][img->block_y+j] = -1;
      enc_picture->ref_pic_id[LIST_1][img->block_x+i][img->block_y+j] = -1;
    }
    return;
  }

  if (!bslice)
  {
    for (j=j0; j<j1; j++)
    for (i=i0; i<i1; i++)
    {
      enc_picture->mv[LIST_0][img->block_x+i][img->block_y+j][0] = img->all_mv[i][j][LIST_0][fwref][mode][0];
      enc_picture->mv[LIST_0][img->block_x+i][img->block_y+j][1] = img->all_mv[i][j][LIST_0][fwref][mode][1];
      enc_picture->ref_idx[LIST_0][img->block_x+i][img->block_y+j] = fwref;
      enc_picture->ref_pic_id[LIST_0][img->block_x+i][img->block_y+j] = enc_picture->ref_pic_num[LIST_0+list_offset][enc_picture->ref_idx[LIST_0][img->block_x+i][img->block_y+j]];
    }
  }
  else
  {
    for (j=j0; j<j1; j++)
      for (i=i0; i<i1; i++)
      {
        if (mode==0)
        {
          pdir  =direct_pdir[img->block_x+i][img->block_y+j];
          fwref =direct_ref_idx[LIST_0][img->block_x+i][img->block_y+j];
          bwref =direct_ref_idx[LIST_1][img->block_x+i][img->block_y+j];
        }

        if ((pdir==0 || pdir==2))
        {
#if BI_PREDICTION
          if (currMB->bi_pred_me && (pdir == 2) && mode == 1)
          {
            short   ******bipred_mv = currMB->bi_pred_me == 1 ? img->bipred_mv1 : img->bipred_mv2;

              enc_picture->mv[LIST_0][img->block_x+i][img->block_y+j][0] = bipred_mv[i][j][LIST_0][0][mode][0];
              enc_picture->mv[LIST_0][img->block_x+i][img->block_y+j][1] = bipred_mv[i][j][LIST_0][0][mode][1];
              enc_picture->ref_idx[LIST_0][img->block_x+i][img->block_y+j] = 0;            
              enc_picture->ref_pic_id[LIST_0][img->block_x+i][img->block_y+j] = enc_picture->ref_pic_num[LIST_0+list_offset][0];
          }
          else
#endif
          {
            enc_picture->mv[LIST_0][img->block_x+i][img->block_y+j][0] = img->all_mv[i][j][LIST_0][fwref][mode][0];
            enc_picture->mv[LIST_0][img->block_x+i][img->block_y+j][1] = img->all_mv[i][j][LIST_0][fwref][mode][1];
            enc_picture->ref_idx[LIST_0][img->block_x+i][img->block_y+j] = fwref;
            enc_picture->ref_pic_id[LIST_0][img->block_x+i][img->block_y+j] = enc_picture->ref_pic_num[LIST_0+list_offset][enc_picture->ref_idx[LIST_0][img->block_x+i][img->block_y+j]];
          }
        }
        else
        {
          enc_picture->mv[LIST_0][img->block_x+i][img->block_y+j][0] = 0;
          enc_picture->mv[LIST_0][img->block_x+i][img->block_y+j][1] = 0;
          enc_picture->ref_idx[LIST_0][img->block_x+i][img->block_y+j] = -1;
          enc_picture->ref_pic_id[LIST_0][img->block_x+i][img->block_y+j] = -1;
        }

        if ((pdir==1 || pdir==2))
        {
#if BI_PREDICTION
          if (currMB->bi_pred_me && (pdir == 2) && mode == 1)
          {
            short   ******bipred_mv = currMB->bi_pred_me == 1 ? img->bipred_mv1 : img->bipred_mv2;
            
            enc_picture->mv[LIST_1][img->block_x+i][img->block_y+j][0] = bipred_mv[i][j][LIST_1][0][mode][0];
            enc_picture->mv[LIST_1][img->block_x+i][img->block_y+j][1] = bipred_mv[i][j][LIST_1][0][mode][1];
            enc_picture->ref_idx[LIST_1][img->block_x+i][img->block_y+j] = 0;            
            enc_picture->ref_pic_id[LIST_1][img->block_x+i][img->block_y+j] = enc_picture->ref_pic_num[LIST_1+list_offset][0];
          }
          else
#endif
          {
            enc_picture->mv[LIST_1][img->block_x+i][img->block_y+j][0] = img->all_mv[i][j][LIST_1][bwref][mode][0];
            enc_picture->mv[LIST_1][img->block_x+i][img->block_y+j][1] = img->all_mv[i][j][LIST_1][bwref][mode][1];
            enc_picture->ref_idx[LIST_1][img->block_x+i][img->block_y+j] = bwref;
            enc_picture->ref_pic_id[LIST_1][img->block_x+i][img->block_y+j] = enc_picture->ref_pic_num[LIST_1+list_offset][enc_picture->ref_idx[LIST_1][img->block_x+i][img->block_y+j]];
          }
        }
        else
        {
          enc_picture->mv[LIST_1][img->block_x+i][img->block_y+j][0] = 0;
          enc_picture->mv[LIST_1][img->block_x+i][img->block_y+j][1] = 0;
          enc_picture->ref_idx[LIST_1][img->block_x+i][img->block_y+j] = -1;
          enc_picture->ref_pic_id[LIST_1][img->block_x+i][img->block_y+j] = -1;
        }
      }
  }
}

/*!
 *************************************************************************************
 * \brief
 *    skip macroblock field inference
 * \return
 *    inferred field flag
 *************************************************************************************
 */
int field_flag_inference()
{
  int mb_field;
  Macroblock *currMB = &img->mb_data[img->current_mb_nr];

  if (currMB->mbAvailA)
  {
    mb_field = img->mb_data[currMB->mbAddrA].mb_field;
  }
  else
  {
    // check top macroblock pair
    if (currMB->mbAvailB)
    {
      mb_field = img->mb_data[currMB->mbAddrB].mb_field;
    }
    else
      mb_field = 0;
  }

  return mb_field;
}

/*!
 *************************************************************************************
 * \brief
 *    Store motion vectors for 8x8 partition
 *************************************************************************************
 */

void StoreMVBlock8x8(int dir, int block8x8, int mode, int ref, int bw_ref, int pdir8, int bframe)
{
  int i, j, i0, j0, ii, jj;
  short ******all_mv  = img->all_mv;
  short ******pred_mv = img->pred_mv;


  i0 = (block8x8%2)<<1;
  j0 = (block8x8/2)<<1;
  ii = i0+2;
  jj = j0+2;

  if (!bframe)
  {
    if (pdir8>=0) //(mode8!=IBLOCK)&&(mode8!=I16MB))  // && ref != -1)
    {
      for (j=j0; j<jj; j++)
        for (i=i0; i<ii; i++)
        {
          all_mv8x8[dir][LIST_0][i][j][0] = all_mv[i][j][LIST_0][ref][4][0];
          all_mv8x8[dir][LIST_0][i][j][1] = all_mv[i][j][LIST_0][ref][4][1];
          pred_mv8x8[dir][LIST_0][i][j][0] = pred_mv[i][j][LIST_0][ref][4][0];
          pred_mv8x8[dir][LIST_0][i][j][1] = pred_mv[i][j][LIST_0][ref][4][1];
        }
    }
  }
  else
  {
    if (pdir8==0) // forward
    {
      for (j=j0; j<jj; j++)
        for (i=i0; i<ii; i++)
        {
          all_mv8x8[dir][LIST_0][i][j][0] = all_mv[i][j][LIST_0][ref][mode][0];
          all_mv8x8[dir][LIST_0][i][j][1] = all_mv[i][j][LIST_0][ref][mode][1];
          pred_mv8x8[dir][LIST_0][i][j][0] = pred_mv[i][j][LIST_0][ref][mode][0];
          pred_mv8x8[dir][LIST_0][i][j][1] = pred_mv[i][j][LIST_0][ref][mode][1];
        }
    }
    else if (pdir8==1) // backward
    {
      for (j=j0; j<jj; j++)
        for (i=i0; i<ii; i++)
        {
          all_mv8x8[dir][LIST_1][i][j][0] = all_mv[i][j][LIST_1][bw_ref][mode][0];
          all_mv8x8[dir][LIST_1][i][j][1] = all_mv[i][j][LIST_1][bw_ref][mode][1];
          pred_mv8x8[dir][LIST_1][i][j][0] = pred_mv[i][j][LIST_1][bw_ref][mode][0];
          pred_mv8x8[dir][LIST_1][i][j][1] = pred_mv[i][j][LIST_1][bw_ref][mode][1];
        }
    }
    else if (pdir8==2) // bidir
    {
      for (j=j0; j<jj; j++)
        for (i=i0; i<ii; i++)
        {
          all_mv8x8[dir][LIST_0][i][j][0] = all_mv[i][j][LIST_0][ref][mode][0];
          all_mv8x8[dir][LIST_0][i][j][1] = all_mv[i][j][LIST_0][ref][mode][1];
          pred_mv8x8[dir][LIST_0][i][j][0] = pred_mv[i][j][LIST_0][ref][mode][0];
          pred_mv8x8[dir][LIST_0][i][j][1] = pred_mv[i][j][LIST_0][ref][mode][1];

          all_mv8x8[dir][LIST_1][i][j][0] = all_mv[i][j][LIST_1][bw_ref][mode][0];
          all_mv8x8[dir][LIST_1][i][j][1] = all_mv[i][j][LIST_1][bw_ref][mode][1];
          pred_mv8x8[dir][LIST_1][i][j][0] = pred_mv[i][j][LIST_1][bw_ref][mode][0];
          pred_mv8x8[dir][LIST_1][i][j][1] = pred_mv[i][j][LIST_1][bw_ref][mode][1];
        }
    }
    else
    {
      error("invalid direction mode", 255);
    }
  }
}



/*!
 *************************************************************************************
 * \brief
 *    Store motion vectors of 8x8 partitions of one macroblock
 *************************************************************************************
 */
void StoreMV8x8(int dir)
{
  int block8x8;

  int bframe = (img->type == B_SLICE);

  for (block8x8=0; block8x8<4; block8x8++)
    StoreMVBlock8x8(dir, block8x8, part8x8mode[block8x8],
                              part8x8fwref[block8x8], part8x8bwref[block8x8], part8x8pdir[block8x8], bframe);
}

/*!
*************************************************************************************
* \brief
*    Restore motion vectors for 8x8 partition
*************************************************************************************
*/
void RestoreMVBlock8x8(int dir, int block8x8, int mode, int ref, int bw_ref, int pdir8, int bframe)
{
  int i, j, i0, j0, ii, jj;
  short ******all_mv  = img->all_mv;
  short ******pred_mv = img->pred_mv;

  i0 = (block8x8%2)<<1;
  j0 = (block8x8/2)<<1;
  ii = i0+2;
  jj = j0+2;

  if (!bframe)
  {
    if (pdir8>=0) //(mode8!=IBLOCK)&&(mode8!=I16MB))  // && ref != -1)
    {
      for (j=j0; j<jj; j++)
        for (i=i0; i<ii; i++)
        {
          all_mv[i][j][LIST_0][ref][4][0]  = all_mv8x8[dir][LIST_0][i][j][0] ;
          all_mv[i][j][LIST_0][ref][4][1]  = all_mv8x8[dir][LIST_0][i][j][1] ;
          pred_mv[i][j][LIST_0][ref][4][0] = pred_mv8x8[dir][LIST_0][i][j][0];
          pred_mv[i][j][LIST_0][ref][4][1] = pred_mv8x8[dir][LIST_0][i][j][1];
        }
    }
  }
  else
  {
    if (pdir8==0) // forward
    {
      for (j=j0; j<jj; j++)
        for (i=i0; i<ii; i++)
        {
          all_mv[i][j][LIST_0][ref][mode][0]  = all_mv8x8[dir][LIST_0][i][j][0] ;
          all_mv[i][j][LIST_0][ref][mode][1]  = all_mv8x8[dir][LIST_0][i][j][1] ;
          pred_mv[i][j][LIST_0][ref][mode][0] = pred_mv8x8[dir][LIST_0][i][j][0];
          pred_mv[i][j][LIST_0][ref][mode][1] = pred_mv8x8[dir][LIST_0][i][j][1];
        }
    }
    else if (pdir8==1) // backward
    {
      for (j=j0; j<jj; j++)
        for (i=i0; i<ii; i++)
        {
          all_mv[i][j][LIST_1][bw_ref][mode][0]  = all_mv8x8[dir][LIST_1][i][j][0] ;
          all_mv[i][j][LIST_1][bw_ref][mode][1]  = all_mv8x8[dir][LIST_1][i][j][1] ;
          pred_mv[i][j][LIST_1][bw_ref][mode][0] = pred_mv8x8[dir][LIST_1][i][j][0];
          pred_mv[i][j][LIST_1][bw_ref][mode][1] = pred_mv8x8[dir][LIST_1][i][j][1];
        }
    }
    else if (pdir8==2) // bidir
    {
      for (j=j0; j<jj; j++)
        for (i=i0; i<ii; i++)
        {
          all_mv[i][j][LIST_0][ref][mode][0]  = all_mv8x8[dir][LIST_0][i][j][0] ;
          all_mv[i][j][LIST_0][ref][mode][1]  = all_mv8x8[dir][LIST_0][i][j][1] ;
          pred_mv[i][j][LIST_0][ref][mode][0] = pred_mv8x8[dir][LIST_0][i][j][0];
          pred_mv[i][j][LIST_0][ref][mode][1] = pred_mv8x8[dir][LIST_0][i][j][1];

          all_mv[i][j][LIST_1][bw_ref][mode][0]  = all_mv8x8[dir][LIST_1][i][j][0] ;
          all_mv[i][j][LIST_1][bw_ref][mode][1]  = all_mv8x8[dir][LIST_1][i][j][1] ;
          pred_mv[i][j][LIST_1][bw_ref][mode][0] = pred_mv8x8[dir][LIST_1][i][j][0];
          pred_mv[i][j][LIST_1][bw_ref][mode][1] = pred_mv8x8[dir][LIST_1][i][j][1];
        }
    }
    else
    {
      error("invalid direction mode", 255);
    }
  }
}



/*!
 *************************************************************************************
 * \brief
 *    Restore motion vectors of 8x8 partitions of one macroblock
 *************************************************************************************
 */
void RestoreMV8x8(int dir)
{
  int block8x8;

  int bframe = (img->type == B_SLICE);

  for (block8x8=0; block8x8<4; block8x8++)
    RestoreMVBlock8x8(dir, block8x8, part8x8mode[block8x8],
                                part8x8fwref[block8x8], part8x8bwref[block8x8], part8x8pdir[block8x8], bframe);
}


/*!
 *************************************************************************************
 * \brief
 *    Store predictors for 8x8 partition
 *************************************************************************************
 */

void StoreNewMotionVectorsBlock8x8(int dir, int block8x8, int mode, int fw_ref, int bw_ref, int pdir8, int bframe)
{
  int i, j, i0, j0, ii, jj;
  short ******all_mv  = img->all_mv;
  short ******pred_mv = img->pred_mv;


  i0 = (block8x8%2)<<1;
  j0 = (block8x8/2)<<1;
  ii = i0+2;
  jj = j0+2;

  if (pdir8<0)
  {
    for (j=j0; j<jj; j++)
      for (i=i0; i<ii; i++)
      {
        all_mv8x8[dir][LIST_0][i][j][0] = 0;
        all_mv8x8[dir][LIST_0][i][j][1] = 0;
        all_mv8x8[dir][LIST_1][i][j][0] = 0;
        all_mv8x8[dir][LIST_1][i][j][1] = 0;
      }
      return;
  }

  if (!bframe)
  {
    for (j=j0; j<jj; j++)
      for (i=i0; i<ii; i++)
      {
        all_mv8x8[dir][LIST_0][i][j][0] = all_mv[i][j][LIST_0][fw_ref][4][0];
        all_mv8x8[dir][LIST_0][i][j][1] = all_mv[i][j][LIST_0][fw_ref][4][1];
        pred_mv8x8[dir][LIST_0][i][j][0] = pred_mv[i][j][LIST_0][fw_ref][4][0];
        pred_mv8x8[dir][LIST_0][i][j][1] = pred_mv[i][j][LIST_0][fw_ref][4][1];
      }
  }
  else
  {
    if ((pdir8==0 || pdir8==2))
    {
      for (j=j0; j<jj; j++)
        for (i=i0; i<ii; i++)
        {
          all_mv8x8[dir][LIST_0][i][j][0] = all_mv[i][j][LIST_0][fw_ref][mode][0];
          all_mv8x8[dir][LIST_0][i][j][1] = all_mv[i][j][LIST_0][fw_ref][mode][1];
          pred_mv8x8[dir][LIST_0][i][j][0] = pred_mv[i][j][LIST_0][fw_ref][mode][0];
          pred_mv8x8[dir][LIST_0][i][j][1] = pred_mv[i][j][LIST_0][fw_ref][mode][1];
        }
    }
    else
    {
      for (j=j0; j<jj; j++)
        for (i=i0; i<ii; i++)
        {
          all_mv8x8[dir][LIST_0][i][j][0] = 0;
          all_mv8x8[dir][LIST_0][i][j][1] = 0;
        }
    }

    if ((pdir8==1 || pdir8==2))
    {
      for (j=j0; j<jj; j++)
        for (i=i0; i<ii; i++)
        {
          all_mv8x8[dir][LIST_1][i][j][0] = all_mv[i][j][LIST_1][bw_ref][mode][0];
          all_mv8x8[dir][LIST_1][i][j][1] = all_mv[i][j][LIST_1][bw_ref][mode][1];
          pred_mv8x8[dir][LIST_1][i][j][0] = pred_mv[i][j][LIST_1][bw_ref][mode][0];
          pred_mv8x8[dir][LIST_1][i][j][1] = pred_mv[i][j][LIST_1][bw_ref][mode][1];
        }
    }
    else
    {
      for (j=j0; j<jj; j++)
        for (i=i0; i<ii; i++)
        {
          all_mv8x8[dir][LIST_1][i][j][0] = 0;
          all_mv8x8[dir][LIST_1][i][j][1] = 0;
        }
    }
  }
}

/*!
 ************************************************************************
 * \brief
 *    Makes the decision if 8x8 tranform will be used (for RD-off)
 ************************************************************************
 */
int GetBestTransformP8x8()
{
  int    block_y, block_x, pic_pix_y, pic_pix_x, i, j, k;
  int    mb_y, mb_x, block8x8;
  int    cost8x8=0, cost4x4=0;
  int    diff4x4[64], *diff_ptr;
  int    diff8x8[64];

  if(input->AllowTransform8x8==2) //always allow 8x8 transform
    return 1;
  
  for (block8x8=0; block8x8<4; block8x8++)
  {
    mb_y = (block8x8 / 2) << 3;
    mb_x = (block8x8 % 2) << 3;
    //===== loop over 4x4 blocks =====
    k=0;
    for (block_y=mb_y; block_y<mb_y+8; block_y+=4)
    {
      pic_pix_y = img->opix_y + block_y;
      
      //get cost for transform size 4x4
      for (block_x=mb_x; block_x<mb_x+8; block_x+=4)
      {
        pic_pix_x = img->opix_x + block_x;

        //===== get displaced frame difference ======
        diff_ptr=&diff4x4[k];
        for (j=0; j<4; j++)
        for (i=0; i<4; i++, k++)
        {
          //4x4 transform size
          diff4x4[k] = imgY_org[pic_pix_y+j][pic_pix_x+i] - mpr8x8[j+block_y][i+block_x];
          //8x8 transform size
          diff8x8[k] = imgY_org[pic_pix_y+j][pic_pix_x+i] - mpr_8x8ts[j+block_y][i+block_x];
        }

        cost4x4 += SATD (diff_ptr, input->hadamard);
      }
    }

    cost8x8 += SATD8X8 (diff8x8, input->hadamard);
  }

  if(cost8x8<cost4x4)
    return 1;
  else
    return 0;
}

/*!
 *************************************************************************************
 * \brief
 *    Mode Decision for a macroblock
 *************************************************************************************
 */
 void encode_one_macroblock ()
 {
   static const int  b8_mode_table[6]  = {0, 4, 5, 6, 7};         // DO NOT CHANGE ORDER !!!
   static const int  mb_mode_table[8]  = {0, 1, 2, 3, P8x8, I16MB, I4MB, I8MB}; // DO NOT CHANGE ORDER !!!
   // Residue Color Transform
   static const int  mb_mode_table_RCT[11]  = {0, 1, 2, 3, P8x8, I16MB, I16MB, I16MB, I16MB, I4MB, I8MB};
   int max_index;

   int         valid[MAXMODE];
   int         rerun, block, index, mode, i0, i1, j0, j1, i, j, k, ctr16x16, dummy;
   short       pdir, best_pdir, ref;
   double      qp, lambda_mode, lambda_motion, min_rdcost, rdcost = 0, max_rdcost=1e30;
   int         lambda_motion_factor;
   int         fw_mcost, bw_mcost, bid_mcost, mcost, max_mcost=INT_MAX;
   int         cnt_nonz = 0, best_cnt_nonz = 0 ;
   short       best_fw_ref = 0;
   int64       curr_cbp_blk;
   int         cost=0;
   int         min_cost = max_mcost, min_cost8x8, cost8x8, cost_direct=0, have_direct=0, i16mode;
   int         intra1 = 0;
   int         temp_cpb = 0;
   int         best_transform_flag = 0;
   int         direct4x4_tmp, direct8x8_tmp, cost8x8_direct=0;

   int         intra       = (((img->type==P_SLICE||img->type==SP_SLICE) && img->mb_y==img->mb_y_upd && img->mb_y_upd!=img->mb_y_intra) || img->type==I_SLICE);
   int         spframe     = (img->type==SP_SLICE);
   int         siframe     = (img->type==SI_SLICE);
   int         bframe      = (img->type==B_SLICE);
   int         runs        = (input->RestrictRef==1 && input->rdopt==2 && (img->type==P_SLICE || img->type==SP_SLICE || (img->type==B_SLICE && img->nal_reference_idc>0)) ? 2 : 1);

   int         checkref    = (input->rdopt && input->RestrictRef && (img->type==P_SLICE || img->type==SP_SLICE));
   Macroblock* currMB      = &img->mb_data[img->current_mb_nr];
   Macroblock* prevMB      = img->current_mb_nr ? &img->mb_data[img->current_mb_nr-1]:NULL ;

   int     **ipredmodes = img->ipredmode;
   short   best_bw_ref = -1;
   short   ******allmvs = img->all_mv;
   int     ****i4p;  //for non-RD-opt. mode
   
#if BI_PREDICTION
   int bpd_mcost_l0,bpd_mcost_l1;
#endif


   int  l,list_offset;

#ifdef BEST_NZ_COEFF
   int best_nz_coeff[2][2];
#endif

   int curr_mb_field = ((img->MbaffFrameFlag)&&(currMB->mb_field));

   int tmp_8x8_flag, tmp_no_mbpart;
   
   // Residue Color Transform
   int residue_R, residue_G, residue_B, temp;
   int cr_cbp = 0;

   // find out the correct list offsets
   if (curr_mb_field)
   {
     if(img->current_mb_nr%2)
       list_offset = 4; // bottom field mb
     else
       list_offset = 2; // top field mb
   }
   else
   {
     list_offset = 0;  // no mb aff or frame mb
   }

   if(input->FMEnable)
     decide_intrabk_SAD();

   intra |= RandomIntra (img->current_mb_nr);    // Forced Pseudo-Random Intra

   //===== SET VALID MODES =====

   valid[I8MB]   = input->AllowTransform8x8;
   valid[I4MB]   = (input->AllowTransform8x8==2) ? 0:1;
   valid[I16MB]  = 1;

   valid[0]      = (!intra );
   valid[1]      = (!intra && input->InterSearch16x16);
   valid[2]      = (!intra && input->InterSearch16x8);
   valid[3]      = (!intra && input->InterSearch8x16);
   valid[4]      = (!intra && input->InterSearch8x8);
   valid[5]      = (!intra && input->InterSearch8x4);
   valid[6]      = (!intra && input->InterSearch4x8);
   valid[7]      = (!intra && input->InterSearch4x4);
   if (input->AllowTransform8x8==2) valid[5] = valid[6] = valid[7] = 0;  // tchen 6-10-04
   valid[P8x8]   = (valid[4] || valid[5] || valid[6] || valid[7]);
   valid[12]     = (siframe);

   if (!img->MbaffFrameFlag)
   {
     for (l=0+list_offset;l<(2+list_offset);l++)
     {
       for(k = 0; k < listXsize[l]; k++)
       {
         listX[l][k]->chroma_vector_adjustment= 0;
         if(img->structure == TOP_FIELD && img->structure != listX[l][k]->structure)
           listX[l][k]->chroma_vector_adjustment = -2;
         if(img->structure == BOTTOM_FIELD && img->structure != listX[l][k]->structure)
           listX[l][k]->chroma_vector_adjustment = 2;
       }
     }
   }
   else
   {
     if (curr_mb_field)
     {
       for (l=0+list_offset;l<(2+list_offset);l++)
       {
         for(k = 0; k < listXsize[l]; k++)
         {
           listX[l][k]->chroma_vector_adjustment= 0;
           if(img->current_mb_nr % 2 == 0 && listX[l][k]->structure == BOTTOM_FIELD)
             listX[l][k]->chroma_vector_adjustment = -2;
           if(img->current_mb_nr % 2 == 1 && listX[l][k]->structure == TOP_FIELD)
             listX[l][k]->chroma_vector_adjustment = 2;
         }
       }
     }
     else
     {
       for (l=0+list_offset;l<(2+list_offset);l++)
       {
         for(k = 0; k < listXsize[l]; k++)
         {
           listX[l][k]->chroma_vector_adjustment= 0;
         }
       }
     }

   }

   //===== SET LAGRANGE PARAMETERS =====

   if (input->rdopt)
   {
     qp = (double)img->qp - SHIFT_QP;

     if (input->UseExplicitLambdaParams)
     {
       if (img->type==B_SLICE && img->nal_reference_idc)
         lambda_mode   = input->LambdaWeight[5] * pow (2, img->bitdepth_lambda_scale + qp/3.0) ;       
       else
         lambda_mode   = input->LambdaWeight[img->type] * pow (2, img->bitdepth_lambda_scale + qp/3.0) ;       
     }
     else
     {
       double lambda_scale = (img->type == B_SLICE && img->nal_reference_idc == 0 ? 1.0: 1.0 - 0.05 * (double) input->jumpd);

       if (input->successive_Bframe>0)
         lambda_mode   = 0.68 * pow (2, img->bitdepth_lambda_scale + qp/3.0) * (bframe ? max(2.00,min(4.00,(qp / 6.0))):spframe?max(1.4,min(3.0,(qp / 12.0))):1.0);
       else
         lambda_mode   = 0.85 * pow (2, img->bitdepth_lambda_scale + qp/3.0) * (bframe ? 4.0:spframe?max(1.4,min(3.0,(qp / 12.0))):1.0);
       
       if (input->PyramidCoding == 2)
       {
         if (img->type==B_SLICE && img->nal_reference_idc)
           lambda_mode *= (1.0 - min(0.4,0.20 * (double) gop_structure[img->b_frame_to_code-1].pyramid_layer)) ;
       }
       else
       {
         if (img->type==B_SLICE && img->nal_reference_idc)
           lambda_mode *= 0.80;
       }
       
       lambda_mode *=lambda_scale;

     }
   if (input->hadamardqpel)
   {
     lambda_mode *= 0.95;
   }
     
     lambda_motion = sqrt (lambda_mode);
   }
   else
   {
     lambda_mode = lambda_motion = QP2QUANT[max(0,img->qp-SHIFT_QP)];
   }

   lambda_motion_factor = LAMBDA_FACTOR (lambda_motion);

#if BI_PREDICTION
   for (mode=0; mode<MAXMODE; mode++)
   {
     img->bi_pred_me[mode]=0;
   }
#endif


   for (rerun=0; rerun<runs; rerun++)
   {
     if (runs==2)
     {
       if (rerun==0)   input->rdopt=1;
       else            input->rdopt=2;
     }

     // reset chroma intra predictor to default
     currMB->c_ipred_mode = DC_PRED_8;

     if (!intra)
     {
       //===== set direct motion vectors =====
       if (bframe)
       {
         Get_Direct_Motion_Vectors ();
       }

       //===== MOTION ESTIMATION FOR 16x16, 16x8, 8x16 BLOCKS =====
       for (min_cost=INT_MAX, best_mode=1, mode=1; mode<4; mode++)
       {
#if BI_PREDICTION
         bi_pred_me = 0;
         img->bi_pred_me[mode]=0;
#endif

         if (valid[mode])
         {
           for (cost=0, block=0; block<(mode==1?1:2); block++)
           {
             PartitionMotionSearch (mode, block, lambda_motion);

             //--- set 4x4 block indizes (for getting MV) ---
             j = (block==1 && mode==2 ? 2 : 0);
             i = (block==1 && mode==3 ? 2 : 0);

             //--- get cost and reference frame for forward prediction ---
             for (fw_mcost=max_mcost, ref=0; ref<listXsize[LIST_0+list_offset]; ref++)
             {
               if (!checkref || ref==0 || CheckReliabilityOfRef (block, LIST_0, ref, mode))
               {
                 mcost  = (input->rdopt ? REF_COST (lambda_motion_factor, ref, LIST_0 + list_offset) : (int)(2*lambda_motion*min(ref,1)));

                 mcost += motion_cost[mode][LIST_0][ref][block];
                 if (mcost < fw_mcost)
                 {
                   fw_mcost    = mcost;
                   best_fw_ref = ref;
                 }
               }
             }

             if (bframe)
             {
               //--- get cost for bidirectional prediction ---
               for (bw_mcost=max_mcost, ref=0; ref<listXsize[LIST_1 + list_offset]; ref++)
               {
                 mcost  = (input->rdopt ? REF_COST (lambda_motion_factor, ref, LIST_1 + list_offset) : (int)(2*lambda_motion*min(ref,1)));
                 mcost += motion_cost[mode][LIST_1][ref][block];
                 if (mcost < bw_mcost)
                 {
                   bw_mcost    = mcost;
                   best_bw_ref = ref;
                 }
               }

               // search bidirectional between best forward and ref_idx=0 backward
               bid_mcost  = (input->rdopt ? (REF_COST (lambda_motion_factor, best_fw_ref,LIST_0+list_offset)+REF_COST (lambda_motion_factor, 0,LIST_1+list_offset)) : (int)(2*lambda_motion*min(best_fw_ref,1)));
               bid_mcost += BIDPartitionCost (mode, block, best_fw_ref, 0, lambda_motion_factor);

#if BI_PREDICTION 
              if (mode == 1 && img->type==B_SLICE && input->BiPredMotionEstimation )
              {                
                // search bidirectional between ref_idx=0 forward and ref_idx=0 backward
                bpd_mcost_l0  = (input->rdopt ? (REF_COST (lambda_motion_factor, 0,LIST_0+list_offset)+REF_COST (lambda_motion_factor, 0,LIST_1+list_offset)) : (int)(2*lambda_motion*min(0,1)));
                bpd_mcost_l0 += BPredPartitionCost(mode, block, 0, 0, lambda_motion_factor,0);
                bpd_mcost_l1  = (input->rdopt ? (REF_COST (lambda_motion_factor, 0,LIST_0+list_offset)+REF_COST (lambda_motion_factor, 0,LIST_1+list_offset)) : (int)(2*lambda_motion*min(0,1)));
                bpd_mcost_l1 += BPredPartitionCost(mode, block, 0, 0, lambda_motion_factor,1);                
              }
              else
              {
                bpd_mcost_l0  = INT_MAX;
                bpd_mcost_l1  = INT_MAX;
              }
              
#endif

#if BI_PREDICTION 
              if (input->BiPredMotionEstimation && mode==1)
              {
                //--- get prediction direction ----
                if (fw_mcost<=bw_mcost && fw_mcost<=bid_mcost && fw_mcost<bpd_mcost_l0 && fw_mcost<bpd_mcost_l1)
                {
                  best_pdir = 0;
                  best_bw_ref = 0;
                  cost += fw_mcost;
                }
                else if (bw_mcost<=fw_mcost && bw_mcost<=bid_mcost && bw_mcost<bpd_mcost_l0 && bw_mcost<bpd_mcost_l1)
                {
                  best_pdir = 1;
                  cost += bw_mcost;
                  best_fw_ref = 0;
                }
                else if (bid_mcost<=fw_mcost && bid_mcost<=bw_mcost && bid_mcost<bpd_mcost_l0 && bid_mcost<bpd_mcost_l1)
                {
                  best_pdir = 2;
                  cost += bid_mcost;
                  bi_pred_me = 0;
                  img->bi_pred_me[mode]=0;
                  best_bw_ref = 0;  
                }
                else if (bpd_mcost_l0<=fw_mcost && bpd_mcost_l0<=bw_mcost && bpd_mcost_l0<=bid_mcost&& bpd_mcost_l0<=bpd_mcost_l1)
                {
                  best_pdir = 2;
                  best_bw_ref = 0;
                  best_fw_ref = 0;
                  cost += bpd_mcost_l0;                  
                  bi_pred_me = 1;
                  img->bi_pred_me[mode]=1;
                }
                else
                {
                  best_bw_ref = 0;
                  best_fw_ref = 0;
                  best_pdir = 2;
                  cost += bpd_mcost_l1;                                  
                  bi_pred_me = 2;
                  img->bi_pred_me[mode]=2;
                }               
              }
              else
#endif
              {
                //--- get prediction direction ----
               if (fw_mcost<=bw_mcost && fw_mcost<=bid_mcost)
               {
                 best_pdir = 0;
                 best_bw_ref = 0;
                 cost += fw_mcost;
               }
               else if (bw_mcost<=fw_mcost && bw_mcost<=bid_mcost)
               {
                 best_pdir = 1;
                 cost += bw_mcost;
                 best_fw_ref = 0;
               }
               else
               {
                 best_pdir = 2;
                 cost += bid_mcost;
                 best_bw_ref = 0;
               }
              }                          
             }
             else // if (bframe)
             {
               best_pdir  = 0;
               cost      += fw_mcost;
             }

             if (mode==1)
             {
               if (best_pdir==1)
               {
                 for (j=0; j<4; j++)
                 {
                   for (i=0; i<4; i++)
                   {
                     enc_picture->ref_idx[LIST_0][img->block_x+(block&1)*2+i][img->block_y+(block&2)+j] = -1;
                     enc_picture->ref_pic_id [LIST_0][img->block_x+(block&1)*2+i][img->block_y+(block&2)+j] = -1;
                     enc_picture->mv[LIST_0][img->block_x+(block&1)*2+i][img->block_y+(block&2)+j][0] = 0;
                     enc_picture->mv[LIST_0][img->block_x+(block&1)*2+i][img->block_y+(block&2)+j][1] = 0;
                   }
                 }
               }
               else
               {
                 for (j=0; j<4; j++)
                 {
                   for (i=0; i<4; i++)
                   {
#if BI_PREDICTION
                     if (img->bi_pred_me[mode])
                     {
                       short   ******bipred_mv = img->bi_pred_me[mode] == 1 ? img->bipred_mv1 : img->bipred_mv2;
                       
                       enc_picture->ref_idx[LIST_0][img->block_x+(block&1)*2+i][img->block_y+(block&2)+j] = 0;
                       enc_picture->ref_pic_id [LIST_0][img->block_x+(block&1)*2+i][img->block_y+(block&2)+j] = enc_picture->ref_pic_num[LIST_0 + list_offset][0];  
                       enc_picture->mv[LIST_0][img->block_x+(block&1)*2+i][img->block_y+(block&2)+j][0] = bipred_mv[i][j][LIST_0][0][mode][0];
                       enc_picture->mv[LIST_0][img->block_x+(block&1)*2+i][img->block_y+(block&2)+j][1] = bipred_mv[i][j][LIST_0][0][mode][1];
                     }
#endif
                     else 
                     {
                       enc_picture->ref_idx[LIST_0][img->block_x+(block&1)*2+i][img->block_y+(block&2)+j] = best_fw_ref;
                       enc_picture->ref_pic_id [LIST_0][img->block_x+(block&1)*2+i][img->block_y+(block&2)+j] = enc_picture->ref_pic_num[LIST_0 + list_offset][enc_picture->ref_idx[LIST_0][img->block_x+(block&1)*2+i][img->block_y+(block&2)+j]];  
                       enc_picture->mv[LIST_0][img->block_x+(block&1)*2+i][img->block_y+(block&2)+j][0] = img->all_mv[i][j][LIST_0][best_fw_ref][mode][0];
                       enc_picture->mv[LIST_0][img->block_x+(block&1)*2+i][img->block_y+(block&2)+j][1] = img->all_mv[i][j][LIST_0][best_fw_ref][mode][1];
                     }
                     
                   }
                 }
               }
               
               if (bframe)
               {
                 if (best_pdir==0)
                 {
                   for (j=0; j<4; j++)
                   {
                     for (i=0; i<4; i++)
                     {
                       enc_picture->ref_idx[LIST_1][img->block_x+(block&1)*2+i][img->block_y+(block&2)+j] = -1;
                       enc_picture->ref_pic_id [LIST_1][img->block_x+(block&1)*2+i][img->block_y+(block&2)+j] = -1;
                       enc_picture->mv[LIST_1][img->block_x+(block&1)*2+i][img->block_y+(block&2)+j][0] = 0;
                       enc_picture->mv[LIST_1][img->block_x+(block&1)*2+i][img->block_y+(block&2)+j][1] = 0;
                     }
                   }
                 }
                 else
                 {
                   for (j=0; j<4; j++)
                   {
                     for (i=0; i<4; i++)
                     {                     
#if BI_PREDICTION
                       if (img->bi_pred_me[mode])
                       {
                         short   ******bipred_mv = img->bi_pred_me[mode] == 1 ? img->bipred_mv1 : img->bipred_mv2;
                         
                         enc_picture->ref_idx[LIST_1][img->block_x+(block&1)*2+i][img->block_y+(block&2)+j] = 0;
                         enc_picture->ref_pic_id [LIST_1][img->block_x+(block&1)*2+i][img->block_y+(block&2)+j] = enc_picture->ref_pic_num[LIST_1 + list_offset][0];
                         enc_picture->mv[LIST_1][img->block_x+(block&1)*2+i][img->block_y+(block&2)+j][0] = bipred_mv[i][j][LIST_1][0][mode][0];
                         enc_picture->mv[LIST_1][img->block_x+(block&1)*2+i][img->block_y+(block&2)+j][1] = bipred_mv[i][j][LIST_1][0][mode][1];
                       }
                       else 
#endif
                       {
                         enc_picture->ref_idx[LIST_1][img->block_x+(block&1)*2+i][img->block_y+(block&2)+j] = best_bw_ref;
                         enc_picture->ref_pic_id [LIST_1][img->block_x+(block&1)*2+i][img->block_y+(block&2)+j] = enc_picture->ref_pic_num[LIST_1 + list_offset][enc_picture->ref_idx[LIST_1][img->block_x+(block&1)*2+i][img->block_y+(block&2)+j]];
                         if(best_bw_ref>=0)
                         {
                           enc_picture->mv[LIST_1][img->block_x+(block&1)*2+i][img->block_y+(block&2)+j][0] = img->all_mv[i][j][LIST_1][best_bw_ref][mode][0];
                           enc_picture->mv[LIST_1][img->block_x+(block&1)*2+i][img->block_y+(block&2)+j][1] = img->all_mv[i][j][LIST_1][best_bw_ref][mode][1];
                         }
                       }
                       
                     }
                   }
                 }
               }
             }
             else if (mode==2)
             {
               int blocky_plus_block2 = img->block_y+block*2;
               for (j=0; j<2; j++)
               {
                 for (i=0; i<4; i++)
                 {
                   if (best_pdir==1)
                   {
                    enc_picture->ref_idx[LIST_0][img->block_x+i][blocky_plus_block2+j] = -1;
                    enc_picture->ref_pic_id [LIST_0][img->block_x+i][blocky_plus_block2+j] = -1;
                    enc_picture->mv[LIST_0][img->block_x+i][blocky_plus_block2+j][0] = 0;
                    enc_picture->mv[LIST_0][img->block_x+i][blocky_plus_block2+j][1] = 0;
                   }
                   else
                   {                     
                     enc_picture->ref_idx[LIST_0][img->block_x+i][blocky_plus_block2+j] = best_fw_ref;
                     enc_picture->ref_pic_id [LIST_0][img->block_x+i][blocky_plus_block2+j] = enc_picture->ref_pic_num[LIST_0 + list_offset][enc_picture->ref_idx[LIST_0][img->block_x+i][blocky_plus_block2+j]];
                     enc_picture->mv[LIST_0][img->block_x+i][blocky_plus_block2+j][0] = img->all_mv[i][j+block*2][LIST_0][best_fw_ref][mode][0];
                     enc_picture->mv[LIST_0][img->block_x+i][blocky_plus_block2+j][1] = img->all_mv[i][j+block*2][LIST_0][best_fw_ref][mode][1];                    
                   }

                   if (bframe)
                   {
                     if (best_pdir==0)
                     {
                       enc_picture->ref_idx[LIST_1][img->block_x+i][blocky_plus_block2+j] = -1;
                       enc_picture->ref_pic_id [LIST_1][img->block_x+i][blocky_plus_block2+j] = -1;
                       enc_picture->mv[LIST_1][img->block_x+i][blocky_plus_block2+j][0] = 0;
                       enc_picture->mv[LIST_1][img->block_x+i][blocky_plus_block2+j][1] = 0;
                     }
                     else
                     {
                       enc_picture->ref_idx[LIST_1][img->block_x+i][blocky_plus_block2+j] = best_bw_ref;
                       enc_picture->ref_pic_id [LIST_1][img->block_x+i][blocky_plus_block2+j] = enc_picture->ref_pic_num[LIST_1 + list_offset][enc_picture->ref_idx[LIST_1][img->block_x+i][blocky_plus_block2+j]];
                       if(best_bw_ref>=0)
                       {
#ifndef KS_MV
                         enc_picture->mv[LIST_1][img->block_x+i][blocky_plus_block2+j][0] = img->all_mv[i][j][LIST_1][best_bw_ref][mode][0];
                         enc_picture->mv[LIST_1][img->block_x+i][blocky_plus_block2+j][1] = img->all_mv[i][j][LIST_1][best_bw_ref][mode][1];
#else
                         enc_picture->mv[LIST_1][img->block_x+i][blocky_plus_block2+j][0] = img->all_mv[i][block*2+j][LIST_1][best_bw_ref][mode][0];
                         enc_picture->mv[LIST_1][img->block_x+i][blocky_plus_block2+j][1] = img->all_mv[i][block*2+j][LIST_1][best_bw_ref][mode][1];
#endif                         
                       }                       
                     }
                   }
                 }
               }
             }
             else
             {
               int blockx_plus_block2 = img->block_x+block*2;
               for (j=0; j<4; j++)
               {
                 for (i=0; i<2; i++)
                 {
                   if (best_pdir==1)
                   {
                     enc_picture->ref_idx[LIST_0][blockx_plus_block2+i][img->block_y+j] = -1;
                     enc_picture->ref_pic_id [LIST_0][blockx_plus_block2+i][img->block_y+j] = -1;
                     enc_picture->mv[LIST_0][blockx_plus_block2+i][img->block_y+j][0] = 0;
                     enc_picture->mv[LIST_0][blockx_plus_block2+i][img->block_y+j][1] = 0;
                   }
                   else
                   {
                     enc_picture->ref_idx[LIST_0][blockx_plus_block2+i][img->block_y+j] = best_fw_ref;
                     enc_picture->ref_pic_id [LIST_0][blockx_plus_block2+i][img->block_y+j] = enc_picture->ref_pic_num[LIST_0 + list_offset][enc_picture->ref_idx[LIST_0][blockx_plus_block2+i][img->block_y+j]];
#ifndef KS_MV
                     enc_picture->mv[LIST_0][blockx_plus_block2+i][img->block_y+j][0] = img->all_mv[i][j][LIST_0][best_fw_ref][mode][0];
                     enc_picture->mv[LIST_0][blockx_plus_block2+i][img->block_y+j][1] = img->all_mv[i][j][LIST_0][best_fw_ref][mode][1];
#else
                     enc_picture->mv[LIST_0][blockx_plus_block2+i][img->block_y+j][0] = img->all_mv[block*2+i][j][LIST_0][best_fw_ref][mode][0];
                     enc_picture->mv[LIST_0][blockx_plus_block2+i][img->block_y+j][1] = img->all_mv[block*2+i][j][LIST_0][best_fw_ref][mode][1];
#endif
                   }

                   if (bframe)
                   {
                     if (best_pdir==0)
                     {
                       enc_picture->ref_idx[LIST_1][blockx_plus_block2+i][img->block_y+j] = -1;
                       enc_picture->ref_pic_id [LIST_1][blockx_plus_block2+i][img->block_y+j] = -1;
                       enc_picture->mv[LIST_1][blockx_plus_block2+i][img->block_y+j][0] = 0;
                       enc_picture->mv[LIST_1][blockx_plus_block2+i][img->block_y+j][1] = 0;
                     }
                     else
                     {
                       enc_picture->ref_idx[LIST_1][blockx_plus_block2+i][img->block_y+j] = best_bw_ref;
                       enc_picture->ref_pic_id [LIST_1][blockx_plus_block2+i][img->block_y+j] = enc_picture->ref_pic_num[LIST_1 + list_offset][enc_picture->ref_idx[LIST_1][blockx_plus_block2+i][img->block_y+j]];
                       if(best_bw_ref>=0)
                       {
#ifndef KS_MV
                         enc_picture->mv[LIST_1][blockx_plus_block2+i][img->block_y+j][0] = img->all_mv[i][j][LIST_1][best_bw_ref][mode][0];
                         enc_picture->mv[LIST_1][blockx_plus_block2+i][img->block_y+j][1] = img->all_mv[i][j][LIST_1][best_bw_ref][mode][1];
#else
                         enc_picture->mv[LIST_1][blockx_plus_block2+i][img->block_y+j][0] = img->all_mv[block*2+i][j][LIST_1][best_bw_ref][mode][0];
                         enc_picture->mv[LIST_1][blockx_plus_block2+i][img->block_y+j][1] = img->all_mv[block*2+i][j][LIST_1][best_bw_ref][mode][1];
#endif
                       }
                     }
                   }
                 }
               }
             }

             //----- set reference frame and direction parameters -----
             if (mode==3)
             {
               best8x8fwref [3][block  ] = best8x8fwref [3][  block+2] = best_fw_ref;
               best8x8pdir  [3][block  ] = best8x8pdir  [3][  block+2] = best_pdir;
               best8x8bwref [3][block  ] = best8x8bwref [3][  block+2] = best_bw_ref;
             }
             else if (mode==2)
             {
               best8x8fwref [2][2*block] = best8x8fwref [2][2*block+1] = best_fw_ref;
               best8x8pdir  [2][2*block] = best8x8pdir  [2][2*block+1] = best_pdir;
               best8x8bwref [2][2*block] = best8x8bwref [2][2*block+1] = best_bw_ref;
             }
             else
             {
               best8x8fwref [1][0] = best8x8fwref [1][1] = best8x8fwref [1][2] = best8x8fwref [1][3] = best_fw_ref;
               best8x8pdir  [1][0] = best8x8pdir  [1][1] = best8x8pdir  [1][2] = best8x8pdir  [1][3] = best_pdir;
               best8x8bwref [1][0] = best8x8bwref [1][1] = best8x8bwref [1][2] = best8x8bwref [1][3] = best_bw_ref;
             }

             //--- set reference frames and motion vectors ---
             if (mode>1 && block==0)
               SetRefAndMotionVectors (block, mode, best_pdir, best_fw_ref, best_bw_ref);

          } // for (block=0; block<(mode==1?1:2); block++)

          if(!input->rdopt)
          {
            currMB->luma_transform_size_8x8_flag = 0;
            if (input->AllowTransform8x8) //for inter rd-off, set 8x8 to do 8x8 transform
            {
              SetModesAndRefframeForBlocks(mode);
              currMB->luma_transform_size_8x8_flag = TransformDecision(-1, &cost);
            }
          }

          if (cost < min_cost)
          {
            best_mode = mode;
            min_cost  = cost;
            best_transform_flag = currMB->luma_transform_size_8x8_flag;
          }
        } // if (valid[mode])
      } // for (mode=1; mode<4; mode++)

      if (valid[P8x8])
      {
        cost8x8 = 0;
        cost8_8x8ts = INT_MAX;

        //===== store coding state of macroblock =====
        store_coding_state (cs_mb);

        currMB->all_blk_8x8 = -1;

        if (input->AllowTransform8x8)
        {
          //===========================================================
          // Check 8x8 partition with transform size 8x8 
          //===========================================================
          //=====  LOOP OVER 8x8 SUB-PARTITIONS  (Motion Estimation & Mode Decision) =====
          for (cost_direct=cbp8x8=cbp_blk8x8=cnt_nonz_8x8=0, block=0; block<4; block++)
          {
            //--- set coordinates ---
            j0 = ((block/2)<<3);    j1 = (j0>>2);
            i0 = ((block%2)<<3);    i1 = (i0>>2);

#ifdef BEST_NZ_COEFF
            {
              int x, y;
              for(y = 0; y <= 1; y++)
                for(x = 0; x <= 1; x++)
                  best_nz_coeff[x][y]= img->nz_coeff[img->current_mb_nr][i1 + x][j1 + y] = 0;
            }
#endif
            currMB->luma_transform_size_8x8_flag = 1; //switch to transform size 8x8

            //=====  LOOP OVER POSSIBLE CODING MODES FOR 8x8 SUB-PARTITION  =====
            for (min_cost8x8=INT_MAX,min_rdcost=1e30, index=(bframe?0:1); index<2; index++)
            {
              mode = b8_mode_table[index]; 
              if ((valid[mode] && mode != 0) || (mode == 0 && active_sps->direct_8x8_inference_flag))
              {
                curr_cbp_blk = 0;
                
                if (mode==0)
                {
                  //--- Direct Mode ---
                  if (!input->rdopt)
                  {
                    // T.Nishi(MEI ACC) 04-28-2004
                    direct4x4_tmp=0; direct8x8_tmp=0;
                    direct4x4_tmp = Get_Direct_Cost8x8 ( block, &direct8x8_tmp);
                    if ((direct4x4_tmp==INT_MAX)||(cost_direct==INT_MAX))
                    {
                      cost_direct = (INT_MAX);
                      cost8x8_direct = (INT_MAX);
                    }
                    else
                    {
                      cost_direct += direct4x4_tmp;
                      cost8x8_direct += direct8x8_tmp;
                    }

                    switch(input->AllowTransform8x8)
                    {
                    case 1: // Mixture of 8x8 & 4x4 transform
                      if((direct8x8_tmp < direct4x4_tmp) ||
                        !(valid[5] && valid[6] && valid[7]))
                        cost = direct8x8_tmp;
                      else
                        cost = direct4x4_tmp;
                      break;
                    case 2: // 8x8 Transform only
                      cost = direct8x8_tmp;
                      break;
                    default: // 4x4 Transform only
                      cost = direct4x4_tmp;
                      break;
                    }
                    // T.Nishi(MEI ACC) 04-28-2004 end

                    if (input->AllowTransform8x8==2) cost = min_cost8x8 + 1;  // tchen 6-10-04: disable direct_8x8 mode that allows 4x4 dct only

                    have_direct ++;
                  }

                  best_fw_ref = direct_ref_idx[LIST_0][img->block_x+(block&1)*2][img->block_y+(block&2)];
                  best_bw_ref = direct_ref_idx[LIST_1][img->block_x+(block&1)*2][img->block_y+(block&2)];
                  best_pdir   = direct_pdir[img->block_x+(block&1)*2][img->block_y+(block&2)];
                } // if (mode==0)
                else
                {

                  //======= motion estimation for all reference frames ========
                  //-----------------------------------------------------------
                  PartitionMotionSearch (mode, block, lambda_motion);

                  //--- get cost and reference frame for forward prediction ---
                  for (fw_mcost=max_mcost, ref=0; ref<listXsize[LIST_0+list_offset]; ref++)
                  {
                    if (!checkref || ref==0 || CheckReliabilityOfRef (block, LIST_0, ref, mode))
                    {
                      mcost  = (input->rdopt ? REF_COST(lambda_motion_factor,ref,LIST_0+list_offset) : (int)(2*lambda_motion*min(ref,1)));

                      mcost += motion_cost[mode][LIST_0][ref][block];
                      if (mcost < fw_mcost)
                      {
                        fw_mcost    = mcost;
                        best_fw_ref = ref;
                      }
                    }
                  }

                  //store forward reference index for every block
                  for (j=0; j<2; j++)
                    for (i=0; i<2; i++)
                    {
                      enc_picture->ref_idx[LIST_0][img->block_x+(block&1)*2+i][img->block_y+(block&2)+j] = best_fw_ref;
                      enc_picture->ref_pic_id [LIST_0][img->block_x+(block&1)*2+i][img->block_y+(block&2)+j] = enc_picture->ref_pic_num[LIST_0 + list_offset][enc_picture->ref_idx[LIST_0][img->block_x+(block&1)*2+i][img->block_y+(block&2)+j]];
                    }

                    if (bframe)
                    {
                      for (bw_mcost=max_mcost, ref=0; ref<listXsize[LIST_1+list_offset]; ref++)
                      {
                        mcost  = (input->rdopt ? REF_COST(lambda_motion_factor,ref,LIST_1+list_offset) : (int)(2*lambda_motion*min(ref,1)));
                        
                        mcost += motion_cost[mode][LIST_1][ref][block];
                        if (mcost < bw_mcost)
                        {
                          bw_mcost    = mcost;
                          best_bw_ref = ref;
                        }
                      }

                      // bidirectional uses best forward and zero backward reference
                      bid_mcost  = (input->rdopt ? (REF_COST (lambda_motion_factor, best_fw_ref, LIST_0 + list_offset)+REF_COST (lambda_motion_factor, 0, LIST_1 + list_offset)) : (int)(2*lambda_motion*min(best_fw_ref,1)));
                      bid_mcost += BIDPartitionCost (mode, block, best_fw_ref, 0, lambda_motion_factor );

                      //--- get prediction direction ----
                      if      (fw_mcost<=bw_mcost && fw_mcost<=bid_mcost)
                      {
                        best_pdir = 0;
                        cost = fw_mcost;
                        best_bw_ref = -1;
                      }
                      else if (bw_mcost<=fw_mcost && bw_mcost<=bid_mcost)
                      {
                        best_pdir = 1;
                        cost = bw_mcost;
                        best_fw_ref = -1;
                      }
                      else
                      {
                        best_pdir = 2;
                        cost = bid_mcost;
                        best_bw_ref = 0;
                      }
                      //store backward reference index for every block
                      for (j=0; j<2; j++)
                        for (i=0; i<2; i++)
                        {
                          enc_picture->ref_idx[LIST_0][img->block_x+(block&1)*2+i][img->block_y+(block&2)+j] = best_fw_ref;
                          enc_picture->ref_idx[LIST_1][img->block_x+(block&1)*2+i][img->block_y+(block&2)+j] = best_bw_ref;
                          //enc_picture->ref_pic_id [LIST_1][img->block_x+(block&1)*2+i][img->block_y+(block&2)+j] = enc_picture->ref_pic_num[LIST_1 + list_offset][enc_picture->ref_idx[LIST_1][img->block_x+(block&1)*2+i][img->block_y+(block&2)+j]];
                        }
                    } // if (bframe)
                    else
                    {
                      best_pdir = 0;
                      cost      = fw_mcost;
                    }
                } // if (mode!=0)

                //--- store coding state before coding with current mode ---
                store_coding_state (cs_cm);
                
                if (input->rdopt)
                {
                  //--- get and check rate-distortion cost ---
                  rdcost = RDCost_for_8x8blocks (&cnt_nonz, &curr_cbp_blk, lambda_mode,
                    block, mode, best_pdir, best_fw_ref, best_bw_ref);
                }
                else
                {
                  if (cost!=INT_MAX)
                    cost += (REF_COST (lambda_motion_factor, B8Mode2Value (mode, best_pdir), list_offset + (best_pdir<1?0:1)) - 1);
                }
                
                
                //--- set variables if best mode has changed ---
                if (( input->rdopt && rdcost < min_rdcost) ||
                  (!input->rdopt && cost < min_cost8x8  )   )
                {
                  min_cost8x8          = cost;
                  min_rdcost           = rdcost;
                  part8x8mode  [block] = mode;
                  part8x8pdir  [block] = best_pdir;
                  part8x8fwref [block] = best_fw_ref;
                  part8x8bwref [block] = best_bw_ref;

#ifdef BEST_NZ_COEFF
                  {
                    int x, y;
                    for(y = 0; y <= 1; y++)
                      for(x = 0; x <= 1; x++)
                        best_nz_coeff[x][y]= cnt_nonz ? img->nz_coeff[img->current_mb_nr][i1 + x][j1 + y] : 0;
                  }
#endif

                                    
                  //--- store number of nonzero coefficients ---
                  best_cnt_nonz  = cnt_nonz;
                  
                  if (input->rdopt)
                  {
                    //--- store block cbp ---
                    cbp_blk8x8    &= (~(0x33 << (((block>>1)<<3)+((block%2)<<1)))); // delete bits for block
                    cbp_blk8x8    |= curr_cbp_blk;
                    
                    //--- store coefficients ---
                    for (k=0; k< 4; k++)
                      for (j=0; j< 2; j++)
                        for (i=0; i<65; i++)  cofAC_8x8ts[block][k][j][i] = img->cofAC[block][k][j][i]; // 18->65 for ABT
                        
                    //--- store reconstruction & prediction ---
                    for (j=j0; j<j0+8; j++)
                      for (i=i0; i<i0+8; i++)
                      {
                        // Residue Color Transform
                        if(!img->residue_transform_flag)
                        {
                          rec_mbY_8x8ts[j][i] = enc_picture->imgY[img->pix_y+j][img->pix_x+i];
                          mpr_8x8ts   [j][i] = img->mpr[i][j];
                        } 
                        else 
                        {
                          rec_resG_8x8ts[i][j] = rec_resG[i][j];
                          resTrans_R_8x8ts[i][j] = resTrans_R[i][j];
                          resTrans_B_8x8ts[i][j] = resTrans_B[i][j];
                          mprRGB_8x8ts[0][i][j] = mprRGB[0][i][j];
                          mprRGB_8x8ts[1][i][j] = mprRGB[1][i][j];
                          mprRGB_8x8ts[2][i][j] = mprRGB[2][i][j];
                        }
                      }
                  }

                  //--- store coding state ---
                  store_coding_state (cs_b8);
                } // if (rdcost <= min_rdcost)

                //--- re-set coding state as it was before coding with current mode was performed ---
                reset_coding_state (cs_cm);
              } // if ((valid[mode] && mode != 0) || (mode == 0 && active_sps->direct_8x8_inference_flag))
            } // for (min_rdcost=1e30, index=(bframe?0:1); index<6; index++)

#ifdef BEST_NZ_COEFF
            {
              int x, y;
              for(y = 0; y <= 1; y++)
                for(x = 0; x <= 1; x++)
                  img->nz_coeff[img->current_mb_nr][i1 + x][j1 + y]
                  = best_nz_coeff[x][y];
            }
#endif

            if (!input->rdopt)
            {
              cost8x8 += min_cost8x8;

              mode = part8x8mode[block];
              pdir = part8x8pdir[block];

              curr_cbp_blk  = 0;
              best_cnt_nonz = LumaResidualCoding8x8 (&dummy, &curr_cbp_blk, block, pdir,
                                                    (pdir==0||pdir==2?mode:0),
                                                    (pdir==1||pdir==2?mode:0),
                                                    part8x8fwref[block],
                                                    part8x8bwref[block]);
              cbp_blk8x8   &= (~(0x33 << (((block>>1)<<3)+((block%2)<<1)))); // delete bits for block
              cbp_blk8x8   |= curr_cbp_blk;

              //--- store coefficients ---
              for (k=0; k< 4; k++)
                for (j=0; j< 2; j++)
                  for (i=0; i<65; i++)  cofAC_8x8ts[block][k][j][i] = img->cofAC[block][k][j][i]; // 18->65 for ABT

              //--- store reconstruction and prediction ---
              for (j=j0; j<j0+8; j++)
                for (i=i0; i<i0+8; i++)
                {
                  // Residue Color Transform
                  if(!img->residue_transform_flag)
                  {
                    rec_mbY_8x8ts[j][i] = enc_picture->imgY[img->pix_y+j][img->pix_x+i];
                    mpr_8x8ts    [j][i] = img->mpr[i][j];
                  } 
                  else 
                  {
                    rec_resG_8x8ts[i][j] = rec_resG[i][j];
                    resTrans_R_8x8ts[i][j] = resTrans_R[i][j];
                    resTrans_B_8x8ts[i][j] = resTrans_B[i][j];
                    mprRGB_8x8ts[0][i][j] = mprRGB[0][i][j];
                    mprRGB_8x8ts[1][i][j] = mprRGB[1][i][j];
                    mprRGB_8x8ts[2][i][j] = mprRGB[2][i][j];
                  }
                }
            }

            //----- set cbp and count of nonzero coefficients ---
            if (best_cnt_nonz)
            {
              cbp8x8        |= (1<<block);
              cnt_nonz_8x8   += best_cnt_nonz;
            }

            //======= save motion data for 8x8 partition for transform size 8x8 ========
            StoreNewMotionVectorsBlock8x8(0, block, part8x8mode[block], part8x8fwref[block], part8x8bwref[block], part8x8pdir[block], bframe);

            //set motion data for using as predictor data for next run
            SetRefAndMotionVectors (block, part8x8mode[block], part8x8pdir[block], part8x8fwref[block], part8x8bwref[block]);

            //===== set the coding state after current block =====
            if (block < 3)
              reset_coding_state (cs_b8);

          } // for (cbp8x8=cbp_blk8x8=cnt_nonz_8x8=0, block=0; block<4; block++)

          cbp8_8x8ts      = cbp8x8;
          cbp_blk8_8x8ts  = cbp_blk8x8;
          cnt_nonz8_8x8ts = cnt_nonz_8x8;
          cost8_8x8ts     = cost8x8;                //for disabled rd-opt
          currMB->luma_transform_size_8x8_flag = 0; //switch to 4x4 transform size
        }// if (input->AllowTransform8x8)

        cost8x8 = INT_MAX;

        if (input->AllowTransform8x8 != 2)  
        {
          cost8x8 = 0;

          //=================================================================
          // Check 8x8, 8x4, 4x8 and 4x4 partitions with transform size 4x4
          //=================================================================
          //=====  LOOP OVER 8x8 SUB-PARTITIONS  (Motion Estimation & Mode Decision) =====
          for (cost_direct=cbp8x8=cbp_blk8x8=cnt_nonz_8x8=0, block=0; block<4; block++)
          {
            //--- set coordinates ---
            j0 = ((block/2)<<3);    j1 = (j0>>2);
            i0 = ((block%2)<<3);    i1 = (i0>>2);
            
#ifdef BEST_NZ_COEFF
            {
              int x, y;
              for(y = 0; y <= 1; y++)
                for(x = 0; x <= 1; x++)
                  best_nz_coeff[x][y] = img->nz_coeff[img->current_mb_nr][i1 + x][j1 + y] = 0;
            }
#endif
            //=====  LOOP OVER POSSIBLE CODING MODES FOR 8x8 SUB-PARTITION  =====
            for (min_cost8x8=INT_MAX, min_rdcost=1e30, index=(bframe?0:1); index<5; index++)
            {
              if (valid[mode=b8_mode_table[index]])
              {
                curr_cbp_blk = 0;
                
                if (mode==0)
                {
                  //--- Direct Mode ---
                  if (!input->rdopt)
                  {
                    cost = Get_Direct_Cost8x8 ( block,  &direct8x8_tmp);
                    if((cost==INT_MAX)||(cost_direct==INT_MAX))
                    {
                      cost_direct = INT_MAX;
                    }
                    else
                    {
                      cost_direct += cost;
                    }
                    have_direct ++;
                  }
                  best_fw_ref = direct_ref_idx[LIST_0][img->block_x+(block&1)*2][img->block_y+(block&2)];
                  best_bw_ref = direct_ref_idx[LIST_1][img->block_x+(block&1)*2][img->block_y+(block&2)];
                  best_pdir   = direct_pdir[img->block_x+(block&1)*2][img->block_y+(block&2)];
                } // if (mode==0)
                else
                {
                  //--- motion estimation for all reference frames ---
                  PartitionMotionSearch (mode, block, lambda_motion);
                  
                  //--- get cost and reference frame for forward prediction ---
                  for (fw_mcost=max_mcost, ref=0; ref<listXsize[LIST_0+list_offset]; ref++)
                  {
                    if (!checkref || ref==0 || CheckReliabilityOfRef (block, LIST_0, ref, mode))
                    {
                      mcost  = (input->rdopt ? REF_COST(lambda_motion_factor,ref,LIST_0+list_offset) : (int)(2*lambda_motion*min(ref,1)));
                      
                      mcost += motion_cost[mode][LIST_0][ref][block];
                      if (mcost < fw_mcost)
                      {
                        fw_mcost    = mcost;
                        best_fw_ref = ref;
                      }
                    }
                  }
                  
                  //store forward reference index for every block
                  for (j=0; j<2; j++)
                    for (i=0; i<2; i++)
                    {
                      enc_picture->ref_idx[LIST_0][img->block_x+(block&1)*2+i][img->block_y+(block&2)+j] = best_fw_ref;
                      enc_picture->ref_pic_id [LIST_0][img->block_x+(block&1)*2+i][img->block_y+(block&2)+j] = enc_picture->ref_pic_num[LIST_0 + list_offset][enc_picture->ref_idx[LIST_0][img->block_x+(block&1)*2+i][img->block_y+(block&2)+j]];
                    }
                    
                    if (bframe)
                    {
                      for (bw_mcost=max_mcost, ref=0; ref<listXsize[LIST_1+list_offset]; ref++)
                      {
                        mcost  = (input->rdopt ? REF_COST(lambda_motion_factor,ref,LIST_1+list_offset) : (int)(2*lambda_motion*min(ref,1)));
                        
                        mcost += motion_cost[mode][LIST_1][ref][block];
                        if (mcost < bw_mcost)
                        {
                          bw_mcost    = mcost;
                          best_bw_ref = ref;
                        }
                      }
                      
                      // bidirectional uses best forward and zero backward reference
                      bid_mcost  = (input->rdopt ? (REF_COST (lambda_motion_factor, best_fw_ref, LIST_0 + list_offset)+REF_COST (lambda_motion_factor, 0, LIST_1 + list_offset)) : (int)(2*lambda_motion*min(best_fw_ref,1)));
                      bid_mcost += BIDPartitionCost (mode, block, best_fw_ref, 0, lambda_motion_factor );
                      
                      //--- get prediction direction ----
                      if      (fw_mcost<=bw_mcost && fw_mcost<=bid_mcost)
                      {
                        best_pdir = 0;
                        cost = fw_mcost;
                        best_bw_ref = -1;
                      }
                      else if (bw_mcost<=fw_mcost && bw_mcost<=bid_mcost)
                      {
                        best_pdir = 1;
                        cost = bw_mcost;
                        best_fw_ref = -1;
                      }
                      else
                      {
                        best_pdir = 2;
                        cost = bid_mcost;
                        best_bw_ref = 0;
                      }
                      //store backward reference index for every block
                      for (j=0; j<2; j++)
                        for (i=0; i<2; i++)
                        {
                          enc_picture->ref_idx[LIST_0][img->block_x+(block&1)*2+i][img->block_y+(block&2)+j] = best_fw_ref;
                          enc_picture->ref_idx[LIST_1][img->block_x+(block&1)*2+i][img->block_y+(block&2)+j] = best_bw_ref;
                        }
                    } // if (bframe)
                    else
                    {
                      best_pdir = 0;
                      cost      = fw_mcost;
                    }
                } // if (mode!=0)
                
                //--- store coding state before coding with current mode ---
                store_coding_state (cs_cm);
                
                if (input->rdopt)
                {
                  //--- get and check rate-distortion cost ---
                  rdcost = RDCost_for_8x8blocks (&cnt_nonz, &curr_cbp_blk, lambda_mode,
                    block, mode, best_pdir, best_fw_ref, best_bw_ref);
                }
                else
                {
                  if (cost!=INT_MAX)
                  cost += (REF_COST (lambda_motion_factor, B8Mode2Value (mode, best_pdir), list_offset + (best_pdir<1?0:1)) - 1);
                }
                
                //--- set variables if best mode has changed ---
                if (( input->rdopt && rdcost < min_rdcost) ||
                  (!input->rdopt && cost   < min_cost8x8  )   )
                {
                  min_cost8x8                  = cost;
                  min_rdcost                   = rdcost;
                  best8x8mode          [block] = mode;
                  best8x8pdir    [P8x8][block] = best_pdir;
                  best8x8fwref   [P8x8][block] = best_fw_ref;
                  best8x8bwref   [P8x8][block] = best_bw_ref;
                  
                  
#ifdef BEST_NZ_COEFF
                  {
                    int x, y;
                    for(y = 0; y <= 1; y++)
                      for(x = 0; x <= 1; x++)
                        best_nz_coeff[x][y]= cnt_nonz ? img->nz_coeff[img->current_mb_nr][i1 + x][j1 + y] : 0;
                  }
#endif
                  
                  //--- store number of nonzero coefficients ---
                  best_cnt_nonz  = cnt_nonz;
                  
                  if (input->rdopt)
                  {
                    //--- store block cbp ---
                    cbp_blk8x8    &= (~(0x33 << (((block>>1)<<3)+((block%2)<<1)))); // delete bits for block
                    cbp_blk8x8    |= curr_cbp_blk;
                    
                    //--- store coefficients ---
                    for (k=0; k< 4; k++)
                      for (j=0; j< 2; j++)
                        for (i=0; i<65; i++)  cofAC8x8[block][k][j][i] = img->cofAC[block][k][j][i]; // 18->65 for ABT
                        
                    //--- store reconstruction and prediction ---
                    for (j=j0; j<j0+8; j++)
                      for (i=i0; i<i0+8; i++)
                      {
                        rec_mbY8x8[j][i] = enc_picture->imgY[img->pix_y+j][img->pix_x+i];
                        mpr8x8    [j][i] = img->mpr[i][j];
                      }
                  }
                  // Residue Color Transform
                  if(img->residue_transform_flag)
                  {
                    for (j=j0; j<j0+8; j++)
                      for (i=i0; i<i0+8; i++)
                      {
                        rec_resG_8x8[i][j] = rec_resG[i][j];
                        resTrans_R_8x8[i][j] = resTrans_R[i][j];
                        resTrans_B_8x8[i][j] = resTrans_B[i][j];
                        mprRGB_8x8[0][i][j] = mprRGB[0][i][j];
                        mprRGB_8x8[1][i][j] = mprRGB[1][i][j];
                        mprRGB_8x8[2][i][j] = mprRGB[2][i][j];
                      }
                  }

                  //--- store coding state ---
                  store_coding_state (cs_b8);
                } // if (rdcost <= min_rdcost)
                
                //--- re-set coding state as it was before coding with current mode was performed ---
                reset_coding_state (cs_cm);
              } // if (valid[mode=b8_mode_table[index]])
            } // for (min_rdcost=1e30, index=(bframe?0:1); index<6; index++)

#ifdef BEST_NZ_COEFF
            {
              int x, y;
              for(y = 0; y <= 1; y++)
                for(x = 0; x <= 1; x++)
                  img->nz_coeff[img->current_mb_nr][i1 + x][j1 + y]
                  = best_nz_coeff[x][y];
            }
#endif
            
            cost8x8 += min_cost8x8;
            
            if (!input->rdopt)
            {
              mode = best8x8mode[block];
              pdir = best8x8pdir[P8x8][block];
              
              curr_cbp_blk  = 0;
              best_cnt_nonz = LumaResidualCoding8x8 (&dummy, &curr_cbp_blk, block, pdir,
                                                      (pdir==0||pdir==2?mode:0),
                                                      (pdir==1||pdir==2?mode:0),
                                                      best8x8fwref[P8x8][block],
                                                      best8x8bwref[P8x8][block]);

              cbp_blk8x8   &= (~(0x33 << (((block>>1)<<3)+((block%2)<<1)))); // delete bits for block
              cbp_blk8x8   |= curr_cbp_blk;

              //--- store coefficients ---
              for (k=0; k< 4; k++)
                for (j=0; j< 2; j++)
                  for (i=0; i<65; i++)  cofAC8x8[block][k][j][i] = img->cofAC[block][k][j][i]; // 18->65 for ABT

                  //--- store reconstruction and prediction ---
                  for (j=j0; j<j0+8; j++)
                    for (i=i0; i<i0+8; i++)
                    {
                      rec_mbY8x8[j][i] = enc_picture->imgY[img->pix_y+j][img->pix_x+i];
                      mpr8x8    [j][i] = img->mpr[i][j];
                    }
                  // Residue Color Transform
                  if(img->residue_transform_flag)
                  {
                    for (j=j0; j<j0+8; j++)
                      for (i=i0; i<i0+8; i++)
                      {
                        rec_resG_8x8[i][j] = rec_resG[i][j];
                        resTrans_R_8x8[i][j] = resTrans_R[i][j];
                        resTrans_B_8x8[i][j] = resTrans_B[i][j];
                        mprRGB_8x8[0][i][j] = mprRGB[0][i][j];
                        mprRGB_8x8[1][i][j] = mprRGB[1][i][j];
                        mprRGB_8x8[2][i][j] = mprRGB[2][i][j];
                      }
                  }
            }
            
            //----- set cbp and count of nonzero coefficients ---
            if (best_cnt_nonz)
            {
              cbp8x8        |= (1<<block);
              cnt_nonz_8x8  += best_cnt_nonz;
            }
            
            mode=best8x8mode[block];
            //===== reset intra prediction modes (needed for prediction, must be stored after 8x8 mode dec.) =====
            j0 = img->block_y+2*(block/2);
            i0 = img->block_x+2*(block%2);
            for (j=j0; j<j0+2; j++)
              for (i=i0; i<i0+2; i++) 
                ipredmodes[i][j]         = DC_PRED;
              
            i0 = 4*block;
            for (i=i0; i<i0+4; i++)    currMB->intra_pred_modes[i]  = DC_PRED;
            
            if (block<3)
            {
              //===== re-set reconstructed block =====
              j0   = 8*(block/2);
              i0   = 8*(block%2);
              for (j=j0; j<j0+8; j++)
                for (i=i0; i<i0+8; i++)  
                  enc_picture->imgY[img->pix_y+j][img->pix_x+i] = rec_mbY8x8[j][i];
            } // if (block<3)
              
              //===== set motion vectors and reference frames (prediction) =====
              SetRefAndMotionVectors (block, mode, best8x8pdir[P8x8][block], best8x8fwref[P8x8][block], best8x8bwref[P8x8][block]);

              //===== set the coding state after current block =====
              reset_coding_state (cs_b8);

          } // for (cbp8x8=cbp_blk8x8=cnt_nonz_8x8=0, block=0; block<4; block++)
        } // if (input->AllowTransform8x8 != 2)


        //--- re-set coding state (as it was before 8x8 block coding) ---
        reset_coding_state (cs_mb);
        for (i=0; i<16; i++)
          for(j=0; j<16; j++)
            diffy[j][i] = imgY_org[img->opix_y+j][img->opix_x+i]-img->mpr[i][j];

        //check cost for P8x8 for non-rdopt mode
        if (!input->rdopt && (cost8x8 < min_cost || cost8_8x8ts < min_cost))
        {
          best_mode = P8x8;
          if (input->AllowTransform8x8)
          {
            if (cost8_8x8ts < cost8x8)
            {
              min_cost = cost8_8x8ts;
              currMB->luma_transform_size_8x8_flag=1;
            }
            else if(cost8x8 < cost8_8x8ts)
            {
              min_cost = cost8x8;
              currMB->luma_transform_size_8x8_flag=0;
            }
            else
            {
              min_cost = cost8_8x8ts;
              currMB->luma_transform_size_8x8_flag=1;
              if (GetBestTransformP8x8() == 0)
              {
                min_cost = cost8x8;
                currMB->luma_transform_size_8x8_flag=0;
              }
            }
          }
          else
          {
            min_cost = cost8x8;
            currMB->luma_transform_size_8x8_flag=0;
          }
        }// if (!input->rdopt && (cost8x8 < min_cost || cost8_8x8ts < min_cost))
      }
      else // if (valid[P8x8])
      {
        cost8x8 = INT_MAX;
      }

      // Find a motion vector for the Skip mode
      if((img->type == P_SLICE)||(img->type == SP_SLICE))
        FindSkipModeMotionVector ();
    }
    else // if (img->type!=I_SLICE)
    {
      min_cost = INT_MAX;
    }

    //========= C H O O S E   B E S T   M A C R O B L O C K   M O D E =========
    //-------------------------------------------------------------------------

    if (input->rdopt)
    {
      int mb_available_up;
      int mb_available_left;
      int mb_available_up_left;

      min_rdcost = max_rdcost;

      if (img->yuv_format != YUV400)
      {
        // precompute all new chroma intra prediction modes
        IntraChromaPrediction(&mb_available_up, &mb_available_left, &mb_available_up_left);
        for (currMB->c_ipred_mode=DC_PRED_8; currMB->c_ipred_mode<=PLANE_8; currMB->c_ipred_mode++)
        {

          // bypass if c_ipred_mode is not allowed
          if ((input->ChromaIntraDisable == 1 && currMB->c_ipred_mode!=DC_PRED_8) ||
            (currMB->c_ipred_mode==VERT_PRED_8 && !mb_available_up) ||
              (currMB->c_ipred_mode==HOR_PRED_8 && !mb_available_left) ||
              (currMB->c_ipred_mode==PLANE_8 && (!mb_available_left || !mb_available_up || !mb_available_up_left)))
            continue;

          // Residue Color Transform
          if(img->residue_transform_flag)
            max_index = 11;
          else
            max_index = 8;

#if BI_PREDICTION
          if (input->BiPredMotionEstimation)
          img->bi_pred_me[1] =0;  
#endif

          //===== GET BEST MACROBLOCK MODE =====
          for (ctr16x16=0, index=0; index<max_index; index++)
          {
            mode = mb_mode_table[index];
            // Residue Color Transform
            if(img->residue_transform_flag)
            {
              mode = mb_mode_table_RCT[index];
              if( mode == I16MB) i16mode = index -5;
              // bypass if i16mode is not allowed
              if (mode == I16MB &&
                ((i16mode==VERT_PRED_16 && !mb_available_up) ||
                (i16mode==HOR_PRED_16 && !mb_available_left) ||
                (i16mode==PLANE_16 && (!mb_available_left || !mb_available_up || !mb_available_up_left))))
                continue;
            }
            else
            {
              mode = mb_mode_table[index];
              i16mode = 0; 
            }

            //--- for INTER16x16 check all prediction directions ---
            if (mode==1 && img->type==B_SLICE)
            {
              best8x8pdir[1][0] = best8x8pdir[1][1] = best8x8pdir[1][2] = best8x8pdir[1][3] = ctr16x16;
#if BI_PREDICTION            
              if ((input->BiPredMotionEstimation) && (img->type==B_SLICE))
              {
                if (ctr16x16 == 2 && img->bi_pred_me[mode] < 2 && mode == 1 ) 
                {
                  ctr16x16--;
                }                
              }
#endif
              if (ctr16x16 < 2) index--;
              ctr16x16++;
            }

            img->NoResidueDirect = 0;

            if (input->SkipIntraInInterSlices && img->type != I_SLICE)
            {
              if (mode >= I16MB && best_mode == 0 && currMB->cbp == 0)
              {
                valid[mode] = 0;
              }
            }

            if (valid[mode])
            {
              //--- transform size ---
              currMB->luma_transform_size_8x8_flag = 0; //4x4 transform defined
              if (input->AllowTransform8x8==2)
              {
                if (mode >= 1 && mode <= 3)
                  currMB->luma_transform_size_8x8_flag = 1;
                else if (mode == 0 && bframe && active_sps->direct_8x8_inference_flag)
                  currMB->luma_transform_size_8x8_flag = 1;
                else if ((mode == P8x8) && (valid[4]))
                  currMB->luma_transform_size_8x8_flag = 1; //run mode 4 with 8x8 transform
              }

              // bypass if c_ipred_mode not used
              SetModesAndRefframeForBlocks (mode);
              if (currMB->c_ipred_mode == DC_PRED_8 || (IS_INTRA(currMB) ))
              {
                while(1)
                {
                  if (RDCost_for_macroblocks (lambda_mode, mode, &min_rdcost, i16mode))
                  {
                    //Rate control
                    if(mode == P8x8)
                    {
                      for (i=0; i<16; i++)
                        for(j=0; j<16; j++)
                          diffy[j][i] = imgY_org[img->opix_y+j][img->opix_x+i] - mpr8x8[j][i];
                    }else
                    {
                      for (i=0; i<16; i++)
                        for(j=0; j<16; j++)
                          diffy[j][i] = imgY_org[img->opix_y+j][img->opix_x+i] - pred[j][i];
                    }

                    store_macroblock_parameters (mode);
                  }

                  if (input->AllowTransform8x8==1)
                  {
                    //=========== try mb_types 1,2,3 with 8x8 transform ===========
                    if ((mode >= 1 && mode <= 3) && currMB->luma_transform_size_8x8_flag == 0)
                    {
                      //try with 8x8 transform size
                      currMB->luma_transform_size_8x8_flag = 1;
                      continue;
                    }
                    //=========== try DIRECT-MODE with 8x8 transform ===========
                    else if (mode == 0 && bframe && active_sps->direct_8x8_inference_flag && currMB->luma_transform_size_8x8_flag == 0)
                    {
                      //try with 8x8 transform size
                      currMB->luma_transform_size_8x8_flag = 1;
                      continue;
                    }
                    //=========== try mb_type P8x8 for mode 4 with 4x4/8x8 transform ===========
                    else if ((mode == P8x8) && (valid[4]) && (currMB->luma_transform_size_8x8_flag == 0))
                    {
                      currMB->luma_transform_size_8x8_flag = 1; //check 8x8 partition for transform size 8x8
                      continue;
                    }
                    else
                    {
                      currMB->luma_transform_size_8x8_flag = 0;
                      break;
                    }
                  }
                  else
                    break;
                }
              } // if (currMB->c_ipred_mode == DC_PRED_8 ||(IS_INTRA(currMB) ))

              if (valid[0] && bframe && mode == 0 && currMB->cbp && (currMB->cbp&15) != 15 && !input->nobskip)
              {
                img->NoResidueDirect = 1;
                if (RDCost_for_macroblocks (lambda_mode, mode, &min_rdcost, i16mode))
                {
                  //Rate control
                  for (i=0; i<16; i++)
                    for(j=0; j<16; j++)
                      diffy[j][i] = imgY_org[img->opix_y+j][img->opix_x+i] - pred[j][i];
                    store_macroblock_parameters (mode);
                }
              }
            } // if valid[mode]
#if BI_PREDICTION            
            if ((input->BiPredMotionEstimation) && (img->type==B_SLICE))
            {
              if (ctr16x16 == 2 && img->bi_pred_me[mode] < 2 && mode == 1 && best8x8pdir[1][0] == 2) 
              {
                img->bi_pred_me[mode] = img->bi_pred_me[mode] + 1;
              }
            }
            
#endif

          }// for (ctr16x16=0, index=0; index<8; index++)
        }// for (currMB->c_ipred_mode=DC_PRED_8; currMB->c_ipred_mode<=PLANE_8; currMB->c_ipred_mode++)
      }//if (img->yuv_format != YUV400)
      else
      {
#if BI_PREDICTION            
        if (input->BiPredMotionEstimation)
          img->bi_pred_me[1] =0;  
#endif
        //============= YUV400 ===============
        //===== GET BEST MACROBLOCK MODE =====
        for (ctr16x16=0, index=0; index<8; index++)
        {
          mode = mb_mode_table[index];

          //--- for INTER16x16 check all prediction directions ---
          if (mode==1 && img->type==B_SLICE)
          {
            best8x8pdir[1][0] = best8x8pdir[1][1] = best8x8pdir[1][2] = best8x8pdir[1][3] = ctr16x16;
#if BI_PREDICTION            
            if ((input->BiPredMotionEstimation) && (img->type==B_SLICE))
            {
              if (ctr16x16 == 2 && img->bi_pred_me[mode] < 2 && mode == 1 ) 
              {
                ctr16x16--;
              }
              
            }
#endif
            
            if (ctr16x16 < 2) index--;
            ctr16x16++;
         }

          img->NoResidueDirect = 0;

          if (valid[mode])
          {
            //--- transform size ---
            currMB->luma_transform_size_8x8_flag = 0; //4x4 transform defined
            if (input->AllowTransform8x8==2)
            {
              if (mode >= 1 && mode <= 3)
                currMB->luma_transform_size_8x8_flag = 1;
              else if (mode == 0 && bframe && active_sps->direct_8x8_inference_flag)
                currMB->luma_transform_size_8x8_flag = 1;
              else if ((mode == P8x8) && (valid[4]))
                currMB->luma_transform_size_8x8_flag = 1; //run mode 4 with 8x8 transform
            }

            // bypass if c_ipred_mode not used
            SetModesAndRefframeForBlocks (mode);
            if (currMB->c_ipred_mode == DC_PRED_8 || (IS_INTRA(currMB) ))
            {
              while(1)
              {
                if (RDCost_for_macroblocks (lambda_mode, mode, &min_rdcost, i16mode))
                {
                  //Rate control
                  if(mode == P8x8)
                  {
                    for (i=0; i<16; i++)
                      for(j=0; j<16; j++)
                        diffy[j][i] = imgY_org[img->opix_y+j][img->opix_x+i] - mpr8x8[j][i];
                  }else
                  {
                    for (i=0; i<16; i++)
                      for(j=0; j<16; j++)
                        diffy[j][i] = imgY_org[img->opix_y+j][img->opix_x+i] - pred[j][i];
                  }

                  store_macroblock_parameters (mode);
                }

                if (input->AllowTransform8x8==1)
                {
                  //=========== try mb_types 1,2,3 with 8x8 transform ===========
                  if ((mode >= 1 && mode <= 3) && currMB->luma_transform_size_8x8_flag == 0)
                  {
                    //try with 8x8 transform size
                    currMB->luma_transform_size_8x8_flag = 1;
                    continue;
                  }
                  //=========== try DIRECT-MODE with 8x8 transform ===========
                  else if (mode == 0 && bframe && active_sps->direct_8x8_inference_flag && currMB->luma_transform_size_8x8_flag == 0)
                  {
                    //try with 8x8 transform size
                    currMB->luma_transform_size_8x8_flag = 1;
                    continue;
                  }
                  //=========== try mb_type P8x8 for mode 4 with 4x4/8x8 transform ===========
                  else if ((mode == P8x8) && (valid[4]) && (currMB->luma_transform_size_8x8_flag == 0))
                  {
                    currMB->luma_transform_size_8x8_flag = 1; //check 8x8 partition for transform size 8x8
                    continue;
                  }
                  else
                  {
                    currMB->luma_transform_size_8x8_flag = 0;
                    break;
                  }
                }
                else
                  break;
              }
            } // if (currMB->c_ipred_mode == DC_PRED_8 ||(IS_INTRA(currMB) ))

            if (valid[0] && bframe && mode == 0 && currMB->cbp && (currMB->cbp&15) != 15 && !input->nobskip) //g050
            {
              img->NoResidueDirect = 1;
              if (RDCost_for_macroblocks (lambda_mode, mode, &min_rdcost, i16mode))
              {
                //Rate control
                for (i=0; i<16; i++)
                  for(j=0; j<16; j++)
                    diffy[j][i] = imgY_org[img->opix_y+j][img->opix_x+i] - pred[j][i];
                  store_macroblock_parameters (mode);
              }
            }
        
          } // if valid[mode]
#if BI_PREDICTION            
          if ((input->BiPredMotionEstimation) && (img->type==B_SLICE))
          {
            if (ctr16x16 == 2 && img->bi_pred_me[mode] < 2 && mode == 1 && best8x8pdir[1][0] == 2) 
            {
              img->bi_pred_me[mode] = img->bi_pred_me[mode] + 1;
            }
          }         
#endif
        }// for (ctr16x16=0, index=0; index<8; index++)
      }//else - if (img->yuv_format != YUV400)
    }
    else //rdopt off
    {
      tmp_8x8_flag = currMB->luma_transform_size_8x8_flag;	//save 8x8_flag
      tmp_no_mbpart = currMB->NoMbPartLessThan8x8Flag;		  //save no-part-less

      if (img->yuv_format != YUV400)
        // precompute all chroma intra prediction modes
        IntraChromaPrediction(NULL, NULL, NULL);

      if (valid[0] && bframe) // check DIRECT MODE
      {
        if(have_direct)
        {
          switch(input->AllowTransform8x8)
          {
          case 1: // Mixture of 8x8 & 4x4 transform
                  if((cost8x8_direct<cost_direct)||
                  !(valid[5] && valid[6] && valid[7]))
                  {
                    cost = cost8x8_direct;
                  }
                  else
                  {
                    cost = cost_direct;
                  }
                  break;
          case 2: // 8x8 Transform only
                  cost = cost8x8_direct;
                  break;
          default: // 4x4 Transform only
                  cost = cost_direct;
                  break;
          }
        }
        else
        { //!have_direct
          cost = Get_Direct_CostMB (lambda_mode);
        }
        cost -= (int)floor(16*lambda_motion+0.4999);
        if (cost <= min_cost)
        {
          if(active_sps->direct_8x8_inference_flag && input->AllowTransform8x8)
          {
            if(input->AllowTransform8x8==2)
              currMB->luma_transform_size_8x8_flag=1;
            else
            {
              if(cost8x8_direct<cost_direct)
                currMB->luma_transform_size_8x8_flag=1;
              else
                currMB->luma_transform_size_8x8_flag=0;
            }
          }
          else
            currMB->luma_transform_size_8x8_flag=0;
          
          //Rate control
          for (i=0; i<16; i++)
            for(j=0; j<16; j++)
              diffy[j][i] = imgY_org[img->pix_y+j][img->pix_x+i]-img->mpr[i][j];

          min_cost  = cost;
          best_mode = 0;
        }
        else
        {
			    currMB->luma_transform_size_8x8_flag = tmp_8x8_flag; // restore if not best
		      currMB->NoMbPartLessThan8x8Flag = tmp_no_mbpart; // restore if not best
    		}

      }
      if (valid[I8MB]) // check INTRA8x8
      {
        currMB->luma_transform_size_8x8_flag = 1; // at this point cost will ALWAYS be less than min_cost 

        currMB->mb_type = I8MB;
        temp_cpb = Mode_Decision_for_new_Intra8x8Macroblock (lambda_mode, &cost);

        if (cost <= min_cost)
        {
        // Residue Color Transform
           if(img->residue_transform_flag)
           {

               for(i=0; i<2; i++)
                 for(j=0; j<4; j++)
                   for(k=0; k<4; k++)
                     if(cbp_chroma_block[i][j][k])
                       cr_cbp = 2;

               cr_cbp = dct_chroma_DC(0, cr_cbp);
               cr_cbp = dct_chroma_DC(1, cr_cbp);

               temp_cpb += (cr_cbp<<4);
               for(j=0; j<MB_BLOCK_SIZE; j++)
                 for(i=0; i<MB_BLOCK_SIZE; i++)
                 {
                   temp_imgU[j][i] = enc_picture->imgUV[0][img->pix_y+j][img->pix_x+i];
                   temp_imgV[j][i] = enc_picture->imgUV[1][img->pix_y+j][img->pix_x+i];
                 }
           }
           
           currMB->cbp = temp_cpb;
           
           //coeffs
           if (input->AllowTransform8x8 != 2)
           {
             i4p=cofAC; cofAC=img->cofAC; img->cofAC=i4p;
           }
           
           for(j=0; j<MB_BLOCK_SIZE; j++)
             for(i=0; i<MB_BLOCK_SIZE; i++)
             {
               temp_imgY[j][i] = enc_picture->imgY[img->pix_y+j][img->pix_x+i];
             }

           //Rate control
           for (i=0; i<16; i++)
             for(j=0; j<16; j++)
               diffy[j][i] = imgY_org[img->pix_y+j][img->pix_x+i]-img->mpr[i][j];

           min_cost  = cost;
           best_mode = I8MB;
           tmp_8x8_flag = currMB->luma_transform_size_8x8_flag;
        } 
        else
          currMB->luma_transform_size_8x8_flag = tmp_8x8_flag; // restore if not best
      }
      if (valid[I4MB]) // check INTRA4x4
      {
        currMB->luma_transform_size_8x8_flag = 0;
        currMB->mb_type = I4MB;
        temp_cpb = Mode_Decision_for_Intra4x4Macroblock (lambda_mode, &cost);
        
        if (cost <= min_cost)
        {
        // Residue Color Transform
          if(img->residue_transform_flag)
          {
            for(i=0; i<2; i++)
              for(j=0; j<4; j++)
                for(k=0; k<4; k++)
                  if(cbp_chroma_block[i][j][k])
                    cr_cbp = 2;

            cr_cbp = dct_chroma_DC(0, cr_cbp);
            cr_cbp = dct_chroma_DC(1, cr_cbp);

            temp_cpb += (cr_cbp<<4);
          }
          currMB->cbp = temp_cpb;

          //Rate control
          for (i=0; i<16; i++)
            for(j=0; j<16; j++)
              diffy[j][i] = imgY_org[img->pix_y+j][img->pix_x+i]-img->mpr[i][j];

          min_cost  = cost;
          best_mode = I4MB;
          tmp_8x8_flag = currMB->luma_transform_size_8x8_flag;
        } 
        else
        {
          currMB->luma_transform_size_8x8_flag = tmp_8x8_flag; // restore if not best
          //coeffs
          i4p=cofAC; cofAC=img->cofAC; img->cofAC=i4p;
        }
      }
      if (valid[I16MB]) // check INTRA16x16
      {
        currMB->luma_transform_size_8x8_flag = 0;
        intrapred_luma_16x16 ();
        cost = find_sad_16x16 (&i16mode);

        if (cost < min_cost)
        {
          //Rate control
          for (i=0; i<16; i++)
            for(j=0; j<16; j++)
              diffy[j][i] = imgY_org[img->pix_y+j][img->pix_x+i]-img->mprr_2[i16mode][j][i];

          // Residue Color Transform
          if(img->residue_transform_flag)
          {
            for (j=0; j<16; j++)
              for (i=0; i<16; i++)
              {
                residue_B = imgUV_org[0][img->pix_y+j][img->pix_x+i] - img->mprr_c[0][currMB->c_ipred_mode][i][j];
                residue_G = imgY_org[img->pix_y+j][img->pix_x+i] - img->mprr_2[i16mode][j][i];
                residue_R = imgUV_org[1][img->pix_y+j][img->pix_x+i] - img->mprr_c[1][currMB->c_ipred_mode][i][j];
                
                /* Forward Residue Transform */
                resTrans_R[i][j] = residue_R-residue_B;
                temp = residue_B+(resTrans_R[i][j]>>1);
                resTrans_B[i][j] = residue_G-temp;
                resTrans_G[i][j] = temp+(resTrans_B[i][j]>>1);
                
                img->m7[i][j]  = resTrans_G[i][j];
              }
          }
          best_mode   = I16MB;
          currMB->cbp = dct_luma_16x16 (i16mode);

          // Residue Color Transform
          if(img->residue_transform_flag)
          {
            for (j=0; j<16; j++)
            for (i=0; i<16; i++)
            {
              rec_resG[i][j] = img->m7[i][j];
              img->m7[i][j]  = resTrans_R[i][j];
            }

            cr_cbp = dct_chroma(0, 0);

            for (j=0; j<16; j++)
            for (i=0; i<16; i++)
            {
              rec_resR[i][j] = img->m7[i][j];
              img->m7[i][j]  = resTrans_B[i][j];
            }

            cr_cbp = dct_chroma(1, cr_cbp);

            for (j=0; j<16; j++)
            for (i=0; i<16; i++)
            {
              rec_resB[i][j] = img->m7[i][j];
            }

            currMB->cbp += (cr_cbp<<4);

            /* Inverse Residue Transform */
            for (j=0; j<16; j++)
            for (i=0; i<16; i++)
            {
              temp      = rec_resG[i][j]-(rec_resB[i][j]>>1);
              residue_G = rec_resB[i][j]+temp;
              residue_B = temp - (rec_resR[i][j]>>1);
              residue_R = residue_B+rec_resR[i][j];

              enc_picture->imgUV[0][img->pix_y+j][img->pix_x+i] = min(img->max_imgpel_value_uv,max(0,residue_B+(int)img->mprr_c[0][currMB->c_ipred_mode][i][j]));
              enc_picture->imgY[img->pix_y+j][img->pix_x+i]     = min(img->max_imgpel_value,max(0,residue_G+(int)img->mprr_2[i16mode][j][i]));
              enc_picture->imgUV[1][img->pix_y+j][img->pix_x+i] = min(img->max_imgpel_value_uv,max(0,residue_R+(int)img->mprr_c[1][currMB->c_ipred_mode][i][j]));
            }
          }
        }
        else
        {
			    currMB->luma_transform_size_8x8_flag = tmp_8x8_flag; // restore
			    currMB->NoMbPartLessThan8x8Flag = tmp_no_mbpart;     // restore
        }
      }
    }

    if (rerun==0)
    {
      intra1 = (currMB->mb_type==I16MB || currMB->mb_type==I4MB || currMB->mb_type==I8MB ? 1 : 0);
    }
  } // for (rerun=0; rerun<runs; rerun++)


  //=====  S E T   F I N A L   M A C R O B L O C K   P A R A M E T E R S ======
  //---------------------------------------------------------------------------

  if (input->rdopt)
  {

    if ((cbp!=0 || best_mode==I16MB ))
      currMB->prev_cbp = 1;
    else if (cbp==0 && !input->RCEnable)
    {
      currMB->delta_qp = 0;
      currMB->qp = currMB->prev_qp;
      img->qp = currMB->qp;
      currMB->prev_cbp = 0;
    }

    set_stored_macroblock_parameters ();
  }
  else
  {
    //===== set parameters for chosen mode =====
    SetModesAndRefframeForBlocks (best_mode);

    if (best_mode==P8x8)
    {
      if (currMB->luma_transform_size_8x8_flag && (cbp8_8x8ts == 0) && input->AllowTransform8x8 != 2)
        currMB->luma_transform_size_8x8_flag = 0;

      SetCoeffAndReconstruction8x8 (currMB);

      for (k=0, j=img->block_y; j<img->block_y+4; j++)
        for (     i=img->block_x; i<img->block_x+4; i++, k++)
        {
          ipredmodes    [i][j] = DC_PRED;
          currMB->intra_pred_modes[k] = DC_PRED;
        }
    }
    else
    {
      //===== set parameters for chosen mode =====
      if (best_mode == I8MB)
      {
        for(i = k = 0; i < 4; i++)
          for(j = 0; j < 4; j++,k++)
          {
            img->ipredmode[img->block_x+i][img->block_y+j] = img->ipredmode8x8[img->block_x+i][img->block_y+j];
            //only every fourth value relevant
            currMB->intra_pred_modes[k] = currMB->intra_pred_modes8x8[k];
          }

        //--- restore reconstruction for 8x8 transform ---
        for(j=0; j<MB_BLOCK_SIZE; j++)
          for(i=0; i<MB_BLOCK_SIZE; i++)
            enc_picture->imgY[img->pix_y+j][img->pix_x+i] = temp_imgY[j][i];
        // Residue Color Transform
        if(img->residue_transform_flag)
        {

           for(j=0; j<MB_BLOCK_SIZE; j++)
             for(i=0; i<MB_BLOCK_SIZE; i++)
             {
               enc_picture->imgUV[0][img->pix_y+j][img->pix_x+i] = temp_imgU[j][i] ;
               enc_picture->imgUV[1][img->pix_y+j][img->pix_x+i] = temp_imgV[j][i] ;
             }
             
        }
      }

      if ((best_mode!=I4MB)&&(best_mode != I8MB))
      {
        for (k=0, j=img->block_y; j<img->block_y+4; j++)
          for (     i=img->block_x; i<img->block_x+4; i++, k++)
          {
            ipredmodes    [i][j] = DC_PRED;
            currMB->intra_pred_modes[k] = DC_PRED;
          }
          if (best_mode!=I16MB)
          {
            if((best_mode>=1) && (best_mode<=3))
              currMB->luma_transform_size_8x8_flag = best_transform_flag;
            LumaResidualCoding ();

            if((currMB->cbp==0)&&(best_mode==0))
              currMB->luma_transform_size_8x8_flag = 0;

            //Rate control
            for (i=0; i<16; i++)
              for(j=0; j<16; j++)
                diffy[j][i] = imgY_org[img->pix_y+j][img->pix_x+i]-img->mpr[i][j];
          }
      }
    }

    //check luma cbp for transform size flag
    if (((currMB->cbp&15) == 0) && !(IS_OLDINTRA(currMB) || currMB->mb_type == I8MB))
      currMB->luma_transform_size_8x8_flag = 0;

    if (img->yuv_format != YUV400)
      // precompute all chroma intra prediction modes
      IntraChromaPrediction(NULL, NULL, NULL);
    img->i16offset = 0;
    dummy = 0;
    // Residue Color Transform
    if ((!(img->residue_transform_flag && (best_mode==I4MB || best_mode==I16MB || best_mode==I8MB))) && img->yuv_format!=YUV400)
      ChromaResidualCoding (&dummy);
    if (best_mode==I16MB)
    {
      img->i16offset = I16Offset  (currMB->cbp, i16mode);
    }
    SetMotionVectorsMB (currMB, bframe);

    //===== check for SKIP mode =====
    if ((img->type==P_SLICE || img->type==SP_SLICE) && best_mode==1 && currMB->cbp==0 &&
      enc_picture->ref_idx[LIST_0][img->block_x][img->block_y]==0 &&
      enc_picture->mv[LIST_0][img->block_x][img->block_y][0]==allmvs[0][0][0][0][0][0] &&
      enc_picture->mv[LIST_0][img->block_x][img->block_y][1]==allmvs[0][0][0][0][0][1]               )
    {
      currMB->mb_type=currMB->b8mode[0]=currMB->b8mode[1]=currMB->b8mode[2]=currMB->b8mode[3]=0;
      currMB->luma_transform_size_8x8_flag = 0;
    }

    if(img->MbaffFrameFlag)
      set_mbaff_parameters();
  }
  
  // Rate control
  if(input->RCEnable)
  {   
    if(img->type==P_SLICE)
    {
      img->MADofMB[img->current_mb_nr] = calc_MAD();
      
      if(input->basicunit<img->Frame_Total_Number_MB)
      {
        img->TotalMADBasicUnit +=img->MADofMB[img->current_mb_nr];
        
        /* delta_qp is present only for non-skipped macroblocks*/
        if ((cbp!=0 || best_mode==I16MB))
          currMB->prev_cbp = 1;
        else
        {
   	      currMB->delta_qp = 0;
          currMB->qp = currMB->prev_qp;
          img->qp = currMB->qp;
          currMB->prev_cbp = 0;
        }
        /* When MBAFF is used, delta_qp is only present for the first non-skipped macroblock of each 
        macroblock pair*/
        if (input->MbInterlace)
        {
          if(!currMB->mb_field)
          {
            DELTA_QP = currMB->delta_qp;
            QP      = currMB->qp;
          }
          else
          {
            DELTA_QP2 = currMB->delta_qp;
            QP2      = currMB->qp;
          }
        }       
      }
    }
  }
  
  if(input->rdopt)
    rdopt->min_rdcost = min_rdcost;
  else
    rdopt->min_rdcost = min_cost;

  if(img->MbaffFrameFlag)
  {
    if (img->current_mb_nr%2) //bottom
    {
      if ((currMB->mb_type ? 0:((img->type == B_SLICE) ? !currMB->cbp:1))  // bottom is skip
        &&(prevMB->mb_type ? 0:((img->type == B_SLICE) ? !prevMB->cbp:1))) // top is skip
      {
        if (!(field_flag_inference() == curr_mb_field))
        {
          rdopt->min_rdcost = 1e30;  // don't allow coding of an MB pair as skip if wrong inference
        }
      }
    }
  }

  //===== Decide if this MB will restrict the reference frames =====
  if (input->RestrictRef==1)
  {
    if (input->rdopt==1)
    {
      refresh_map[2*img->mb_y  ][2*img->mb_x  ] = (intra ? 1 : 0);
      refresh_map[2*img->mb_y  ][2*img->mb_x+1] = (intra ? 1 : 0);
      refresh_map[2*img->mb_y+1][2*img->mb_x  ] = (intra ? 1 : 0);
      refresh_map[2*img->mb_y+1][2*img->mb_x+1] = (intra ? 1 : 0);
    }
    else if (input->rdopt==2)
    {
      refresh_map[2*img->mb_y  ][2*img->mb_x  ] = (intra1==0 && (currMB->mb_type==I16MB || currMB->mb_type==I4MB) ? 1 : 0);
      refresh_map[2*img->mb_y  ][2*img->mb_x+1] = (intra1==0 && (currMB->mb_type==I16MB || currMB->mb_type==I4MB) ? 1 : 0);
      refresh_map[2*img->mb_y+1][2*img->mb_x  ] = (intra1==0 && (currMB->mb_type==I16MB || currMB->mb_type==I4MB) ? 1 : 0);
      refresh_map[2*img->mb_y+1][2*img->mb_x+1] = (intra1==0 && (currMB->mb_type==I16MB || currMB->mb_type==I4MB) ? 1 : 0);
    }
  }
  else if (input->RestrictRef==2)
  {
    refresh_map[2*img->mb_y  ][2*img->mb_x  ] = (currMB->mb_type==I16MB || currMB->mb_type==I4MB ? 1 : 0);
    refresh_map[2*img->mb_y  ][2*img->mb_x+1] = (currMB->mb_type==I16MB || currMB->mb_type==I4MB ? 1 : 0);
    refresh_map[2*img->mb_y+1][2*img->mb_x  ] = (currMB->mb_type==I16MB || currMB->mb_type==I4MB ? 1 : 0);
    refresh_map[2*img->mb_y+1][2*img->mb_x+1] = (currMB->mb_type==I16MB || currMB->mb_type==I4MB ? 1 : 0);
  }

  if(input->FMEnable)
    skip_intrabk_SAD(best_mode, listXsize[LIST_0+list_offset]);
}


void set_mbaff_parameters()
{
  int  i, j, k, l;
  Macroblock  *currMB  = &img->mb_data[img->current_mb_nr];
  int         mode     = best_mode;
  int         bframe   = (img->type==B_SLICE);
  int     **ipredmodes = img->ipredmode;

  if (!img->MbaffFrameFlag)
    return;

  //===== reconstruction values =====
  for (j=0; j<16; j++)
  for (i=0; i<16; i++)
    rdopt->rec_mbY[j][i]           = enc_picture->imgY[img->pix_y+j][img->pix_x+i];

  if (img->yuv_format != YUV400)
  {
    for (j=0; j<img->mb_cr_size_y; j++)
    for (i=0; i<img->mb_cr_size_x; i++)
    {
      rdopt->rec_mbU[j][i]           = enc_picture->imgUV[0][img->pix_c_y+j][img->pix_c_x+i];
      rdopt->rec_mbV[j][i]           = enc_picture->imgUV[1][img->pix_c_y+j][img->pix_c_x+i];
    }
  }

  //===== coefficients and cbp =====
  rdopt->mode = mode;
  rdopt->i16offset = img->i16offset;  // For MBINTLC  -Rajeev
  rdopt->cbp = currMB->cbp;
  rdopt->cbp_blk = currMB->cbp_blk;
  rdopt->mb_type  =currMB->mb_type;

  rdopt->luma_transform_size_8x8_flag = currMB->luma_transform_size_8x8_flag;

  if(rdopt->mb_type == 0 && mode != 0)
  {
    mode=0;
    rdopt->mode=0;
  }

  for(i=0;i<4+img->num_blk8x8_uv;i++)
    for(j=0;j<4;j++)
      for(k=0;k<2;k++)
        for(l=0;l<65;l++)
          rdopt->cofAC[i][j][k][l] = img->cofAC[i][j][k][l];

  for(i=0;i<3;i++)
    for(k=0;k<2;k++)
      for(l=0;l<18;l++)
        rdopt->cofDC[i][k][l] = img->cofDC[i][k][l];


  for (i=0; i<4; i++)
  {
    rdopt->b8mode[i]  = currMB->b8mode[i];
    rdopt->b8pdir[i]  = currMB->b8pdir[i];
  }

  //==== reference frames =====
  for (j=0; j<4; j++)
    for (i=0; i<4; i++)
    {
      rdopt->refar[LIST_0][j][i]       = enc_picture->ref_idx[LIST_0][img->block_x+i][img->block_y+j];
    }

  if (bframe)
  {
    for (j=0; j<4; j++)
      for (i=0; i<4; i++)
      {
        rdopt->refar[LIST_1][j][i]     = enc_picture->ref_idx[LIST_1][img->block_x+i][img->block_y+j];
      }
#if BI_PREDICTION
    rdopt->bi_pred_me = currMB->bi_pred_me;
#endif
  }


  for   (k=0, j=img->block_y; j<img->block_y+4; j++)
    for (     i=img->block_x; i<img->block_x+4; i++, k++)
    {
      rdopt->ipredmode[i][j]     = ipredmodes[i][j];
      rdopt->intra_pred_modes[k] = currMB->intra_pred_modes[k];
    }
}

/*!
************************************************************************
* \brief
*    store coding state (for rd-optimized mode decision), used for 8x8 transformation
************************************************************************
*/
void store_coding_state_cs_cm()
{
  store_coding_state(cs_cm);
}

/*!
************************************************************************
* \brief
*    restore coding state (for rd-optimized mode decision), used for 8x8 transformation
************************************************************************
*/
void reset_coding_state_cs_cm()
{
  reset_coding_state(cs_cm);
}
