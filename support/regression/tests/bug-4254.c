/* bug-4254.c
   A function tests the int result of a call for zero while registers are
   in use at the call. The mcs51 port has the result in A and B for the
   test. With --xstack, it restores the registers from the external stack
   after the call, through A or with a library function that takes its
   arguments in A and B.
 */

#include <testfwk.h>

static unsigned int table[3] = {0x0100, 0, 0x0001};
static unsigned int usedA, usedB;

static unsigned int
get (unsigned char k)
{
  return table[k];
}

static void
use (unsigned int a, unsigned int b)
{
  usedA = a;
  usedB = b;
}

/* a is in registers at the call of get(). */
static unsigned char
test (unsigned int a, unsigned int b, unsigned char k)
{
  if (!get (k))
    return 0;
  use (a, b);
  return 1;
}

#pragma opt_code_size

/* With --opt-code-size, a library function restores the registers. */
static unsigned char
testSize (unsigned int a, unsigned int b, unsigned char k)
{
  if (!get (k))
    return 0;
  use (a, b);
  return 1;
}

void
testBug (void)
{
  ASSERT (test (0x1234, 0x5678, 1) == 0);
  ASSERT (usedA == 0);
  ASSERT (test (0, 0x5678, 2) == 1);
  ASSERT (usedA == 0 && usedB == 0x5678);
  ASSERT (test (0x1234, 0, 0) == 1);
  ASSERT (usedA == 0x1234 && usedB == 0);

  usedA = 0;
  ASSERT (testSize (0x1234, 0x5678, 1) == 0);
  ASSERT (usedA == 0);
  ASSERT (testSize (0, 0x5678, 2) == 1);
  ASSERT (usedA == 0 && usedB == 0x5678);
  ASSERT (testSize (0x1234, 0, 0) == 1);
  ASSERT (usedA == 0x1234 && usedB == 0);
}
