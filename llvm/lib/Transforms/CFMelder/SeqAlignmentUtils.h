#ifndef __SEQ_ALIGNMENT_UTIL_H__
#define __SEQ_ALIGNMENT_UTIL_H__

#include "llvm/Support/raw_ostream.h"
#include <assert.h>
#include <functional>
#include <memory>
#include <ostream>
#include <string>
#include <vector>
using namespace std;

enum Direction
{
  LEFT,
  TOP,
  DIAG,
  NONE
};

struct Cell
{
private:
  int Cost;
  Direction Direc;
  bool IsMatch;

public:
  Cell(int Value, Direction Direc, bool IsMatch) : Cost(Value), Direc(Direc), IsMatch(IsMatch) {}
  Cell(const Cell &C) : Cost(C.Cost), Direc(C.Direc), IsMatch(C.IsMatch) {}

  int getCost() const { return Cost; }
  void setCost(int Value) {Cost = Value;}
  int getDirection() const {return Direc;}
  void setDirection(Direction Value) {Direc = Value;}
  bool getMatch() const {return IsMatch;}
  friend llvm::raw_ostream &operator<<(llvm::raw_ostream &Os, const Cell &C)
  {
    Os << "(" << C.Cost << ",";
    if (C.Direc == DIAG)
      Os << "DIAG";
    else if (C.Direc == TOP)
      Os << "TOP";
    else if (C.Direc == LEFT)
      Os << "LEFT";
    else
      Os << "NONE";
    Os << ",";
    if (C.IsMatch)
      Os << "T";
    else
      Os << "F";
    Os << ")";

    return Os;
  }
};

class Matrix
{
private:
  vector<vector<Cell>> Data;
  int Rows, Cols;

public:
  Matrix(unsigned Rows, unsigned Cols) : Rows(Rows), Cols(Cols)
  {
    for (unsigned I = 0; I < Rows + 1; I++)
    {
      Data.push_back(vector<Cell>(Cols + 1, Cell(0, NONE, false)));
    }
  }

  Cell &operator()(int Row, int Col) { return Data[Row + 1][Col + 1]; }
  Cell operator()(int Row, int Col) const { return Data[Row + 1][Col + 1]; }
  int getRows() const { return Rows; }
  int getCols() const { return Cols; }

  friend llvm::raw_ostream &operator<<(llvm::raw_ostream &Os, const Matrix &M)
  {
    for (unsigned I = 0; I < M.getRows(); ++I)
    {
      for (unsigned J = 0; J < M.getCols(); ++J)
      {
        Os << M(I, J);
      }
      Os << "\n";
    }
    return Os;
  }

};

template <typename elemTy>
struct AlignedPair
{
  elemTy EL = nullptr;
  elemTy ER = nullptr;
  bool Match = false;

public:
  AlignedPair(elemTy EL, elemTy ER) : EL(EL), ER(ER), Match(EL && ER) {}
  AlignedPair(const AlignedPair& Other) {
    this->EL = Other.getLeft();
    this->ER = Other.getRight();
    this->Match = Other.match();
  }
  
  bool match() const { return Match; }
  bool missMatch() const { return !Match; }
  elemTy getLeft() const { return EL; }
  elemTy getRight() const { return ER; }
  elemTy get(int I) {
    if (I == 0) return getLeft();
    return getRight();
  }
  friend ostream &operator<<(ostream &Os, const AlignedPair<elemTy> &Ap)
  {
    if (Ap.getLeft() != nullptr)
      Os << *Ap.getLeft();
    else
      Os << "_";
    Os << " : ";
    if (Ap.getRight() != nullptr)
      Os << *Ap.getRight();
    else
      Os << "_";
    return Os;
  }

  
};

template <typename elemTy>
class AlignedSeq : public vector<AlignedPair<elemTy>>
{

public:
  using vector<AlignedPair<elemTy>>::size;
  using vector<AlignedPair<elemTy>>::end;
  using vector<AlignedPair<elemTy>>::begin;
  using vector<AlignedPair<elemTy>>::reserve;
  using vector<AlignedPair<elemTy>>::insert;

  AlignedSeq &concat(const AlignedSeq &Other)
  {

    reserve(size() + Other.size());
    insert(end(), Other.begin(), Other.end());
    return *this;
  }
};

template<typename elemTy>
class ScoringFunction {
public:
  virtual int operator()(elemTy, elemTy) = 0;
  virtual int gap(int K) = 0;
};

#endif