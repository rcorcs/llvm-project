#ifndef __NEEDLEMAN_WUNSCH_H__
#define __NEEDLEMAN_WUNSCH_H__
#include "SeqAlignmentUtils.h"
#include <assert.h>
#include <functional>
#include <memory>
#include <ostream>
#include <string>

using namespace std;

template <typename elemTy, typename ArrayTy>
class NeedlemanWunsch
{
private:
  ScoringFunction<elemTy>& CostModel;

  AlignedSeq<elemTy> constructSoln(Matrix &M, ArrayTy &Seq1, ArrayTy &Seq2)
  {
    AlignedSeq<elemTy> Result;

    int I = M.getRows() - 1;
    int J = M.getCols() - 1;

    while (I >= 0 || J >= 0)
    {

      if (I >= 0 && J >= 0 && M(I, J).getMatch())
      {
        AlignedPair<elemTy> Point(&Seq1[I], &Seq2[J]);
        Result.insert(Result.begin(), Point);
        I--;
        J--;
      }
      else if (I >= 0 && M(I, J).getDirection() == TOP)
      {
        AlignedPair<elemTy> Point(&Seq1[I], nullptr);
        Result.insert(Result.begin(), Point);
        I--;
      }
      else
      {
        AlignedPair<elemTy> Point(nullptr, &Seq2[J]);
        Result.insert(Result.begin(), Point);
        J--;
      }
    }

    return Result;
  }

public:
  NeedlemanWunsch(ScoringFunction<elemTy>& CostModel) : CostModel(CostModel) {}

  AlignedSeq<elemTy> compute(ArrayTy &Seq1, ArrayTy &Seq2)
  {
    Matrix M(Seq1.size(), Seq2.size());

    for (int I = -1; I < M.getRows(); I++)
    {
      M(I, -1).setCost(-CostModel.gap(0));
      M(I, -1).setDirection(TOP);
    }
    for (int I = -1; I < M.getCols(); I++)
    {
      M(-1, I).setCost(-CostModel.gap(0));
      M(-1, I).setDirection(LEFT);
    }

    for (unsigned I = 0; I < M.getRows(); ++I)
    {
      for (unsigned J = 0; J < M.getCols(); ++J)
      {
        int DiagCost = M(I - 1, J - 1).getCost() + CostModel(*(Seq1.begin() + I), *(Seq2.begin() + J));
        int LeftCost = M(I, J - 1).getCost() - CostModel.gap(0);
        int TopCost = M(I - 1, J).getCost() - CostModel.gap(0);
        int Cost = DiagCost;
        Direction D = DIAG;
        bool IsMatch = true;

        if (Cost < LeftCost)
        {
          D = LEFT;
          Cost = LeftCost;
          IsMatch = false;
        }

        if (Cost < TopCost)
        {
          D = TOP;
          Cost = TopCost;
          IsMatch = false;
        }

        M(I, J) = Cell(Cost, D, IsMatch);
      }
    }

    // llvm::errs() << M << "\n";

    return constructSoln(M, Seq1, Seq2);
  }
};

#endif