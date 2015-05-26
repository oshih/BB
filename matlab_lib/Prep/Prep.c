/*
 * Prep.c
 *
 * Code generation for function 'Prep'
 *
 */

/* Include files */
#include "rt_nonfinite.h"
#include "Prep.h"
#include "Prep_emxutil.h"
#include "ipermute.h"

/* Function Declarations */
static void b_eml_fft(const float x[10000], int n, emxArray_creal32_T *y);
static int div_s32(int numerator, int denominator);
static void eml_fft(const emxArray_real32_T *x, int n, emxArray_creal32_T *y);
static float rt_hypotf_snf(float u0, float u1);
static double rt_powd_snf(double u0, double u1);

/* Function Definitions */
static void b_eml_fft(const float x[10000], int n, emxArray_creal32_T *y)
{
  int nd2;
  int minval;
  int ixDelta;
  emxArray_real32_T *costab1q;
  int nRowsD2;
  int nRowsD4;
  float e;
  int k;
  emxArray_real32_T *costab;
  emxArray_real32_T *sintab;
  int b_n;
  int n2;
  int ix;
  int i;
  boolean_T tst;
  float temp_re;
  float temp_im;
  int iDelta;
  int iDelta2;
  int iheight;
  float twid_im;
  nd2 = y->size[0];
  y->size[0] = n;
  emxEnsureCapacity((emxArray__common *)y, nd2, (int)sizeof(creal32_T));
  if (n > 10000) {
    nd2 = y->size[0];
    y->size[0] = n;
    emxEnsureCapacity((emxArray__common *)y, nd2, (int)sizeof(creal32_T));
    for (nd2 = 0; nd2 < n; nd2++) {
      y->data[nd2].re = 0.0F;
      y->data[nd2].im = 0.0F;
    }
  }

  if (n <= 10000) {
    minval = n;
  } else {
    minval = 10000;
  }

  if (1 >= 10001 - minval) {
    ixDelta = 1;
  } else {
    ixDelta = 10001 - minval;
  }

  emxInit_real32_T(&costab1q, 2);
  nRowsD2 = n / 2;
  nRowsD4 = nRowsD2 / 2;
  e = 6.28318548F / (float)n;
  nd2 = costab1q->size[0] * costab1q->size[1];
  costab1q->size[0] = 1;
  costab1q->size[1] = nRowsD4 + 1;
  emxEnsureCapacity((emxArray__common *)costab1q, nd2, (int)sizeof(float));
  costab1q->data[0] = 1.0F;
  nd2 = nRowsD4 / 2;
  for (k = 1; k <= nd2; k++) {
    costab1q->data[k] = (real32_T)cos(e * (float)k);
  }

  for (k = nd2 + 1; k < nRowsD4; k++) {
    costab1q->data[k] = (real32_T)sin(e * (float)(nRowsD4 - k));
  }

  emxInit_real32_T(&costab, 2);
  emxInit_real32_T(&sintab, 2);
  costab1q->data[nRowsD4] = 0.0F;
  b_n = costab1q->size[1] - 1;
  n2 = (costab1q->size[1] - 1) << 1;
  nd2 = costab->size[0] * costab->size[1];
  costab->size[0] = 1;
  costab->size[1] = n2 + 1;
  emxEnsureCapacity((emxArray__common *)costab, nd2, (int)sizeof(float));
  nd2 = sintab->size[0] * sintab->size[1];
  sintab->size[0] = 1;
  sintab->size[1] = n2 + 1;
  emxEnsureCapacity((emxArray__common *)sintab, nd2, (int)sizeof(float));
  costab->data[0] = 1.0F;
  sintab->data[0] = 0.0F;
  for (k = 1; k <= b_n; k++) {
    costab->data[k] = costab1q->data[k];
    sintab->data[k] = -costab1q->data[b_n - k];
  }

  for (k = costab1q->size[1]; k <= n2; k++) {
    costab->data[k] = -costab1q->data[n2 - k];
    sintab->data[k] = -costab1q->data[k - b_n];
  }

  emxFree_real32_T(&costab1q);
  ix = 0;
  nd2 = 0;
  while ((n > 0) && (nd2 <= 0)) {
    n2 = 0;
    nd2 = 0;
    for (i = 1; i < minval; i++) {
      y->data[nd2].re = x[ix];
      y->data[nd2].im = 0.0F;
      b_n = n;
      tst = true;
      while (tst) {
        b_n >>= 1;
        n2 ^= b_n;
        tst = ((n2 & b_n) == 0);
      }

      nd2 = n2;
      ix++;
    }

    y->data[nd2].re = x[ix];
    y->data[nd2].im = 0.0F;
    ix += ixDelta;
    if (n > 1) {
      for (i = 0; i <= n - 2; i += 2) {
        temp_re = y->data[i + 1].re;
        temp_im = y->data[i + 1].im;
        y->data[i + 1].re = y->data[i].re - y->data[i + 1].re;
        y->data[i + 1].im = y->data[i].im - y->data[i + 1].im;
        y->data[i].re += temp_re;
        y->data[i].im += temp_im;
      }
    }

    iDelta = 2;
    iDelta2 = 4;
    k = nRowsD4;
    iheight = 1 + ((nRowsD4 - 1) << 2);
    while (k > 0) {
      for (i = 0; i < iheight; i += iDelta2) {
        nd2 = i + iDelta;
        temp_re = y->data[nd2].re;
        temp_im = y->data[nd2].im;
        y->data[i + iDelta].re = y->data[i].re - y->data[nd2].re;
        y->data[i + iDelta].im = y->data[i].im - y->data[nd2].im;
        y->data[i].re += temp_re;
        y->data[i].im += temp_im;
      }

      nd2 = 1;
      for (n2 = k; n2 < nRowsD2; n2 += k) {
        e = costab->data[n2];
        twid_im = sintab->data[n2];
        i = nd2;
        b_n = nd2 + iheight;
        while (i < b_n) {
          temp_re = e * y->data[i + iDelta].re - twid_im * y->data[i + iDelta].
            im;
          temp_im = e * y->data[i + iDelta].im + twid_im * y->data[i + iDelta].
            re;
          y->data[i + iDelta].re = y->data[i].re - temp_re;
          y->data[i + iDelta].im = y->data[i].im - temp_im;
          y->data[i].re += temp_re;
          y->data[i].im += temp_im;
          i += iDelta2;
        }

        nd2++;
      }

      k /= 2;
      iDelta = iDelta2;
      iDelta2 <<= 1;
      iheight -= iDelta;
    }

    nd2 = n;
  }

  emxFree_real32_T(&sintab);
  emxFree_real32_T(&costab);
}

