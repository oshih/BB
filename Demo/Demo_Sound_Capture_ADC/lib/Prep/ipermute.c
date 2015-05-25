/*
 * ipermute.c
 *
 * Code generation for function 'ipermute'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "Prep.h"
#include "ipermute.h"
#include "Prep_emxutil.h"

/* Function Definitions */
void ipermute(const emxArray_creal32_T *b, emxArray_creal32_T *a)
{
  int i0;
  int loop_ub;
  i0 = a->size[0] * a->size[1];
  a->size[0] = 1;
  a->size[1] = b->size[0];
  emxEnsureCapacity((emxArray__common *)a, i0, (int)sizeof(creal32_T));
  loop_ub = b->size[0];
  for (i0 = 0; i0 < loop_ub; i0++) {
    a->data[a->size[0] * i0] = b->data[i0];
  }
}

/* End of code generation (ipermute.c) */
