
/*!
 ***************************************************************************
 * \file
 *    q_offsets.h
 *
 * \brief
 *    Headerfile for q_offsets array
 *
 * \date
 *    18. Nov 2004
 ***************************************************************************
 */

#ifndef _Q_OFFSETS_H_
#define _Q_OFFSETS_H_

extern int LevelOffset4x4Luma_Intra[13][4][4];
extern int LevelOffset4x4Chroma_Intra[2][13][4][4];

extern int LevelOffset4x4Luma_Inter[13][4][4];
extern int LevelOffset4x4Chroma_Inter[2][13][4][4];

extern int LevelOffset8x8Luma_Intra[13][8][8];
extern int LevelOffset8x8Luma_Inter[13][8][8];

void Init_QOffsetMatrix ();
void CalculateOffsetParam();
void CalculateOffset8Param();

#endif