static int div_s32(int numerator, int denominator)
{
  int quotient;
  unsigned int absNumerator;
  unsigned int absDenominator;
  boolean_T quotientNeedsNegation;
  if (denominator == 0) {
    if (numerator >= 0) {
      quotient = MAX_int32_T;
    } else {
      quotient = MIN_int32_T;
    }
  } else {
    if (numerator >= 0) {
      absNumerator = (unsigned int)numerator;
    } else {
      absNumerator = (unsigned int)-numerator;
    }

    if (denominator >= 0) {
      absDenominator = (unsigned int)denominator;
    } else {
      absDenominator = (unsigned int)-denominator;
    }

    quotientNeedsNegation = ((numerator < 0) != (denominator < 0));
    absNumerator /= absDenominator;
    if (quotientNeedsNegation) {
      quotient = -(int)absNumerator;
    } else {
      quotient = (int)absNumerator;
    }
  }

  return quotient;
}

static void eml_fft(const emxArray_real32_T *x, int n, emxArray_creal32_T *y)
{
  int nd2;
  int u1;
  int ixDelta;
  emxArray_real32_T *costab1q;
  int nRowsD2;
  int nRowsD4;
  int lastChan;
  float e;
  int k;
  emxArray_real32_T *costab;
  emxArray_real32_T *sintab;
  int b_n;
  int n2;
  int ix;
  int chanStart;
  int i;
  boolean_T tst;
  float temp_re;
  float temp_im;
  int iDelta2;
  int iheight;
  int ihi;
  float twid_im;
  nd2 = y->size[0];
  y->size[0] = n;
  emxEnsureCapacity((emxArray__common *)y, nd2, (int)sizeof(creal32_T));
  if (n > x->size[0]) {
    nd2 = y->size[0];
    y->size[0] = n;
    emxEnsureCapacity((emxArray__common *)y, nd2, (int)sizeof(creal32_T));
    for (nd2 = 0; nd2 < n; nd2++) {
      y->data[nd2].re = 0.0F;
      y->data[nd2].im = 0.0F;
    }
  }

  if (x->size[0] == 0) {
  } else {
    u1 = x->size[0];
    if (n <= u1) {
      u1 = n;
    }

    nd2 = (x->size[0] - u1) + 1;
    if (1 >= nd2) {
      ixDelta = 1;
    } else {
      ixDelta = nd2;
    }

    emxInit_real32_T(&costab1q, 2);
    nRowsD2 = n / 2;
    nRowsD4 = nRowsD2 / 2;
    lastChan = n * (div_s32(x->size[0], x->size[0]) - 1);
    e = 6.28318548F / (float)n;
    nd2 = costab1q->size[0] * costab1q->size[1];
    costab1q->size[0] = 1;
    costab1q->size[1] = nRowsD4 + 1;
    emxEnsureCapacity((emxArray__common *)costab1q, nd2, (int)sizeof(float));
    costab1q->data[0] = 1.0F;
    nd2 = nRowsD4 / 2;
    for (k = 1; k <= nd2; k++) {
      costab1q->data[k] = (real32_T)cos(e * (float)k);
    }

    for (k = nd2 + 1; k < nRowsD4; k++) {
      costab1q->data[k] = (real32_T)sin(e * (float)(nRowsD4 - k));
    }

    emxInit_real32_T(&costab, 2);
    emxInit_real32_T(&sintab, 2);
    costab1q->data[nRowsD4] = 0.0F;
    b_n = costab1q->size[1] - 1;
    n2 = (costab1q->size[1] - 1) << 1;
    nd2 = costab->size[0] * costab->size[1];
    costab->size[0] = 1;
    costab->size[1] = n2 + 1;
    emxEnsureCapacity((emxArray__common *)costab, nd2, (int)sizeof(float));
    nd2 = sintab->size[0] * sintab->size[1];
    sintab->size[0] = 1;
    sintab->size[1] = n2 + 1;
    emxEnsureCapacity((emxArray__common *)sintab, nd2, (int)sizeof(float));
    costab->data[0] = 1.0F;
    sintab->data[0] = 0.0F;
    for (k = 1; k <= b_n; k++) {
      costab->data[k] = costab1q->data[k];
      sintab->data[k] = -costab1q->data[b_n - k];
    }

    for (k = costab1q->size[1]; k <= n2; k++) {
      costab->data[k] = -costab1q->data[n2 - k];
      sintab->data[k] = -costab1q->data[k - b_n];
    }

    emxFree_real32_T(&costab1q);
    ix = 0;
    chanStart = 0;
    while ((n > 0) && (chanStart <= lastChan)) {
      n2 = 0;
      nd2 = chanStart;
      for (i = 1; i < u1; i++) {
        y->data[nd2].re = x->data[ix];
        y->data[nd2].im = 0.0F;
        b_n = n;
        tst = true;
        while (tst) {
          b_n >>= 1;
          n2 ^= b_n;
          tst = ((n2 & b_n) == 0);
        }

        nd2 = chanStart + n2;
        ix++;
      }

      y->data[nd2].re = x->data[ix];
      y->data[nd2].im = 0.0F;
      ix += ixDelta;
      nd2 = (chanStart + n) - 2;
      if (n > 1) {
        for (i = chanStart; i <= nd2; i += 2) {
          temp_re = y->data[i + 1].re;
          temp_im = y->data[i + 1].im;
          y->data[i + 1].re = y->data[i].re - y->data[i + 1].re;
          y->data[i + 1].im = y->data[i].im - y->data[i + 1].im;
          y->data[i].re += temp_re;
          y->data[i].im += temp_im;
        }
      }

      b_n = 2;
      iDelta2 = 4;
      k = nRowsD4;
      iheight = 1 + ((nRowsD4 - 1) << 2);
      while (k > 0) {
        i = chanStart;
        ihi = chanStart + iheight;
        while (i < ihi) {
          nd2 = i + b_n;
          temp_re = y->data[nd2].re;
          temp_im = y->data[nd2].im;
          y->data[i + b_n].re = y->data[i].re - y->data[nd2].re;
          y->data[i + b_n].im = y->data[i].im - y->data[nd2].im;
          y->data[i].re += temp_re;
          y->data[i].im += temp_im;
          i += iDelta2;
        }

        nd2 = chanStart + 1;
        for (n2 = k; n2 < nRowsD2; n2 += k) {
          e = costab->data[n2];
          twid_im = sintab->data[n2];
          i = nd2;
          ihi = nd2 + iheight;
          while (i < ihi) {
            temp_re = e * y->data[i + b_n].re - twid_im * y->data[i + b_n].im;
            temp_im = e * y->data[i + b_n].im + twid_im * y->data[i + b_n].re;
            y->data[i + b_n].re = y->data[i].re - temp_re;
            y->data[i + b_n].im = y->data[i].im - temp_im;
            y->data[i].re += temp_re;
            y->data[i].im += temp_im;
            i += iDelta2;
          }

          nd2++;
        }

        k /= 2;
        b_n = iDelta2;
        iDelta2 <<= 1;
        iheight -= b_n;
      }

      chanStart += n;
    }

    emxFree_real32_T(&sintab);
    emxFree_real32_T(&costab);
  }
}

