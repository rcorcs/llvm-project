
/*!
 ***************************************************************************
 * \file
 *    q_matrix.h
 *
 * \brief
 *    Headerfile for q_matrix array
 *
 * \date
 *    07. Apr 2004
 ***************************************************************************
 */

#ifndef _Q_MATRIX_H_
#define _Q_MATRIX_H_

extern int LevelScale4x4Luma_Intra[6][4][4];
extern int LevelScale4x4Chroma_Intra[2][6][4][4];

extern int LevelScale4x4Luma_Inter[6][4][4];
extern int LevelScale4x4Chroma_Inter[2][6][4][4];

extern int LevelScale8x8Luma_Intra[6][8][8];
extern int LevelScale8x8Luma_Inter[6][8][8];

extern int InvLevelScale4x4Luma_Intra[6][4][4];
extern int InvLevelScale4x4Chroma_Intra[2][6][4][4];

extern int InvLevelScale4x4Luma_Inter[6][4][4];
extern int InvLevelScale4x4Chroma_Inter[2][6][4][4];

extern int InvLevelScale8x8Luma_Intra[6][8][8];
extern int InvLevelScale8x8Luma_Inter[6][8][8];

extern short ScalingList4x4input[6][16];
extern short ScalingList8x8input[2][64];
extern short ScalingList4x4[6][16];
extern short ScalingList8x8[2][64];

extern short UseDefaultScalingMatrix4x4Flag[6];
extern short UseDefaultScalingMatrix8x8Flag[2];


void Init_QMatrix (void);
void CalculateQuantParam();
void CalculateQuant8Param();

#endif
