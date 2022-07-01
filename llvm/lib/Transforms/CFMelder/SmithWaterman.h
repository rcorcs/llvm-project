#ifndef __SMITH_WATERMAN_H__
#define __SMITH_WATERMAN_H__
#include "SeqAlignmentUtils.h"
#include <assert.h>
#include <functional>
#include <memory>
#include <ostream>
#include <string>
using namespace std;

template <typename elemTy, typename ArrayTy, elemTy none_value>
class SmithWaterman {
private:
  ScoringFunction<elemTy> &ScoringFunc;
  bool AllowMismatches = false;

  AlignedSeq<elemTy> constructSoln(Matrix &M, ArrayTy &Seq1, ArrayTy &Seq2,
                                   int StartRow, int EndRow, int StartCol,
                                   int EndCol) {
    // find the max cost
    int Maxi = EndRow, Maxj = EndCol;
    int MaxCost = M(EndRow, EndCol).getCost();
    for (int I = StartRow; I <= EndRow; ++I) {
      for (int J = StartCol; J <= EndCol; ++J) {
        if (M(I, J).getCost() >= MaxCost) {
          Maxi = I;
          Maxj = J;
          MaxCost = M(I, J).getCost();
        }
      }
    }
    AlignedSeq<elemTy> Soln;
    int Starti = Maxi;
    int Startj = Maxj;
    while (Starti >= StartRow && Startj >= StartCol) {
      if (M(Starti, Startj).getDirection() == DIAG) {
        AlignedPair<elemTy> Point(Seq1[Starti], Seq2[Startj]);
        Soln.insert(Soln.begin(), Point);
        Starti--;
        Startj--;
      } else if (M(Starti, Startj).getDirection() == LEFT) {
        AlignedPair<elemTy> Point(none_value, Seq2[Startj]);
        Soln.insert(Soln.begin(), Point);
        Startj--;
      } else {
        AlignedPair<elemTy> Point(Seq1[Starti], none_value);
        Soln.insert(Soln.begin(), Point);
        Starti--;
      }
    }

    // find the reminder solns from end of the Matrix
    if (Maxi < EndRow && Maxj < EndCol) {
      auto SolnRight =
          constructSoln(M, Seq1, Seq2, Maxi + 1, EndRow, Maxj + 1, EndCol);
      Soln.concat(SolnRight);
    }

    // add any remainder from the begining of the Matrix
    while (Starti >= StartRow) {
      AlignedPair<elemTy> Point(Seq1[Starti], none_value);
      Soln.insert(Soln.begin(), Point);
      Starti--;
    }
    while (Startj >= StartCol) {
      AlignedPair<elemTy> Point(none_value, Seq2[Startj]);
      Soln.insert(Soln.begin(), Point);
      Startj--;
    }

    // add any remainder from the end of the Matrix
    if (Maxj == EndCol) {
      Maxi++;
      while (Maxi <= EndRow) {
        AlignedPair<elemTy> Point(Seq1[Maxi], none_value);
        Soln.insert(Soln.end(), Point);
        Maxi++;
      }
    }

    if (Maxi == EndRow) {
      Maxj++;
      while (Maxj <= EndCol) {
        AlignedPair<elemTy> Point(none_value, Seq2[Maxj]);
        Soln.insert(Soln.end(), Point);
        Maxj++;
      }
    }

    return Soln;
  }

  int findMaxGapProfit(Matrix &m, int i, int j, Direction d) {
    assert((d == Direction::LEFT || d == Direction::TOP) &&
           "invalid direction for findMaxGapProfit");
    int MaxGapProit = m(i, j).getCost() - ScoringFunc.gap(0);
    if (d == Direction::LEFT) {
      int Idx = j, GapLen = 1;
      while (Idx >= 0) {
        MaxGapProit = max(MaxGapProit,
                          m(i, j - GapLen).getCost() - ScoringFunc.gap(GapLen));
        Idx--;
        GapLen++;
      }

    } else if (d == Direction::TOP) {
      int Idx = i, GapLen = 1;
      while (Idx >= 0) {
        MaxGapProit = max(MaxGapProit,
                          m(i - GapLen, j).getCost() - ScoringFunc.gap(GapLen));
        Idx--;
        GapLen++;
      }
    }
    return MaxGapProit;
  }

public:
  SmithWaterman(ScoringFunction<elemTy> &ScoringFunc,
                bool AllowMismatches = false)
      : ScoringFunc(ScoringFunc), AllowMismatches(AllowMismatches) {}

  AlignedSeq<elemTy> compute(ArrayTy &Seq1, ArrayTy &Seq2) {
    Matrix M(Seq1.size(), Seq2.size());

    for (unsigned i = 0; i < M.getRows(); ++i) {
      for (unsigned j = 0; j < M.getCols(); ++j) {

        int DiagCost =
            M(i - 1, j - 1).getCost() + ScoringFunc(Seq1[i], Seq2[j]);
        int LeftCost = findMaxGapProfit(M, i, j - 1, Direction::LEFT);
        int TopCost = findMaxGapProfit(M, i - 1, j, Direction::TOP);
        int Cost = DiagCost;
        Direction D = DIAG;
        bool IsMatch = ScoringFunc(Seq1[i], Seq2[j]) == 0 ? false : true;

        if (Cost < LeftCost) {
          D = LEFT;
          Cost = LeftCost;
          IsMatch = false;
        }

        if (Cost < TopCost) {
          D = TOP;
          Cost = TopCost;
          IsMatch = false;
        }

        if (Cost < 0) {
          Cost = 0;
        }

        M(i, j) = Cell(Cost, D, IsMatch);
      }
    }
    // llvm::errs() << M << "\n";
    auto Soln =
        constructSoln(M, Seq1, Seq2, 0, M.getRows() - 1, 0, M.getCols() - 1);

    // remove mismatches if not allowed
    if (!AllowMismatches) {
      AlignedSeq<elemTy> MisMatchesRmvdSoln;
      for (auto Entry : Soln) {
        auto Left = Entry.getLeft();
        auto Right = Entry.getRight();

        if (Left != none_value && Right != none_value &&
            ScoringFunc(Left, Right) == 0) {
          AlignedPair<elemTy> AP1(Left, none_value);
          AlignedPair<elemTy> AP2(none_value, Right);
          MisMatchesRmvdSoln.push_back(AP1);
          MisMatchesRmvdSoln.push_back(AP2);
        } else {
          MisMatchesRmvdSoln.push_back(Entry);
        }
      }
      return MisMatchesRmvdSoln;
    }

    return Soln;
  }
};

#endif