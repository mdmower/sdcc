/** initializer-by-code.c
    SDCC accepts some initializers of objects with static storage duration
    that are not constant expressions. Code that runs before main() computes
    them.
 */

#include <testfwk.h>

char x, y;
char ar[4] = {11, 12, 13, 14};
int vi = 1;

/* The mcs51 small and medium models initialize global variables by code,
   in the order of their definitions. */
#if defined (__SDCC_mcs51) && (defined (__SDCC_MODEL_SMALL) || defined (__SDCC_MODEL_MEDIUM))
#define GLOBALS_BY_CODE
char elem = ar[1];
int masked = vi & 1;
char *elemPtr = &ar[vi];
unsigned char diff = (unsigned)&ar[3] - (unsigned)&ar[1];
int cond = (&x == &y) ? 1 : 2;
char condElem = (&ar[1] != &ar[0]) ? ar[2] : ar[3];
#endif

void
testGlobal (void)
{
#ifdef GLOBALS_BY_CODE
  ASSERT (elem == 12);
  ASSERT (masked == 1);
  ASSERT (elemPtr == &ar[1]);
  ASSERT (diff == 2);
  ASSERT (cond == 2);
  ASSERT (condElem == 13);
#endif
}

int
first (void)
{
  static int a = (&x == &y) ? 5 : 6;

  return a;
}

int
second (char c)
{
  static int a = (&x != &y) ? 7 : 8;
  static char b = (&ar[1] > &ar[0]) ? 9 : 10;

  return c ? a : b;
}

void
testStatic (void)
{
  ASSERT (first () == 6);
  ASSERT (second (1) == 7);
  ASSERT (second (0) == 9);
}