static float rt_hypotf_snf(float u0, float u1)
{
  float y;
  float a;
  float b;
  a = (real32_T)fabs(u0);
  b = (real32_T)fabs(u1);
  if (a < b) {
    a /= b;
    y = b * (real32_T)sqrt(a * a + 1.0F);
  } else if (a > b) {
    b /= a;
    y = a * (real32_T)sqrt(b * b + 1.0F);
  } else if (rtIsNaNF(b)) {
    y = b;
  } else {
    y = a * 1.41421354F;
  }

  return y;
}

static double rt_powd_snf(double u0, double u1)
{
  double y;
  double d0;
  double d1;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = rtNaN;
  } else {
    d0 = fabs(u0);
    d1 = fabs(u1);
    if (rtIsInf(u1)) {
      if (d0 == 1.0) {
        y = rtNaN;
      } else if (d0 > 1.0) {
        if (u1 > 0.0) {
          y = rtInf;
        } else {
          y = 0.0;
        }
      } else if (u1 > 0.0) {
        y = 0.0;
      } else {
        y = rtInf;
      }
    } else if (d1 == 0.0) {
      y = 1.0;
    } else if (d1 == 1.0) {
      if (u1 > 0.0) {
        y = u0;
      } else {
        y = 1.0 / u0;
      }
    } else if (u1 == 2.0) {
      y = u0 * u0;
    } else if ((u1 == 0.5) && (u0 >= 0.0)) {
      y = sqrt(u0);
    } else if ((u0 < 0.0) && (u1 > floor(u1))) {
      y = rtNaN;
    } else {
      y = pow(u0, u1);
    }
  }

  return y;
}

