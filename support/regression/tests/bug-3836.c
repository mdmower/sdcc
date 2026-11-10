/* bug-3836.c
   Comparing a pointer into a specific address space with a generic pointer
   was rejected (error 129) when the generic pointer was the right operand:
   the common type was always taken from the left operand, and a generic
   pointer cannot be converted implicitly to a more specific one.
 */

#include <testfwk.h>

#if defined(__SDCC_mcs51)
char __xdata xvar;
char __xdata *xp = &xvar;
#else
char xvar;
char *xp = &xvar;
#endif

char other;

int
addrLeft (char *g)
{
  char var = 'A';
  if (&var != g)
    return 1;
  return 0;
}

int
addrRight (char *g)
{
  char var = 'A';
  return g == &var;
}

int
specificLeft (char *g)
{
  return xp == g;
}

int
specificRight (char *g)
{
  return g == xp;
}

char *
conditional (char c, char *g)
{
  return c ? xp : g;
}

void
testBug (void)
{
  char *null = 0;

  ASSERT (addrLeft (null) == 1);
  ASSERT (addrLeft (&other) == 1);
  ASSERT (addrRight (&other) == 0);

  ASSERT (specificLeft (&xvar) == 1);
  ASSERT (specificLeft (&other) == 0);
  ASSERT (specificLeft (null) == 0);
  ASSERT (specificRight (&xvar) == 1);
  ASSERT (specificRight (&other) == 0);

  ASSERT (conditional (1, &other) == &xvar);
  ASSERT (conditional (0, &other) == &other);
}
