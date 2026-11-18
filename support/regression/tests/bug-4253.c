/* bug-4253.c
   A function tests the char result of a call while registers are in use
   at the call. With --xstack, the mcs51 port restores the registers from
   the external stack through A after the call, and the result of the call
   is in A for the conditional jump.
 */

#include <testfwk.h>

static unsigned char digits[4] = {0, 0, 5, 7};

static unsigned char
digitAt (unsigned char k)
{
  return digits[k];
}

/* One register is in use at the call. */
static unsigned char
first (void)
{
  unsigned char i;

  for (i = 0; i < 3; i++)
    {
      unsigned char d = digitAt (i);
      if (d)
        break;
    }
  return i;
}

/* The call is in the condition of the loop. */
static unsigned char
firstWhile (void)
{
  unsigned char i = 0;

  while (i < 3 && !digitAt (i))
    i++;
  return i;
}

/* Several registers are in use at the call. */
static unsigned int
sumOfIndexes (unsigned char from, unsigned char to)
{
  unsigned char i;
  unsigned int sum = 0x100;

  for (i = from; i < to; i++)
    if (digitAt (i))
      sum += i;
  return sum;
}

#pragma opt_code_size

/* With --opt-code-size, a library function restores several registers. */
static unsigned int
sumOfIndexesSize (unsigned char from, unsigned char to)
{
  unsigned char i;
  unsigned int sum = 0x100;

  for (i = from; i < to; i++)
    if (digitAt (i))
      sum += i;
  return sum;
}

void
testBug (void)
{
  ASSERT (first () == 2);
  ASSERT (firstWhile () == 2);
  ASSERT (sumOfIndexes (0, 4) == 0x105);
  ASSERT (sumOfIndexes (1, 3) == 0x102);
  ASSERT (sumOfIndexesSize (0, 4) == 0x105);
  ASSERT (sumOfIndexesSize (1, 3) == 0x102);
}