void Prep(const emxArray_real32_T *raw_data, const float m_filter[10000],
          emxArray_real32_T *prc_data)
{
  int asub;
  double delta1;
  double NFFT;
  emxArray_real_T *f;
  int b_f;
  emxArray_real32_T *b_raw_data;
  emxArray_creal32_T *Y_sig;
  emxArray_creal32_T *b_y1;
  float raw_data_re;
  float Y_sig_re;
  float Y_sig_im;
  emxArray_creal32_T *b;
  emxArray_creal32_T *tmp_data;
  int bsub;
  int ak;
  int bk;
  int ck;
  int32_T exitg1;
  float b_im;
  emxArray_real32_T *b_tmp_data;
  emxArray_boolean_T *x;
  emxArray_int32_T *y;

  /*  PREPROCESSING Summary of this function goes here */
  /*  Take the raw data, match filtered it and output in frequency domain */
  /* coder.inline('never'); */
  delta1 = frexp(raw_data->size[1], &asub);
  NFFT = asub;
  if (delta1 == 0.5) {
    NFFT = (double)asub - 1.0;
  }

  emxInit_real_T(&f, 2);
  NFFT = rt_powd_snf(2.0, NFFT);

  /*  Next power of 2 from length of y */
  delta1 = floor(NFFT / 2.0 + 1.0);
  b_f = f->size[0] * f->size[1];
  f->size[0] = 1;
  f->size[1] = (int)delta1;
  emxEnsureCapacity((emxArray__common *)f, b_f, (int)sizeof(double));
  f->data[(int)delta1 - 1] = 1.0;
  if (f->size[1] >= 2) {
    f->data[0] = 0.0;
    if (f->size[1] >= 3) {
      delta1 = 1.0 / ((double)f->size[1] - 1.0);
      b_f = f->size[1];
      for (asub = 0; asub <= b_f - 3; asub++) {
        f->data[asub + 1] = (1.0 + (double)asub) * delta1;
      }
    }
  }

  b_f = f->size[0] * f->size[1];
  f->size[0] = 1;
  emxEnsureCapacity((emxArray__common *)f, b_f, (int)sizeof(double));
  asub = f->size[0];
  b_f = f->size[1];
  asub *= b_f;
  for (b_f = 0; b_f < asub; b_f++) {
    f->data[b_f] *= 96000.0;
  }

  b_emxInit_real32_T(&b_raw_data, 1);

  /*  frequency band */
  b_f = b_raw_data->size[0];
  b_raw_data->size[0] = raw_data->size[1];
  emxEnsureCapacity((emxArray__common *)b_raw_data, b_f, (int)sizeof(float));
  asub = raw_data->size[1];
  for (b_f = 0; b_f < asub; b_f++) {
    b_raw_data->data[b_f] = raw_data->data[raw_data->size[0] * b_f];
  }

  emxInit_creal32_T(&Y_sig, 2);
  b_emxInit_creal32_T(&b_y1, 1);
  eml_fft(b_raw_data, (int)NFFT, b_y1);
  ipermute(b_y1, Y_sig);
  b_f = Y_sig->size[0] * Y_sig->size[1];
  Y_sig->size[0] = 1;
  emxEnsureCapacity((emxArray__common *)Y_sig, b_f, (int)sizeof(creal32_T));
  asub = Y_sig->size[0];
  b_f = Y_sig->size[1];
  raw_data_re = (float)raw_data->size[1];
  asub *= b_f;
  emxFree_real32_T(&b_raw_data);
  for (b_f = 0; b_f < asub; b_f++) {
    Y_sig_re = Y_sig->data[b_f].re;
    Y_sig_im = Y_sig->data[b_f].im;
    if (Y_sig_im == 0.0F) {
      Y_sig->data[b_f].re = Y_sig_re / raw_data_re;
      Y_sig->data[b_f].im = 0.0F;
    } else if (Y_sig_re == 0.0F) {
      Y_sig->data[b_f].re = 0.0F;
      Y_sig->data[b_f].im = Y_sig_im / raw_data_re;
    } else {
      Y_sig->data[b_f].re = Y_sig_re / raw_data_re;
      Y_sig->data[b_f].im = Y_sig_im / raw_data_re;
    }
  }

  emxInit_creal32_T(&b, 2);
  b_eml_fft(m_filter, (int)NFFT, b_y1);
  ipermute(b_y1, b);
  b_f = b->size[0] * b->size[1];
  b->size[0] = 1;
  emxEnsureCapacity((emxArray__common *)b, b_f, (int)sizeof(creal32_T));
  asub = b->size[0];
  b_f = b->size[1];
  asub *= b_f;
  emxFree_creal32_T(&b_y1);
  for (b_f = 0; b_f < asub; b_f++) {
    b->data[b_f].im = -b->data[b_f].im;
  }

  emxInit_creal32_T(&tmp_data, 2);
  b_f = tmp_data->size[0] * tmp_data->size[1];
  tmp_data->size[0] = 1;
  tmp_data->size[1] = Y_sig->size[1];
  emxEnsureCapacity((emxArray__common *)tmp_data, b_f, (int)sizeof(creal32_T));
  b_f = Y_sig->size[1];
  if (b_f == 0) {
  } else {
    asub = 1;
    bsub = 1;
    ak = 0;
    bk = 0;
    ck = 0;
    do {
      exitg1 = 0;
      b_f = Y_sig->size[1];
      if (ck <= b_f - 1) {
        Y_sig_re = Y_sig->data[ak].re;
        Y_sig_im = Y_sig->data[ak].im;
        raw_data_re = b->data[bk].re;
        b_im = b->data[bk].im;
        tmp_data->data[ck].re = Y_sig_re * raw_data_re - Y_sig_im * b_im;
        tmp_data->data[ck].im = Y_sig_re * b_im + Y_sig_im * raw_data_re;
        if (asub < Y_sig->size[1]) {
          ak++;
          bk++;
          bsub++;
          asub++;
        } else if (bsub < b->size[1]) {
          bk++;
          bsub++;
        } else {
          asub = 1;
          bsub = 1;
        }

        ck++;
      } else {
        exitg1 = 1;
      }
    } while (exitg1 == 0);
  }

  emxFree_creal32_T(&b);
  delta1 = NFFT / 2.0;
  NFFT /= 2.0;
  b_f = Y_sig->size[0] * Y_sig->size[1];
  Y_sig->size[0] = 1;
  Y_sig->size[1] = (int)floor(NFFT) + 1;
  emxEnsureCapacity((emxArray__common *)Y_sig, b_f, (int)sizeof(creal32_T));
  asub = (int)floor(NFFT);
  for (b_f = 0; b_f <= asub; b_f++) {
    Y_sig->data[Y_sig->size[0] * b_f] = tmp_data->data[b_f];
  }

  emxFree_creal32_T(&tmp_data);
  emxInit_real32_T(&b_tmp_data, 2);
  b_f = b_tmp_data->size[0] * b_tmp_data->size[1];
  b_tmp_data->size[0] = 1;
  b_tmp_data->size[1] = (int)floor(delta1) + 1;
  emxEnsureCapacity((emxArray__common *)b_tmp_data, b_f, (int)sizeof(float));
  b_f = (int)floor(delta1);
  for (asub = 0; asub <= b_f; asub++) {
    b_tmp_data->data[asub] = rt_hypotf_snf(Y_sig->data[asub].re, Y_sig->
      data[asub].im);
  }

  emxFree_creal32_T(&Y_sig);
  b_f = b_tmp_data->size[0] * b_tmp_data->size[1];
  b_tmp_data->size[0] = 1;
  emxEnsureCapacity((emxArray__common *)b_tmp_data, b_f, (int)sizeof(float));
  asub = b_tmp_data->size[0];
  b_f = b_tmp_data->size[1];
  asub *= b_f;
  for (b_f = 0; b_f < asub; b_f++) {
    b_tmp_data->data[b_f] *= 2.0F;
  }

  emxInit_boolean_T(&x, 2);
  b_f = x->size[0] * x->size[1];
  x->size[0] = 1;
  x->size[1] = f->size[1];
  emxEnsureCapacity((emxArray__common *)x, b_f, (int)sizeof(boolean_T));
  asub = f->size[0] * f->size[1];
  for (b_f = 0; b_f < asub; b_f++) {
    x->data[b_f] = ((20000.0 < f->data[b_f]) && (f->data[b_f] < 23000.0));
  }

  emxFree_real_T(&f);
  bsub = x->size[1];
  asub = 0;
  for (b_f = 1; b_f <= bsub; b_f++) {
    if (x->data[b_f - 1]) {
      asub++;
    }
  }

  emxInit_int32_T(&y, 2);
  b_f = y->size[0] * y->size[1];
  y->size[0] = 1;
  y->size[1] = asub;
  emxEnsureCapacity((emxArray__common *)y, b_f, (int)sizeof(int));
  asub = 0;
  for (b_f = 1; b_f <= bsub; b_f++) {
    if (x->data[b_f - 1]) {
      y->data[asub] = b_f;
      asub++;
    }
  }

  emxFree_boolean_T(&x);
  b_f = prc_data->size[0] * prc_data->size[1];
  prc_data->size[0] = 1;
  prc_data->size[1] = y->size[1];
  emxEnsureCapacity((emxArray__common *)prc_data, b_f, (int)sizeof(float));
  asub = y->size[0] * y->size[1];
  for (b_f = 0; b_f < asub; b_f++) {
    prc_data->data[b_f] = b_tmp_data->data[y->data[b_f] - 1];
  }

  emxFree_int32_T(&y);
  emxFree_real32_T(&b_tmp_data);
}

/* End of code generation (Prep.c) */
