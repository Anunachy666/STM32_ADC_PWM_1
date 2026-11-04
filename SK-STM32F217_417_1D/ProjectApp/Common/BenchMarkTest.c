/*
 *      Whetstone benchmark in C.  This program is a translation of the
 *  original Algol version in "A Synthetic Benchmark" by H.J. Curnow
 *      and B.A. Wichman in Computer Journal, Vol  19 #1, February 1976.
 *
 *  Used to test compiler optimization and floating point performance.
 */

#define FUNC_PLACEMENT	0	/* 0 - Flash, 1 - RAM  */
#if FUNC_PLACEMENT > 0
#define TYPE_FUNC	__arm __ramfunc
#else
#define TYPE_FUNC
#endif

#define ITERATIONS	10	/* 1 Million Whetstone instructions */

#include <math.h>
#include <stdio.h>


float        xx1, xx2, xx3, xx4, x, y, z, t, t1, t2;
float        e1[4];
unsigned int  i, j, k, l, n1, n2, n3, n4, n6, n7, n8, n9, n10, n11;

TYPE_FUNC void pa(float* e);
TYPE_FUNC void p3(float x, float y, float* z);
TYPE_FUNC void p0(void);

TYPE_FUNC void TestWhetstone(void)
{
  /* initialize constants */

  t   =   0.499975;
  t1  =   0.50025;
  t2  =   2.0;

  /* set values of module weights */

  n1  =   1 * ITERATIONS;
  n2  =  12 * ITERATIONS;
  n3  =  14 * ITERATIONS;
  n4  = 345 * ITERATIONS;
  n6  = 210 * ITERATIONS;
  n7  =  32 * ITERATIONS;
  n8  = 899 * ITERATIONS;
  n9  = 616 * ITERATIONS;
  n10 =   1 * ITERATIONS;
  n11 =  93 * ITERATIONS;

/* MODULE 1:  simple identifiers */

  xx1 =  1.0;
  xx2 = -1.0;
  xx3 = -1.0;
  xx4 = -1.0;

  for(i = 1; i <= n1; i += 1)
  {
    xx1 = ( xx1 + xx2 + xx3 - xx4 ) * t;
    xx2 = ( xx1 + xx2 - xx3 + xx4 ) * t;
    xx3 = ( xx1 - xx2 + xx3 + xx4 ) * t;
    xx4 = (-xx1 + xx2 + xx3 + xx4 ) * t;
  }


/* MODULE 2:  array elements */

  e1[0] =  1.0;
  e1[1] = -1.0;
  e1[2] = -1.0;
  e1[3] = -1.0;

  for (i = 1; i <= n2; i +=1)
  {
    e1[0] = ( e1[0] + e1[1] + e1[2] - e1[3] ) * t;
    e1[1] = ( e1[0] + e1[1] - e1[2] + e1[3] ) * t;
    e1[2] = ( e1[0] - e1[1] + e1[2] + e1[3] ) * t;
    e1[3] = (-e1[0] + e1[1] + e1[2] + e1[3] ) * t;
  }

/* MODULE 3:  array as parameter */

  for (i = 1; i <= n3; i += 1)
    pa(e1);

/* MODULE 4:  conditional jumps */

  j = 1;
  for (i = 1; i <= n4; i += 1)
  {
    if (j == 1)
      j = 2;
    else
      j = 3;

    if (j > 2)
      j = 0;
    else
      j = 1;

    if (j < 1 )
      j = 1;
    else
      j = 0;
  }

/* MODULE 5:  omitted */

/* MODULE 6:  integer arithmetic */

  j = 1;
  k = 2;
  l = 3;

  for (i = 1; i <= n6; i += 1)
  {
    j = j * (k - j) * (l -k);
    k = l * k - (l - j) * k;
    l = (l - k) * (k + j);

    e1[l - 2] = j + k + l;    /* C arrays are zero based */
    e1[k - 2] = j * k * l;
  }

/* MODULE 7:  trig. functions */

  x = 0.5;
  y = 0.5;

  for(i = 1; i <= n7; i +=1)
  {
    x = t * atan(t2*sin(x)*cos(x)/(cos(x+y)+cos(x-y)-1.0));
    y = t * atan(t2*sin(y)*cos(y)/(cos(x+y)+cos(x-y)-1.0));
  }

/* MODULE 8:  procedure calls */

  x = 1.0;
  y = 1.0;
  z = 1.0;

  for (i = 1; i <= n8; i +=1) p3(x, y, &z);

/* MODULE9:  array references */

  j = 1;
  k = 2;
  l = 3;

  e1[0] = 1.0;
  e1[1] = 2.0;
  e1[2] = 3.0;

  for(i = 1; i <= n9; i += 1) p0();

/* MODULE10:  integer arithmetic */

  j = 2;
  k = 3;

  for(i = 1; i <= n10; i +=1)
  {
    j = j + k;
    k = j + k;
    j = k - j;
    k = k - j - j;
  }

/* MODULE11:  standard functions */

  x = 0.75;
  for(i = 1; i <= n11; i +=1) x = sqrt( exp( log(x) / t1));
}

TYPE_FUNC void pa(float* e)
{
  int j;
  j = 0;
lab:
  e[0] = (  e[0] + e[1] + e[2] - e[3] ) * t;
  e[1] = (  e[0] + e[1] - e[2] + e[3] ) * t;
  e[2] = (  e[0] - e[1] + e[2] + e[3] ) * t;
  e[3] = ( -e[0] + e[1] + e[2] + e[3] ) / t2;
  j += 1;
  if (j < 6) goto lab;
}

TYPE_FUNC void p3(float x, float y, float* z)
{
  x  = t * (x + y);
  y  = t * (x + y);
  *z = (x + y) /t2;
}

TYPE_FUNC void p0(void)
{
  e1[j] = e1[k];
  e1[k] = e1[l];
  e1[l] = e1[j];
}
        

