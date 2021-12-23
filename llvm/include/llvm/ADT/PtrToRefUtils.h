

#ifndef LLVM_ADT_PTRTOREFUTILS_H
#define LLVM_ADT_PTRTOREFUTILS_H

#include "llvm/ADT/iterator_range.h"

template<typename T, typename PtrIteratorT>
class PtrToRefIterator {
  PtrIteratorT It;

public:
  PtrToRefIterator(PtrIteratorT It) : It(It) {}

  PtrToRefIterator &operator++() { It++; return *this; }

  bool operator==(PtrToRefIterator &other) const {return It == other.It;}
  bool operator!=(PtrToRefIterator &other) const {return !(*this == other);}

  T &operator *() { return *(*It); }
};

template<typename T, typename ContainerT>
class PtrToRefContainer {
  ContainerT &Container;

public:
  using iterator = PtrToRefIterator< T, typename ContainerT::iterator >;

  PtrToRefContainer(ContainerT &Container) : Container(Container) {}

  iterator begin() { return iterator(Container.begin()); }
  iterator end() { return iterator(Container.end()); }
};

#endif
