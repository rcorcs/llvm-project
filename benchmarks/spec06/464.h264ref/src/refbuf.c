
/*!
 ************************************************************************
 * \file refbuf.c
 *
 * \brief
 *    Declarations of teh reference frame buffer types and functions
 ************************************************************************
 */

#include <stdlib.h>

#include "global.h"

#include "refbuf.h"

/*!
 ************************************************************************
 * \brief
 *    Reference buffer write routines
 ************************************************************************
 */
void PutPel_14 (pel_t **Pic, int y, int x, pel_t val)
{
  Pic [IMG_PAD_SIZE*4+y][IMG_PAD_SIZE*4+x] = val;
}

void PutPel_11 (pel_t *Pic, int y, int x, pel_t val, int width)
{
  Pic [y*width+x] = val;
}

/*!
 ************************************************************************
 * \note
 *    The following functions returning line are NOT reentrant!  Use a buffer
 *    provided by the caller to change that (but it costs a memcpy()...
 ************************************************************************
 */
static pel_t line[16];

pel_t *FastLine16Y_11 (pel_t *Pic, int y, int x, int height, int width)
{
  return &Pic [y*width+x];
}


pel_t *UMVLine16Y_11 (pel_t *Pic, int y, int x, int height, int width)
{
  int i, maxx;
  pel_t *Picy;

  Picy = &Pic [max(0,min(height-1,y)) * width];

  if (x < 0) {                    // Left edge ?

    maxx = min(0,x+16);
    for (i = x; i < maxx; i++)
      line[i-x] = Picy [0];       // Replicate left edge pixel

    maxx = x+16;
    for (i = 0; i < maxx; i++)    // Copy non-edge pixels
      line[i-x] = Picy [i];
  }
  else if (x > width-16)  {  // Right edge ?

    maxx = width;
    for (i = x; i < maxx; i++)
      line[i-x] = Picy [i];       // Copy non-edge pixels

    maxx = x+16;
    for (i = max(width,x); i < maxx; i++)
      line[i-x] = Picy [width-1];  // Replicate right edge pixel
  }
  else                            // No edge
    return &Picy [x];

  return line;
}


pel_t *FastLineX (int dummy, pel_t* Pic, int y, int x, int height, int width)
{
  return Pic + y*width + x;
}


pel_t *UMVLineX (int size, pel_t* Pic, int y, int x, int height, int width)
{
  int i, maxx;
  pel_t *Picy;

  Picy = Pic + max(0,min(height-1,y)) * width;

  if (x < 0)                            // Left edge
  {
    maxx = min(0,x+size);
    for (i = x; i < maxx; i++)
    {
      line[i-x] = Picy [0];             // Replicate left edge pixel
    }
    maxx = x+size;
    for (i = 0; i < maxx; i++)          // Copy non-edge pixels
      line[i-x] = Picy [i];
  }
  else if (x > width-size)         // Right edge
  {
    maxx = width;
    for (i = x; i < maxx; i++)
    {
      line[i-x] = Picy [i];             // Copy non-edge pixels
    }
    maxx = x+size;
    for (i = max(width,x); i < maxx; i++)
    {
      line[i-x] = Picy [width-1];  // Replicate right edge pixel
    }
  }
  else                                  // No edge
  {
    return Picy + x;
  }

  return line;
}

/*!
 ************************************************************************
 * \brief
 *    Reference buffer, 1/4 pel
 ************************************************************************
 */
pel_t UMVPelY_14 (pel_t **Pic, int y, int x, int height, int width)
{
  int width4  = ((width+2*IMG_PAD_SIZE-1)<<2);
  int height4 = ((height+2*IMG_PAD_SIZE-1)<<2);

  x = x + IMG_PAD_SIZE*4;
  y = y + IMG_PAD_SIZE*4;

  if (x < 0)
  {
    if (y < 0)
      return Pic [y&3][x&3];
    if (y > height4)
      return Pic [height4+(y&3)][x&3];
    return Pic [y][x&3];
  }

  if (x > width4)
  {
    if (y < 0)
      return Pic [y&3][width4+(x&3)];
    if (y > height4)
      return Pic [height4+(y&3)][width4+(x&3)];
    return Pic [y][width4+(x&3)];
  }

  if (y < 0)    // note: corner pixels were already processed
    return Pic [y&3][x];
  if (y > height4)
    return Pic [height4+(y&3)][x];

  return Pic [y][x];
}

pel_t FastPelY_14 (pel_t **Pic, int y, int x, int height, int width)
{
  return Pic [IMG_PAD_SIZE*4+y][IMG_PAD_SIZE*4+x];
}


