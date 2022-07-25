#ifndef _SPECQSORTH_
#define _SPECQSORTH_
#include "frame.h"

BEGIN_POV_NAMESPACE

//static void spec_swap(void *x, void *y, int l);

//static void spec_sort(char *array, int size, int begin, int end, int (*cmp)(void*,void*));
void spec_qsort(void *array, int nitems, int size, int (*cmp)(void*,void*));

END_POV_NAMESPACE

#endif
